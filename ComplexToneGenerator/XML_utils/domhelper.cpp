#include "domhelper.h"

DomHelper::DomHelper(QObject * hostObj) :
    m_doc(0),
    m_obj(hostObj)
{
}

DomHelper::DomHelper() :
    m_doc(0),
    m_obj(new QObject())
{
}

DomHelper::~DomHelper() {
    delete m_doc;
}

bool DomHelper::selfObjectData(QDomDocument * doc,const QString& rootTag) {
    if (doc==NULL)  return false;

    QDomElement _rootElement = doc->createElement(rootTag);
    _rootElement.setAttribute(DOMHELPER_VERSION_ATTRIBUTE,DOMHELPER_VERSION);
    doc->appendChild(_rootElement);

    Q_ASSERT(!doc->isNull());
    //Inserting class name
    QDomElement _classname = doc->createElement(DOMHELPER_OBJECTTYPE_TAG);
    QDomText _classnameText = doc->createTextNode(m_obj->metaObject()->className());
    _rootElement.appendChild(_classname);
    _classname.appendChild(_classnameText);
    qDebug() << "DomHelper::selfObjectData generate self data with TAG "<< rootTag;
 //   qDebug() << "DomHelper::selfObjectData start with tag |" << rootTag <<"|";
 //   qDebug() << "DomHelper::selfObjectData start  | "<< doc->toText().data()<< " |end";
    const QMetaObject* metaObject = m_obj->metaObject();
    for(int i = 0; i < metaObject->propertyCount(); ++i) {
        QMetaProperty _prop=metaObject->property(i);
        QString _propName=_prop.name();
        QVariant _propValueVariant=_prop.read(m_obj);
        //qDebug() << "DomHelper::generateDomDocument class "<< metaObject->className()<< " prop is " << _propName;
        //Save parameters that can be converted in string, others are rejected
        if (_propValueVariant.canConvert(QVariant::String)) {
            QString _propValue=_propValueVariant.toString();
            qDebug() << "DomHelper::selfObjectData appending " << _propName << " with value " << _propValue;
            //appending
            QDomElement _element = doc->createElement(_propName);
            QDomText _elementValue=doc->createTextNode(_propValue);
            _rootElement.appendChild(_element);
            _element.appendChild(_elementValue);
         //   qDebug() << "DomHelper::selfObjectData start  | "<< doc->toText().data()<< " |end";
        } else {
            qWarning() << "DomHelper::selfObjectData can convert property" << _propName ;
        }
    }
    return true;
}

void DomHelper::initDomDocument(const QString &docTypeTag, const QString &rootTag) {
    //Set up document
    if (m_doc!=NULL) delete m_doc;
    m_doc=new QDomDocument(docTypeTag);//(new QString(rootTag))->append("_").append(m_obj->metaObject()->className()));
    QDomElement _root=m_doc->createElement(rootTag);
    m_doc->appendChild(_root);
}


void DomHelper::initDomDocument(const QString &rootTag) {
    //Set up document
    if (m_doc!=NULL) delete m_doc;
    m_doc=new QDomDocument( (new QString(rootTag))->append("_").append(m_obj->metaObject()->className()));
    QDomElement _root=m_doc->createElement(rootTag);
    m_doc->appendChild(_root);
}

bool DomHelper::appendDomDocument(const QDomDocument& doc) {
    return this->appendDomDocument(&doc);
}

bool DomHelper::appendDomDocument(const QDomDocument *doc) {
    if (m_doc==NULL) {
    //    qDebug() << "DomHelper::appendDomDocument m_doc NULL, regenerate document";
    //    generateDomDocument();
        qWarning() << "DomHelper::appendDomDocument can't append to a NULL document";
        return false;
    }
  //  qDebug() << "DomHelper::appendDomDocument  processing node " << doc->nodeName();
    if (!doc->isNull()) {
        m_doc->firstChild().appendChild(doc->firstChild());
    } else {
        qWarning() << "DomHelper::appendDomDocument trying to append a null doc, node " << doc->nodeName();
        return false;
    }
    return true;
}

bool DomHelper::setClassByDomData(const QDomDocument & doc) {
    const QDomDocument * pDoc=&doc;
    return this->setClassByDomData(pDoc);
}

bool DomHelper::setClassByDomData(const QDomDocument * doc) {
    if (doc->isNull() ) {
        qWarning() << "DomHelper::setClassByDomData document is null";
        QMessageBox::warning(0, "DomHelper::setClassByDomData","Document is null");
        return false;
    }
    if (!doc->isDocument()) {
        qWarning() << "DomHelper::setClassByDomData document is not a document, is " << doc->nodeType();
        QMessageBox::warning(0, "DomHelper::setClassByDomData",(new QString("Document is not a document is %1"))->arg(doc->nodeType()));
        return false;
    }
    QDomNode node = doc->firstChild();
    return this->setClassByDomData(node);
}

bool DomHelper::setClassByDomData(QDomNode& node) {
    if (node.isNull()) {
        qWarning() << "DomHelper::setClassByDomData node is null";
        QMessageBox::warning(0, "DomHelper::setClassByDomData","Node is null");
        return false;
    }
    if (!isImportableByDomData(node)) {
        qWarning() << "DomHelper::setClassByDomData document is not importable";
        QMessageBox::warning(0, "DomHelper::setClassByDomData","Document is not importable");
        return false;
    }
    parseEntry(node.toElement());
    return true;
}

void DomHelper::parseEntry(const QDomElement &element)
{
    QDomNode node = element.firstChild();
    while (!node.isNull()) {
         qDebug() << "DomParser::parseEntry  node " << node.toElement().tagName();
         qDebug() << "DomParser::parseEntry  value= " << DomHelper::getNodeValue(node);
         qDebug() << "DomHelper::parseEntry  parsing node " << node.nodeName();
         if (node.isElement()) {
         //   qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is element ";

            //Verifiy is this node has attribute. TODO verify parent child is DOMHELPER_OBJECTTYPE_TAG
            if (node.hasAttributes()) {
                Q_ASSERT(parseAndVerifyAttributeVersion(node.attributes()));
     //           qDebug () << "DomHelper::parseEntry node "<< node.nodeName()<< "  has attributes";
            }

            //Extract the actual class metaobject
            const QMetaObject* metaObject = m_obj->metaObject();

            //Verifying the class is correct
            if (node.nodeName()==DOMHELPER_OBJECTTYPE_TAG) {
                qDebug() << "DomHelper::parseEntry this class "<< m_obj->metaObject()->className() <<" is elaborating the  node "<< node.nodeName()<< " with the class property " << DOMHELPER_OBJECTTYPE_TAG;
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
                if (!parseAndSetProperty(node.toElement(), _prop)) qDebug() << "DomHelper::parseEntry node "<< node.nodeName()<< "property "<< _prop.name() << " NOT SET!";
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
        qDebug() << "DomHelper::isSameObjectType class " << m_obj->metaObject()->className() << " is elaborating property  with value " << _value;
        retval=QString::compare(_value,m_obj->metaObject()->className())==0 ? true : false;
      //  qDebug() << "DomHelper::isSameObjectType compare  " << _value << " and "  << m_obj->metaObject()->className() ;
    } else {
        qWarning() << "DomHelper::isSameObjectType  this node is not valid to set " << m_obj->metaObject()->className() << "  " <<
                    " invalid node, null=" << node.isNull() <<
                    " isText() " << node.isText();
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
        retval=metaProperties.write(m_obj,_value);
        qDebug() << "DomHelper::parseAndSetProperty class " << m_obj->metaObject()->className() <<" write property " << _propName << " with value " << _value <<
                    "property was " << (retval ? "set OK": " NOT SET");

    } else {
        qWarning() << "DomHelper::parseAndSetProperty  can't elaborate property " << _propName << " with value " <<
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

bool DomHelper::parseDOMToQTreeWidget( DomHelper *dh, QTreeWidget * treeWidget) {
    if (dh==NULL) {
        QMessageBox::warning(0, "DomHelper::parseXMLToQTreeWidget","NULL POINTER to DomHelper object");
        return false;
    }
    if (treeWidget==NULL) {
        QMessageBox::warning(0, "DomHelper::parseXMLToQTreeWidget","NULL POINTER to QTreeWidget object");
        return false;
    }
    const QDomDocument * doc=dh->getDomDocument();
    return DomHelper::parseDOMToQTreeWidget(doc, treeWidget);
}

bool DomHelper::parseDOMToQTreeWidget(const QDomDocument *doc, QTreeWidget * treeWidget) {

    //VERIFYING IS A NODE
    if (doc==NULL) {
        QMessageBox::warning(0, "DomHelper::parseXMLToQTreeWidget","NULL POINTER to QDomDocument object");
        return false;
    }
    if (doc->isNull()){
        QMessageBox::warning(0, "DomHelper::parseXMLToQTreeWidget","QDomDocument is null");
        return false;
    }
    if (!doc->isDocument()) {
       QMessageBox::warning(0, "DomHelper::parseXMLToQTreeWidget",QObject::tr("QDomDocument is not a document is %1 ").arg(doc->nodeType()));
       return false;
    }

    treeWidget->clear();
    qDebug() << "DomHelper::parseXMLToQTreeWidget  start parsing " << doc->nodeName() ;

    QDomNode node = doc->firstChild();
    //qDebug() << "DomHelper::parseXMLToQTreeWidget  first child is " << node.nodeName() ;
    QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget,QStringList(QString("Type: %1").arg(node.nodeName())));
    treeWidget->addTopLevelItem(item);
    treeWidget->setColumnCount(1);

    while (!node.isNull()) {
        DomHelper::parseEntryToQTreeWidget(node.toElement(), item,1);
        node = node.nextSibling();
    }
    return true;
}

bool DomHelper::save(const QString namefile, const QDomDocument& doc) {
    return DomHelper::save(namefile, &doc);
}

bool DomHelper::save(const QString namefile, const QDomDocument * doc) {
    bool retval=false;
    QFile _file(namefile);
    _file.open(QFile::WriteOnly);
    if (_file.isOpen()) {
        QTextStream _out(&_file);
        doc->save(_out, DomHelper::defaultIndentation);
        _file.close();
        retval=true;
    }
    return retval;
}

bool DomHelper::load(const QString namefile,  QDomDocument *doc) {
    bool retval=false;
    QFile _file(namefile);
    _file.open(QFile::ReadOnly);
    if (_file.isOpen()) {
        QString errorStr;
        int errorLine;
        int errorColumn;
        if (!doc->setContent(&_file, true, &errorStr, &errorLine, &errorColumn) || !_file.isOpen()) {
            QMessageBox::warning(0, QObject::tr("DOM Parser"),
                                 QObject::tr("Parse error at line %1, " "column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr));
        } else retval=true;
        if (_file.isOpen()) _file.close();
    }
    return retval;
}

void DomHelper::parseEntryToQTreeWidget(const QDomElement &element, QTreeWidgetItem *parent, int parentLevel)
{
    QDomNode node = element.firstChild();
    while (!node.isNull()) {
        //qDebug() << "DomParser::parseEntry  node " << node.toElement().tagName();
    //    qDebug() << "DomParser::parseEntry  parsing node " << node.nodeName();
         if (node.isElement()) {
            //qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is element ";
            QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Node %1 is an Element").arg(node.nodeName())));
            DomHelper::parseEntryToQTreeWidget(node.toElement(), childitem,++parentLevel);
            QString _attr="";
            if (node.hasAttributes()) {
               childitem=new QTreeWidgetItem(childitem, QStringList(QString("Attributes node %1").arg(node.nodeName())));
               DomHelper::parseAttributeToQTreeWidget(node.attributes(), childitem, parentLevel);
            }
            node = node.nextSibling();
            continue;
         }
         if (node.isCDATASection()) {
             //qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is CDATASection --" << node.toText().data() << "--";
             new QTreeWidgetItem(parent, QStringList(QString("Node %1 is a CDATA, Value= |%2|").arg(node.nodeName(),node.toText().data())));
             node = node.nextSibling();
             continue;
         }
         if (node.isComment()) {
            //qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is comment --" << node.toText().data() << "--";
            new QTreeWidgetItem(parent, QStringList(QString("Node %1 is a Comment, Value= |%2|").arg(node.nodeName(),node.toText().data())));
            node = node.nextSibling();
            continue;
         }
         if (node.isText()) {
            //qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " text --" << node.toText().data() << "--";
            new QTreeWidgetItem(parent, QStringList(QString("Node %1 is a Text, Value= |%2|").arg(node.nodeName(),node.toText().data())));
            node = node.nextSibling();
            continue;
         }
         if (node.isDocument()) {
            //qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is document ";
            QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Node %1 is a Document").arg(node.nodeName())));
            DomHelper::parseEntryToQTreeWidget(node.toElement(), childitem,++parentLevel);
            node = node.nextSibling();
            continue;
         }
         if (node.isDocumentType()) {
            qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is  document type";
            QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Node %1 is a DocumentType").arg(node.nodeName())));
            DomHelper::parseEntryToQTreeWidget(node.toElement(), childitem,++parentLevel);
            node = node.nextSibling();
            continue;
         }
         if (node.isDocumentFragment()) {
            qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is  document fragment";
            QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Node %1 is a DocumentFragment").arg(node.nodeName())));
            DomHelper::parseEntryToQTreeWidget(node.toElement(), childitem,++parentLevel);
            node = node.nextSibling();
            continue;
         }
         if (node.isEntity() || node.isEntityReference () || node.isNotation () || node.isProcessingInstruction ()) {
             qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is  not supported by this parser";
             node = node.nextSibling();
             continue;
         }
        node = node.nextSibling();
    }
}

void DomHelper::parseAttributeToQTreeWidget(const QDomNamedNodeMap &element, QTreeWidgetItem *parent, int parentLevel) {
    for (unsigned int n=0;n<element.length(); n++) {
        QDomNode node=element.item(n);
        Q_ASSERT(node.isAttr());
        QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Attribute %1=%2").arg(node.nodeName()).arg(node.toAttr().nodeValue())));
        DomHelper::parseEntryToQTreeWidget(node.toElement(), childitem,++parentLevel);
    }
}

QString DomHelper::getNodeValue(const QDomNode &node) {
    if (node.isNull() ) return "";
    QDomNode nodeText = node.firstChild();
    if (!nodeText.isNull() && nodeText.isText()) {
        QString _value=nodeText.toText().data();
    //    qDebug() << "DomHelper::getNodeValue find node  " << node.nodeName() << " with properties=" <<_value;
        return _value;
    } else {
        qWarning() << "DomHelper::getNodeValue " << node.nodeName() << " doesn't look a valid node ";
        return "";
    }
}
