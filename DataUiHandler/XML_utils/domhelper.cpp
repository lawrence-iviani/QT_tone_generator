#include "domhelper.h"


DomHelper::DomHelper(QObject *hostDelegate, QString docType, QString rootTag, uint version, QString fileExtension) :
    m_document(NULL),
    m_hostObject(hostDelegate),
    m_docType(docType),
    m_rootTag(rootTag),
    m_fileSuffix(fileExtension),
    m_version(version),
    m_importingDomData(false)
{
}

DomHelper::DomHelper() :
    m_document(NULL),
    m_hostObject(NULL),
    m_docType(DOMHELPER_DEFAULT_DOCTYPE),
    m_rootTag(DOMHELPER_DEFAULT_ROOT_TAG),
    m_fileSuffix(DOMHELPER_DEFAULT_FILE_SUFFIX),
    m_version(DOMHELPER_VERSION),
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
    _rootElement.setAttribute(DOMHELPER_VERSION_ATTRIBUTE,m_version);
    _rootElement.setAttribute(DOMHELPER_OBJECTTYPE_ATTRIBUTE,m_hostObject->metaObject()->className());
    m_document->appendChild(_rootElement);

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

    //20130221 REMOVED the delete function, it looks a little bit messy. But in this way i don't have some problem in memory leakage?
//    deleteDomDocument();
//    //Set up document
//    m_document=new  QDomDocument(m_docType);
    if (m_document) {
        deleteDomDocument();
        delete m_document;
    }
    m_document=new  QDomDocument(m_docType);
}

bool DomHelper::setClassByDomData(const QDomDocument* doc, bool allowUpdate, ErrorMessage* errMessage) {
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
    return this->setClassByDomData(node, allowUpdate, errMessage);
}

bool DomHelper::setClassByDomData(const QDomDocument& doc, bool allowUpdate, ErrorMessage* errMessage) {
    return setClassByDomData(&doc,allowUpdate,errMessage);
}

bool DomHelper::setClassByDomData(const QDomNode* node, bool allowUpdate, ErrorMessage* errMessage) {
    m_importingDomData=true;
    if (node->isNull()) {
        ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,"DomHelper::setClassByDomData node is null");
        m_importingDomData=false;
        return false;
    }
    ErrorMessage _err;
    if (!isImportableByDomData(node,&_err)) {
        if (errMessage) {
            errMessage->appendMessage(_err);
            ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,"DomHelper::setClassByDomData document is not importable");
        }
        m_importingDomData=false;
        return false;
    }
    if (!allowUpdate) {
        bool prevValue=m_hostObject->blockSignals(true);
        parseEntry(node->toElement());
        m_hostObject->blockSignals(prevValue);
    } else
        parseEntry(node->toElement());

    m_importingDomData=false;
    this->selfObjectData();//this recreate the dom object, otherwise the dom document remains with the old data and not the new one
    return true;
}

bool DomHelper::setClassByDomData(const QDomNode& node, bool allowUpdate, ErrorMessage* errMessage) {
    return setClassByDomData(&node,allowUpdate,errMessage);
}

bool DomHelper::isImportableByDomData(const QDomNode* node, ErrorMessage* errMessage ) {
    if (node->isNull()) {
        errMessage->setMethod(Q_FUNC_INFO);
        errMessage->setMessage("The RootTag is NULL");
        return false;
    }
    if (!node->isElement()) {
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage("The RootTag is NOT an element");
        }
        return false;
    }
    QDomElement _element=node->toElement();
    if (!(_element.tagName()==m_rootTag)) {
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(QString("RootTag not compatbile, expected <%1> found <%2>").arg(m_rootTag).arg(node->toElement().tagName()));
        }
        return false;
    }

    if (!_element.hasAttributes()) {
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(QString("RootTag hasn't attribute to compare"));
        }
        return false;
    }

    QDomNode _attVersion=DomHelperUtility::getAttribute(_element.attributes(),DOMHELPER_VERSION_ATTRIBUTE);
    if (_attVersion.isNull() || !isCorrectVersion(_attVersion)) {
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(QString("RootTag has an invalid attribute version"));
        }
        return false;
    }

    QDomNode _attObjectType=DomHelperUtility::getAttribute(_element.attributes(),DOMHELPER_OBJECTTYPE_ATTRIBUTE);
    if (_attObjectType.isNull() || !isCorrectObjectType(_attObjectType)) {
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(QString("RootTag is an invalid object type"));
        }
        return false;
    }
    //TODO, VERIFY ATTRIBUTE OBJECT TYPE
//    QDomNodeList _nodeList=node->toElement().elementsByTagName(DOMHELPER_OBJECTTYPE_TAG);
//    if (_nodeList.length()==0) {
//        if (errMessage) {
//            errMessage->setMethod(Q_FUNC_INFO);
//            errMessage->setMessage(QString("Not node to recognize Object type"));
//        }
//        return false;
//    }
//    if (_nodeList.length()!=1) {
//        if (errMessage) {
//            errMessage->setMethod(Q_FUNC_INFO);
//            errMessage->setMessage(QString("Too many node to recognize Object type"));
//        }
//        return false;
//    }
//    if (!isSameObjectType(_nodeList.at(0).toElement())) {
//        if (errMessage) {
//            errMessage->setMethod(Q_FUNC_INFO);
//            errMessage->setMessage(QString("Wrong node object type, expected to find %1 but found %2").
//                                   arg(m_hostObject->metaObject()->className()).
//                                   arg(DomHelperUtility::getNodeValue(_nodeList.at(0))));
//        }
//        return false;
//    }
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

            //Verifiy is this node has attribute. If yes check version and objectype
             //TODO: with this approach ONLY the ROOT element could have attributes
             //The function below check for version and object tyep, they return false only if
             //they found the attribute version or objtype and compare it. It the attribute isn't present
             //they return true
            if (node.hasAttributes()) {
                Q_ASSERT(parseAndVerifyAttributeVersion(node.attributes()));
                Q_ASSERT(parseAndVerifyAttributeObjecttype(node.attributes()));
     //           qDebug () << "DomHelper::parseEntry node "<< node.nodeName()<< "  has attributes";
            }

            //Extract the actual class metaobject
            const QMetaObject* metaObject = m_hostObject->metaObject();

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
    QDomNode _attNode=DomHelperUtility::getAttribute(element,DOMHELPER_VERSION_ATTRIBUTE);
    if (_attNode.isNull()) return true;
    return isCorrectVersion(_attNode);
}

bool DomHelper::parseAndVerifyAttributeObjecttype(const QDomNamedNodeMap &element) {
    QDomNode _attObjectType=DomHelperUtility::getAttribute(element,DOMHELPER_OBJECTTYPE_ATTRIBUTE);
    if (_attObjectType.isNull()) return true;
    return isCorrectObjectType(_attObjectType);
}

bool DomHelper::isCorrectVersion(const QDomNode& node) {
    bool conversionOk=false;
    uint _version=node.toAttr().nodeValue().toUInt(&conversionOk);
    if (!conversionOk) {
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,QString("Attribute %1 has a bad formatted  value  |%2|").arg(node.nodeName()).arg(node.toAttr().nodeValue())));
        return false;
    }
    if (_version!=m_version) {
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,QString("INVALID VERSION %1 valid version is |%2|").arg(node.toAttr().nodeValue()).arg(DOMHELPER_VERSION)));
        return false;
    }
    return true;
}

bool DomHelper::isCorrectObjectType(const QDomNode& node) {
    QString _objName=node.toAttr().nodeValue();
    if (_objName!=m_hostObject->metaObject()->className()) {
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,QString("INVALID OBJECT TYPE %1 valid expected |%2|")
                                            .arg(node.toAttr().nodeValue())
                                            .arg(m_hostObject->metaObject()->className())));
        return false;
    }
    return true;
}


//bool DomHelper::isSameObjectType(const QDomElement &element) {
//    bool retval =false;

//    //Looking in the first node
//    QDomNode node = element.firstChild();
//    if (!node.isNull() && node.isText()) {
//        QString _value=node.toText().data();
//       // qDebug() << "DomHelper::isSameObjectType class " << m_obj->metaObject()->className() << " is elaborating property  with value " << _value;
//        retval=QString::compare(_value,m_hostObject->metaObject()->className())==0 ? true : false;
//      //  qDebug() << "DomHelper::isSameObjectType compare  " << _value << " and "  << m_obj->metaObject()->className() ;
//    } else {
//        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,QString("this node is not valid to set %1  invalid node, null=%2 isText() %3")
//                              .arg(m_hostObject->metaObject()->className())
//                              .arg(node.isNull())
//                              .arg( node.isText()))  );
//    }
// //   qDebug() << "DomHelper::isSameObjectType class " << m_obj->metaObject()->className() << (retval  ? " is ": "is NOT ") << " a VALID class!";
//    return retval;
//}

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

const QDomDocument DomHelper::getDomDocument() {
//This is a trick, due to the fact that many times the DOM document is empty
 //  if (m_document->isNull() || m_document->firstChild().isNull())
    //This is a trick, due to the fact that many times the DOM document is empty, especially during init a class there some inconsitency problem,
    // this trick fix the behaviour
    selfObjectData();
    return (const QDomDocument) *m_document;
}


