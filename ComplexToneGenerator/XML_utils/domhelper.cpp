#include "domhelper.h"

DomHelper::DomHelper(QObject * hostObj) :
    m_doc(0),
    m_obj(hostObj)
{
}

void DomHelper::generateDomDocument() {
    generateDomDocument("rootTag");
}

void DomHelper::generateDomDocument(const QString &rootTag) {
    initDomDocument(rootTag);
    qDebug() << "DomHelper::generateDomDocument start with tag rootTag=" << rootTag ;
    const QMetaObject* metaObject = m_obj->metaObject();

    for(int i = 0; i < metaObject->propertyCount(); ++i) {
        QMetaProperty _prop=metaObject->property(i);
        QString _propName=_prop.name();
        QVariant _propValueVariant=_prop.read(m_obj);

        //Save parameters that can be converted in string, others are rejected
        if (_propValueVariant.canConvert(QVariant::String)) {
            QString _propValue=_propValueVariant.toString();
   //         qDebug() << "DomHelper::generateDomDocument " << _propName << " is " << _propValue;
            //appending
            QDomElement _element = m_doc->createElement(_propName);
            QDomText _elementValue=m_doc->createTextNode(_propValue);
            m_doc->firstChild().appendChild(_element);
            _element.appendChild(_elementValue);
        } else {
            qDebug() << "DomHelper::generateDomDocument can convert property" << _propName ;
        }
    }
}

void DomHelper::initDomDocument(const QString &rootTag) {
    //Set up document
    if (m_doc!=NULL) delete m_doc;
    m_doc=new QDomDocument( (new QString("Doc_title_"))->append(m_obj->metaObject()->className()));
    QDomElement _root = m_doc->createElement(rootTag);
    m_doc->appendChild(_root);

    //Inserting class name
    QDomElement _classname = m_doc->createElement(DOMHELPER_OBJECTTYPE_TAG);
    QDomText _classnameText = m_doc->createTextNode(objectType());
    _classname.setAttribute(DOMHELPER_VERSION_TAG,DOMHELPER_VERSION);
    m_doc->firstChild().appendChild(_classname);
    _classname.appendChild(_classnameText);
}


bool DomHelper::appendDomDocument(const QDomDocument *doc) {
    if (m_doc==NULL) {
        qDebug() << "BaseClass::appendDomDocument m_doc NULL, regenerate document";
        generateDomDocument();
    }
    qDebug() << "BaseClass::appendDomDocument  processing node " << doc->nodeName();
    if (!doc->isNull()) {
        m_doc->firstChild().appendChild(doc->firstChild());
        //QTreeWidget * t=(new ReadAndWriteXML())->parseXMLToQTreeWidget(m_doc,new QTreeWidget());
        //t->setWindowTitle("DomHelper::appendDomDocument");
        //t->show();
    } else {
        qDebug() << "BaseClass::appendDomDocument trying to append a null doc, node " << doc->nodeName();
        return false;
    }
    return true;
}

//bool DomHelper::setClassByDomData(const QDomDocumentFragment & docfrag) {
//    if (docfrag.isNull() ) {
//        qWarning() << "BaseClass::setClassByDomData document fragment is invalid";
//        return false;
//    }
//    if (!docfrag.isDocument()) {
//        qWarning() << "BaseClass::setClassByDomData document fragment is not a document fragment, is " << docfrag.nodeType();
//        return false;
//    }

//    QDomNode node = docfrag.firstChild();
//    while (!node.isNull()) {
//        qDebug() << "BaseClass::setClassByDomData looking into node  " << node.nodeName();
//        parseEntry(node.toElement());
//        node = node.nextSibling();
//    }
//    return true;
//}

bool DomHelper::setClassByDomData(const QDomDocument & doc) {
    if (doc.isNull() ) {
        qWarning() << "BaseClass::setClassByDomData document is invalid";
        return false;
    }
    if (!doc.isDocument()) {
        qWarning() << "BaseClass::setClassByDomData document is not a document, is " << doc.nodeType();
        return false;
    }

    QDomNode node = doc.firstChild();
    while (!node.isNull()) {
        qDebug() << "BaseClass::setClassByDomData looking into node  " << node.nodeName();
        parseEntry(node.toElement());
        node = node.nextSibling();
    }
    return true;
}

void DomHelper::parseEntry(const QDomElement &element)
{
    QDomNode node = element.firstChild();
    while (!node.isNull()) {
        //qDebug() << "DomParser::parseEntry  node " << node.toElement().tagName();
        //qDebug() << "BaseClass::parseEntry  parsing node " << node.nodeName();
         if (node.isElement()) {
         //   qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is element ";

            //Verifiy is this node has attribute. TODO verify parent child is DOMHELPER_OBJECTTYPE_TAG
            if (node.hasAttributes()) {
                Q_ASSERT(parseAndVerifyAttributeVersion(node.attributes()));
     //           qDebug () << "BaseClass::parseEntry node "<< node.nodeName()<< "  has attributes";
            }

            //Extract the actual class metaobject
            const QMetaObject* metaObject = m_obj->metaObject();

            //Verifying the class is correct
            if (node.nodeName()==DOMHELPER_OBJECTTYPE_TAG) {
      //          qDebug() << "BaseClass::parseEntry this class "<< m_obj->metaObject()->className() <<" is elaborating the  node "<< node.nodeName()<< " with the class property " << DOMHELPER_OBJECTTYPE_TAG;
                Q_ASSERT(isSameObjectType(node.toElement()));
            }

            //Looking if some properties are stored in this node
            int _indexProp=metaObject->indexOfProperty(node.nodeName().toUtf8().constData());
 //           qDebug() << "BaseClass::parseEntry node "<< node.nodeName()<< " index of property is " << _indexProp;

            //If yes, going to set  property
            if ( _indexProp>=0 ) {
                QMetaProperty _prop=metaObject->property(_indexProp);
   //             qDebug() << "BaseClass::parseEntry node "<< node.nodeName()<< " elaborating property ("<<_indexProp<<") " << _prop.name();
                //If the node is a text node and in the previous node a properties tag was found it's time to set the property
                if (!parseAndSetProperty(node.toElement(), _prop)) qDebug() << "BaseClass::parseEntry node "<< node.nodeName()<< "property "<< _prop.name() << " NOT SET!";
                node = node.nextSibling();
                continue;
            }
   //         qDebug() << "BaseClass::parseEntry node "<< node.nodeName()<< "complete, looking for the next node";
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
        if (QString::compare(DOMHELPER_VERSION_TAG,node.nodeName())==0) {
            bool conversionOk=false;
            uint _version=node.toAttr().nodeValue().toUInt(&conversionOk);
            if (!conversionOk) {
                qWarning() << "DomHelper::parseAttributeVersion Attribute " << node.nodeName() << "has a bad formatted  value |" << node.toAttr().nodeValue()<<"|";
                return false;
            }
            if (_version!=DOMHELPER_VERSION) {
                qWarning() << "DomHelper::parseAttributeVersion INVALID VERSION! " << node.toAttr().nodeValue()<< " valid version is " << DOMHELPER_VERSION;
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
 //       qDebug() << "BaseClass::isSameObjectType class " << m_obj->metaObject()->className() << " is elaborating property  with value " << _value;
        retval=QString::compare(_value,m_obj->metaObject()->className())==0 ? true : false;
       // qDebug() << "BaseClass::isSameObjectType compare  "<< _value << " and " << objectType <<" is  " << QString::compare(_value,objectType) ;
    } else {
        qWarning() << "BaseClass::isSameObjectType  this node is not valid to set " << m_obj->metaObject()->className() << "  " <<
                    " invalid node, null=" << node.isNull() <<
                    " isText() " << node.isText();
    }
 //   qDebug() << "BaseClass::isSameObjectType class " << m_obj->metaObject()->className() << (retval  ? " is ": "is NOT ") << " a VALID class!";
    return retval;
}

bool DomHelper::parseAndSetProperty(const QDomElement &element, QMetaProperty &metaProperties) {
    bool retval =false;
    QString _propName=metaProperties.name();
    QDomNode node = element.firstChild();
    if (!node.isNull() && node.isText()) {
        QString _value=node.toText().data();
        qDebug() << "BaseClass::parseAndSetProperty find node  " << node.nodeName();
        qDebug() << "BaseClass::parseAndSetProperty class " << m_obj->metaObject()->className() <<" is elaborating property " << _propName << " with value " << _value;

        retval=metaProperties.write(m_obj,_value);
    } else {
        qWarning() << "BaseClass::parseAndSetProperty  can't elaborate property " << _propName << " with value " <<
                    " invalid node, null=" << node.isNull() <<
                    " isText=node.isText()";
    }
    return retval;
}

bool DomHelper::isSameObjectType(const QDomDocument *doc, QObject * obj) {
    if (QString::compare(obj->metaObject()->className(),DomHelper::getObjectType(doc))==0) return true;
    else return false;
}

QString DomHelper::getObjectType(const QDomDocument *doc) {
    QString retval="";
    //Get the root element
    QDomElement docElem = doc->documentElement();

    // get the node's interested in, this time only caring about the objectType
    QDomNodeList nodeList = docElem.elementsByTagName(DOMHELPER_OBJECTTYPE_TAG);

    Q_ASSERT(nodeList.length()==1);

    for (int i = 0;i < nodeList.count(); i++)
    {
            // get the current one as QDomElement
            QDomElement el = nodeList.at(i).toElement();
            //Verifying the class is correct
            if (el.nodeName()==DOMHELPER_OBJECTTYPE_TAG) {
                QDomNode node = el.firstChild();
                if (!node.isNull() && node.isText()) {
                    retval=node.toText().data();
                }
            }
    }
    return retval;
}

QString DomHelper::getObjectType(const QDomDocument & doc) {
    return DomHelper::getObjectType(&doc);
}
