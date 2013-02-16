#include "domhelper.h"


DomHelper::DomHelper(QObject *hostDelegate, QString docType, QString rootTag, QString fileExtension) :
    m_document(),
    m_hostObject(hostDelegate),
    m_docType(docType),
    m_rootTag(rootTag),
    m_fileSuffix(fileExtension),
    m_importingDomData(false)
{
}

DomHelper::DomHelper() :
    m_document(),
    m_hostObject(NULL),
    m_docType(DOMHELPER_DEFAULT_DOCTYPE),
    m_rootTag(DOMHELPER_DEFAULT_ROOT_TAG),
    m_fileSuffix(DOMHELPER_DEFAULT_FILE_SUFFIX),
    m_importingDomData(false)
{
}

DomHelper::~DomHelper() {
    deleteDomDocument();
}

void DomHelper::setHostObject(QObject *obj) {
    m_hostObject=obj;
}

void DomHelper::removeAllDocumentChildNodes() {
    while (m_document->hasChildNodes()) {
        QDomNode n=m_document->firstChild();
        removeAllChildNodes(n);
        m_document->removeChild(n);
    }
}

void DomHelper::removeAllChildNodes(QDomNode& node) {
    while (node.hasChildNodes()) {
        QDomNode n=node.firstChild();
        removeAllChildNodes(n);
        node.removeChild(n);
    }
}

bool DomHelper::selfObjectData() {

    if (isImportingDomData()) return false;
    //clearing document
    initDomDocument();

    QDomElement _rootElement = m_document->createElement(m_rootTag);
    _rootElement.setAttribute(DOMHELPER_VERSION_ATTRIBUTE,DOMHELPER_VERSION);
    m_document->appendChild(_rootElement);

    //Inserting class name
    QDomElement _classname = m_document->createElement(DOMHELPER_OBJECTTYPE_TAG);
    QDomText _classnameText = m_document->createTextNode(m_hostObject->metaObject()->className());
    _rootElement.appendChild(_classname);
    _classname.appendChild(_classnameText);

    //qDebug() << "DomHelper::selfObjectData generate self data with TAG "<< rootTag;
    //qDebug() << "DomHelper::selfObjectData start with tag |" << rootTag <<"|";
    //qDebug() << "DomHelper::selfObjectData start  | "<< doc->toText().data()<< " |end";
    const QMetaObject* metaObject = m_hostObject->metaObject();
    for(int i = 0; i < metaObject->propertyCount(); ++i) {
        QMetaProperty _prop=metaObject->property(i);
        QString _propName=_prop.name();
        QVariant _propValueVariant=_prop.read(m_hostObject);
        //qDebug() << "DomHelper::generateDomDocument class "<< metaObject->className()<< " prop is " << _propName;
        //Save parameters that can be converted in string, others are rejected
        if (_propName=="uuid") continue; //uuid is not a memeber of qvariant
        if (_propValueVariant.canConvert(QVariant::String)) {
            //appending element node
            QDomElement _element=m_document->createElement(_propName);
            _rootElement.appendChild(_element);
            //appending text node to previous element node
            QString _propValue=_propValueVariant.toString();
            QDomText _elementValue=m_document->createTextNode(_propValue);
            _element.appendChild(_elementValue);
            //qDebug() << "DomHelper::selfObjectData start  | "<< doc->toText().data()<< " |end";
        } else {
            PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,QString("can convert property %1 ").arg(_propName),ErrorMessage::WARNINGMESSAGE));
        }
    }
    return true;
}

void DomHelper::deleteDomDocument() {
    if (m_document && !m_document->isNull()) {
       removeAllDocumentChildNodes();
       m_document->clear();
    }
    if (m_document) {
        delete m_document;
        m_document=NULL;
    }
}

void DomHelper::initDomDocument() {

    deleteDomDocument();
    //Set up document
    m_document=new  QDomDocument(m_docType);

}


//void DomHelper::initDomDocument(const QString &rootTag) {
//    //Set up document
//    if (m_doc!=NULL) delete m_doc;
//    m_doc=new QDomDocument( (new QString(rootTag))->append("_").append(m_obj->metaObject()->className()));
//    QDomElement _root=m_doc->createElement(rootTag);
//    m_doc->appendChild(_root);
//}

//bool DomHelper::appendDomDocument(const QDomDocument& doc) {
//    return this->appendDomDocument(&doc);
//}

//bool DomHelper::appendDomDocument(const QDomDocument *doc) {
//    if (m_doc==NULL) {
//    //    qDebug() << "DomHelper::appendDomDocument m_doc NULL, regenerate document";
//    //    generateDomDocument();
//        qWarning() << "DomHelper::appendDomDocument can't append to a NULL document";
//        return false;
//    }
//  //  qDebug() << "DomHelper::appendDomDocument  processing node " << doc->nodeName();
//    if (!doc->isNull()) {
//        m_doc->firstChild().appendChild(doc->firstChild());
//    } else {
//        qWarning() << "DomHelper::appendDomDocument trying to append a null doc, node " << doc->nodeName();
//        return false;
//    }
//    return true;
//}

bool DomHelper::setClassByDomData(const QDomDocument* doc, ErrorMessage* errMessage) {
    m_importingDomData=true;
    if (doc->isNull() ) {
        ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,"Document is null");
        m_importingDomData=false;
        return false;
    }
    if (!doc->isDocument()) {
        ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,"document is not a document");
        m_importingDomData=false;
        return false;
    }
    QDomNode node = doc->firstChild();
    return this->setClassByDomData(node, errMessage);
}

bool DomHelper::setClassByDomData(const QDomDocument& doc, ErrorMessage* errMessage) {
    return setClassByDomData(&doc,errMessage);
}

bool DomHelper::setClassByDomData(const QDomNode* node, ErrorMessage* errMessage) {
    m_importingDomData=true;
    if (node->isNull()) {
        ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,"DomHelper::setClassByDomData node is null");
        m_importingDomData=false;
        return false;
    }
    ErrorMessage _err;
    if (!isImportableByDomData(node,&_err)) {
        errMessage->appendMessage(_err);
        ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,"DomHelper::setClassByDomData document is not importable");
        m_importingDomData=false;
        return false;
    }
    parseEntry(node->toElement());
    m_importingDomData=false;
    this->selfObjectData();//this recreate the dom object, otherwise the dom document remains with the old data and not the new one
    return true;
}

bool DomHelper::setClassByDomData(const QDomNode& node, ErrorMessage* errMessage) {
    return setClassByDomData(&node,errMessage);
}

bool DomHelper::isImportableByDomData(const QDomNode* node, ErrorMessage* errMessage ) {
    if (node->isNull()) {
        errMessage->setMethod(Q_FUNC_INFO);
        errMessage->setMessage("The node is NULL");
        return false;
    }
    if (!node->isElement()) {
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage("The node is NOT an element");
        }
        return false;
    }
    if (!(node->toElement().tagName()==m_rootTag)) {
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(QString("RootTag not compatbile, expected <%1> found <%2>").arg(m_rootTag).arg(node->toElement().tagName()));
        }
        return false;
    }
    return true;
}

bool DomHelper::isImportableByDomData(const QDomNode& node, ErrorMessage* errMessage ) {
    return isImportableByDomData(&node,errMessage);
}

void DomHelper::parseEntry(const QDomElement &element)
{
    QDomNode node = element.firstChild();
    while (!node.isNull()) {
         //qDebug() << "DomParser::parseEntry  node " << node.toElement().tagName();
         //qDebug() << "DomParser::parseEntry  value= " << DomHelper::getNodeValue(node);
         //qDebug() << "DomHelper::parseEntry  parsing node " << node.nodeName();
         if (node.isElement()) {
         //   qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is element ";

            //Verifiy is this node has attribute. TODO verify parent child is DOMHELPER_OBJECTTYPE_TAG
            if (node.hasAttributes()) {
                Q_ASSERT(parseAndVerifyAttributeVersion(node.attributes()));
     //           qDebug () << "DomHelper::parseEntry node "<< node.nodeName()<< "  has attributes";
            }

            //Extract the actual class metaobject
            const QMetaObject* metaObject = m_hostObject->metaObject();

            //Verifying the class is correct
            if (node.nodeName()==DOMHELPER_OBJECTTYPE_TAG) {
               // qDebug() << "DomHelper::parseEntry this class "<< m_obj->metaObject()->className() <<" is elaborating the  node "<< node.nodeName()<< " with the class property " << DOMHELPER_OBJECTTYPE_TAG;
                Q_ASSERT(isSameObjectType(node.toElement()));
            }

            //Looking if some properties are stored in this node
            int _indexProp=metaObject->indexOfProperty(node.nodeName().toUtf8().constData());
 //           qDebug() << "DomHelper::parseEntry node "<< node.nodeName()<< " index of property is " << _indexProp;

            //If yes, going to set  property
            if ( _indexProp>=0 ) {
                QMetaProperty _prop=metaObject->property(_indexProp);
   //             qDebug() << "DomHelper::parseEntry node "<< node.nodeName()<< " elaborating property ("<<_indexProp<<") " << _prop.name();
                //If the node is a text node and in the previous node a properties tag was found it's time to set the property
                if (!parseAndSetProperty(node.toElement(), _prop))
                    PRINT_DEBUG_LEVEL (ErrorMessage::DEBUG_NOT_SO_IMPORTANT, ErrorMessage::DEBUG(Q_FUNC_INFO,QString("node %1 property %2  NOT SET!").arg(node.nodeName()).arg(_prop.name())) ) ;
                node = node.nextSibling();
                continue;
            }
   //         qDebug() << "DomHelper::parseEntry node "<< node.nodeName()<< "complete, looking for the next node";
            parseEntry(node.toElement());
            node = node.nextSibling();
            continue;
         }
        node = node.nextSibling();
    }
}

bool DomHelper::parseAndVerifyAttributeVersion(const QDomNamedNodeMap &element) {
    for (unsigned int n=0;n<element.length(); n++) {
        QDomNode node=element.item(n);
        Q_ASSERT(node.isAttr());
        //Check if the tag is version
  //      qDebug() << "DomHelper::parseAttributeVersion Attribute is " << node.nodeName() << " value=" << node.toAttr().nodeValue();
        if (QString::compare(DOMHELPER_VERSION_ATTRIBUTE,node.nodeName())==0) {
            bool conversionOk=false;
            uint _version=node.toAttr().nodeValue().toUInt(&conversionOk);
            if (!conversionOk) {
               // qWarning() << ErrorMessage(Q_FUNC_INFO,QString("Attribute %1 has a bad formatted  value  |%2|").arg(node.nodeName()).arg(node.toAttr().nodeValue()),ErrorMessage::WARNINGMESSAGE);
                PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,QString("Attribute %1 has a bad formatted  value  |%2|").arg(node.nodeName()).arg(node.toAttr().nodeValue())));
                return false;
            }
            if (_version!=DOMHELPER_VERSION) {
                PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,QString("INVALID VERSION %1 valid version is |%2|").arg(node.toAttr().nodeValue()).arg(DOMHELPER_VERSION)));
                return false;
            }
        }
    }
    return true;
}

bool DomHelper::isSameObjectType(const QDomElement &element) {
    bool retval =false;

    //Looking in the first node
    QDomNode node = element.firstChild();
    if (!node.isNull() && node.isText()) {
        QString _value=node.toText().data();
       // qDebug() << "DomHelper::isSameObjectType class " << m_obj->metaObject()->className() << " is elaborating property  with value " << _value;
        retval=QString::compare(_value,m_hostObject->metaObject()->className())==0 ? true : false;
      //  qDebug() << "DomHelper::isSameObjectType compare  " << _value << " and "  << m_obj->metaObject()->className() ;
    } else {
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,QString("this node is not valid to set %1  invalid node, null=%2 isText() %3")
                              .arg(m_hostObject->metaObject()->className())
                              .arg(node.isNull())
                              .arg( node.isText()))  );
    }
 //   qDebug() << "DomHelper::isSameObjectType class " << m_obj->metaObject()->className() << (retval  ? " is ": "is NOT ") << " a VALID class!";
    return retval;
}

bool DomHelper::parseAndSetProperty(const QDomElement &element, QMetaProperty &metaProperties) {
    bool retval =false;
    QString _propName=metaProperties.name();
    QDomNode node = element.firstChild();
    if (!node.isNull() && node.isText()) {
        QString _value=node.toText().data();
       // qDebug() << "DomHelper::parseAndSetProperty find node  " << node.nodeName();
        retval=metaProperties.write(m_hostObject,_value);
       // qDebug() << "DomHelper::parseAndSetProperty class " << m_obj->metaObject()->className() <<" write property " << _propName << " with value " << _value <<
       //             "property was " << (retval ? "set OK": " NOT SET");

    } else {
        PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,QString("can't elaborate property %1 invalid value").arg(_propName), ErrorMessage::WARNINGMESSAGE));
    }
    return retval;
}





