#include "domhelperutility.h"

DomHelperUtility::DomHelperUtility()
{
}
//------------ Static members ------------//

bool DomHelperUtility::save(const QString namefile, const QDomDocument& doc, ErrorMessage* errMessage) {
    return DomHelperUtility::save(namefile, &doc,errMessage);
}

bool DomHelperUtility::save(const QString namefile, const QDomDocument * doc, ErrorMessage *errMessage) {
    bool retval=false;
    QFile _file(namefile);
    _file.open(QFile::WriteOnly);
    if (_file.isOpen()) {
        QTextStream _out(&_file);
        doc->save(_out, DomHelperUtility::defaultIndentation);
        _file.close();
        retval=true;
    } else {
        PRINT_WARNING(ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,QString("can't open file %1 for saving, error is %2")
                              .arg(namefile)
                              .arg(_file.errorString()))  );
    }
    return retval;
}

bool DomHelperUtility::load(const QString namefile,  QDomDocument &doc, ErrorMessage *errMessage) {
    return DomHelperUtility::load(namefile,&doc,errMessage);
}

bool DomHelperUtility::load(const QString namefile,  QDomDocument *doc, ErrorMessage *errMessage) {
    bool retval=false;
    if (doc==NULL) {
        PRINT_WARNING(ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,QString("document is NULL POINTER")));
        return false;
    }
    QFile _file(namefile);
    _file.open(QFile::ReadOnly);
    if (_file.isOpen()) {
        QString errorStr;
        int errorLine;
        int errorColumn;
        if (!doc->setContent(&_file, true, &errorStr, &errorLine, &errorColumn) || !_file.isOpen()) {
            PRINT_WARNING(ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,QString("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr)));
        } else retval=true;
        if (_file.isOpen()) _file.close();
    } else {
        PRINT_WARNING(ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,QString("can't open file %1 for saving, error is %2")
                              .arg(namefile)
                              .arg(_file.errorString()))  );
    }
    return retval;
}

QString DomHelperUtility::getNodeValue(const QDomNode &node) {
    if (node.isNull() ) return "";
    QDomNode nodeText = node.firstChild();
    if (!nodeText.isNull() && nodeText.isText()) {
        QString _value=nodeText.toText().data();
    //    qDebug() << "DomHelperUtility::getNodeValue find node  " << node.nodeName() << " with properties=" <<_value;
        return _value;
    } else {
        PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,QString("%1 doesn't look a valid node").arg(node.nodeName()),ErrorMessage::WARNINGMESSAGE));
        return "";
    }
}

QString DomHelperUtility::getObjectType(const QDomDocument *doc) {
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

QString DomHelperUtility::getObjectType(const QDomDocument & doc) {
    return DomHelperUtility::getObjectType(&doc);
}

//------------ Static ToQTreeWidget ------------//
bool DomHelperUtility::parseDOMToQTreeWidget(DomHelper *dh, QTreeWidget * treeWidget, ErrorMessage* errMessage) {
    if (dh==NULL) {
        ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,"NULL POINTER to DomHelperUtility object");
        return false;
    }
    if (treeWidget==NULL) {
        ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,"NULL POINTER to QTreeWidget object");
        return false;
    }
    const QDomDocument doc=dh->getDomDocument();
    return DomHelperUtility::parseDOMToQTreeWidget(&doc, treeWidget);
}

bool DomHelperUtility::parseDOMToQTreeWidget(const QDomDocument *doc, QTreeWidget * treeWidget, ErrorMessage *errMessage) {

    //VERIFYING IS A NODE
    if (doc==NULL) {
        ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,"NULL POINTER to QDomDocument object");
        return false;
    }
    if (doc->isNull()){
        ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,"QDomDocument is a null DOCUMENT");
        return false;
    }
    if (!doc->isDocument()) {
       ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,QString("QDomDocument is not a document is %1 ").arg(doc->nodeType()));
       return false;
    }

    //qDebug() << "DomHelperUtility::parseXMLToQTreeWidget  start parsing " << doc->nodeName() ;

    QDomNode node = doc->firstChild();
    return parseDOMToQTreeWidget(&node,treeWidget,errMessage);
}

bool DomHelperUtility::parseDOMToQTreeWidget(const QDomNode* rootNode , QTreeWidget * treeWidget, ErrorMessage *errMessage) {
    if (rootNode==NULL) {
        ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,"NULL POINTER to QDomNode object");
        return false;
    }
    if (rootNode->isNull()) {
        ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,"QDomNode is a null NODE");
        return false;
    }
    if (!rootNode->isElement()) {
        ErrorMessage::WARNING(errMessage,Q_FUNC_INFO,QString("QDomNode is not an element"));
        return false;
    }

    treeWidget->clear();
    QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget,QStringList(QString("Type: %1").arg(rootNode->nodeName())));
    treeWidget->addTopLevelItem(item);
    treeWidget->setColumnCount(1);
    QDomNode node=*rootNode;

    while (!node.isNull()) {
        DomHelperUtility::parseEntryToQTreeWidget(node.toElement(), item,1);
        node = node.nextSibling();
    }
    return true;
}

void DomHelperUtility::parseEntryToQTreeWidget(const QDomElement &element, QTreeWidgetItem *parent, int parentLevel)
{
    QDomNode node = element.firstChild();
    while (!node.isNull()) {
        //qDebug() << "DomParser::parseEntry  node " << node.toElement().tagName();
    //    qDebug() << "DomParser::parseEntry  parsing node " << node.nodeName();
         if (node.isElement()) {
            //qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is element ";
            QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Node %1 is an Element").arg(node.nodeName())));
            DomHelperUtility::parseEntryToQTreeWidget(node.toElement(), childitem,++parentLevel);
            QString _attr="";
            if (node.hasAttributes()) {
               childitem=new QTreeWidgetItem(childitem, QStringList(QString("Attributes node %1").arg(node.nodeName())));
               DomHelperUtility::parseAttributeToQTreeWidget(node.attributes(), childitem, parentLevel);
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
            DomHelperUtility::parseEntryToQTreeWidget(node.toElement(), childitem,++parentLevel);
            node = node.nextSibling();
            continue;
         }
         if (node.isDocumentType()) {
            //qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is  document type";
            QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Node %1 is a DocumentType").arg(node.nodeName())));
            DomHelperUtility::parseEntryToQTreeWidget(node.toElement(), childitem,++parentLevel);
            node = node.nextSibling();
            continue;
         }
         if (node.isDocumentFragment()) {
           // qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is  document fragment";
            QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Node %1 is a DocumentFragment").arg(node.nodeName())));
            DomHelperUtility::parseEntryToQTreeWidget(node.toElement(), childitem,++parentLevel);
            node = node.nextSibling();
            continue;
         }
         if (node.isEntity() || node.isEntityReference () || node.isNotation () || node.isProcessingInstruction ()) {
           //  qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is  not supported by this parser";
             node = node.nextSibling();
             continue;
         }
        node = node.nextSibling();
    }
}

void DomHelperUtility::parseAttributeToQTreeWidget(const QDomNamedNodeMap &element, QTreeWidgetItem *parent, int parentLevel) {
    for (unsigned int n=0;n<element.length(); n++) {
        QDomNode node=element.item(n);
        Q_ASSERT(node.isAttr());
        QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Attribute %1=%2").arg(node.nodeName()).arg(node.toAttr().nodeValue())));
        DomHelperUtility::parseEntryToQTreeWidget(node.toElement(), childitem,++parentLevel);
    }
}
