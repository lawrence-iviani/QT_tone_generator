#include "readandwritexml.h"

ReadAndWriteXML::ReadAndWriteXML(QObject *parent) :
    QObject(parent)
{
}

void ReadAndWriteXML::save(const QString namefile, const QDomDocument * doc) {
    QFile _file(namefile,this);
    _file.open(QFile::ReadWrite);
    QTextStream _out(&_file);
    doc->save(_out, indentation);
    _file.close();
}

void ReadAndWriteXML::load(const QString namefile,  QDomDocument *doc) {
    QFile _file(namefile,this);
    _file.open(QFile::ReadWrite);
    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!doc->setContent(&_file, true, &errorStr, &errorLine, &errorColumn) || !_file.isOpen()) {
        QMessageBox::warning(0, QObject::tr("DOM Parser"),
                             QObject::tr("Parse error at line %1, " "column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr));
    }
    if (_file.isOpen()) _file.close();
}

//QTreeWidget* ReadAndWriteXML::parseXMLToQTreeWidget(const QDomDocumentFragment& docFrag, QTreeWidget * treeWidget) {
//    QDomDocument doc=docFrag.toDocument();
//    return parseXMLToQTreeWidget((const QDomDocument*) &doc, treeWidget);
//}

QTreeWidget* ReadAndWriteXML::parseXMLToQTreeWidget(const QDomDocument &doc, QTreeWidget * treeWidget) {
    return parseXMLToQTreeWidget((const QDomDocument*) &doc, treeWidget);
}

QTreeWidget* ReadAndWriteXML::parseXMLToQTreeWidget(const QDomDocument *doc, QTreeWidget * treeWidget) {
    //VERIFYING IS A NODE
    if (doc==NULL) {
        QMessageBox::warning(0, "parseXMLToQTreeWidget","NULL POINTER");
        Q_ASSERT(FALSE);
    }
    if (doc->isNull()){
        QMessageBox::warning(0, "parseXMLToQTreeWidget","QDomDocument is null");
        Q_ASSERT(FALSE);
    }
    if (!doc->isDocument()) {
       QMessageBox::warning(0, "parseXMLToQTreeWidget",QObject::tr("QDomDocument is not a document is %1 ").arg(doc->nodeType()));
       Q_ASSERT(FALSE);
    }

    if (treeWidget==NULL) treeWidget=new QTreeWidget();
    else treeWidget->clear();
    qDebug() << "DomParser::parseXMLToQTreeWidget  start parsing " << doc->nodeName() ;

    QDomNode node = doc->firstChild();
    qDebug() << "DomParser::parseXMLToQTreeWidget  first child is " << node.nodeName() ;
    QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget,QStringList(QString("Type: %1").arg(node.nodeName())));
    treeWidget->addTopLevelItem(item);
    treeWidget->setColumnCount(1);

    while (!node.isNull()) {
        parseEntry(node.toElement(), item);
        node = node.nextSibling();
    }
    return treeWidget;
}

void ReadAndWriteXML::parseEntry(const QDomElement &element, QTreeWidgetItem *parent, int parentLevel)
{
 //   item->setText(0, element.attribute("term"));
    QDomNode node = element.firstChild();
    while (!node.isNull()) {
        //qDebug() << "DomParser::parseEntry  node " << node.toElement().tagName();
    //    qDebug() << "DomParser::parseEntry  parsing node " << node.nodeName();
         if (node.isElement()) {
            //qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is element ";
            QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Type: %1").arg(node.nodeName())));
            parseEntry(node.toElement(), childitem,++parentLevel);
            QString _attr="";
            if (node.hasAttributes()) {
               childitem=new QTreeWidgetItem(childitem, QStringList(QString("Attributes node %1").arg(node.nodeName())));
               parseAttribute(node.attributes(), childitem, parentLevel);
            }
            node = node.nextSibling();
            continue;
         }
         if (node.isCDATASection()) {
             //qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is CDATASection --" << node.toText().data() << "--";
             new QTreeWidgetItem(parent, QStringList(QString("Type: %1 Value: %2").arg(node.nodeName(),node.toText().data())));
             node = node.nextSibling();
             continue;
         }
         if (node.isComment()) {
            //qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is comment --" << node.toText().data() << "--";
            new QTreeWidgetItem(parent, QStringList(QString("Type: %1 Value: %2").arg(node.nodeName(),node.toText().data())));
            node = node.nextSibling();
            continue;
         }
         if (node.isText()) {
            //qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " text --" << node.toText().data() << "--";
            new QTreeWidgetItem(parent, QStringList(QString("Type: %1 Value: %2").arg(node.nodeName(),node.toText().data())));
            node = node.nextSibling();
            continue;
         }
         if (node.isDocument()) {
            //qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is document ";
            QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Type: %1").arg(node.nodeName())));
            parseEntry(node.toElement(), childitem,++parentLevel);
            node = node.nextSibling();
            continue;
         }
         if (node.isDocumentType()) {
            qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is  document type";
            QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Type: %1").arg(node.nodeName())));
            parseEntry(node.toElement(), childitem,++parentLevel);
            node = node.nextSibling();
            continue;
         }
         if (node.isDocumentFragment()) {
            qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is  document fragment";
            QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Type: %1").arg(node.nodeName())));
            parseEntry(node.toElement(), childitem,++parentLevel);
            node = node.nextSibling();
            continue;
         }
         if (node.isEntity() || node.isEntityReference () || node.isNotation () || node.isProcessingInstruction ()) {
             qDebug() << "DomParser::parseEntry "<< node.nodeName()<< " is  not supported";
             node = node.nextSibling();
             continue;
         }

        node = node.nextSibling();
    }
}

void ReadAndWriteXML::parseAttribute(const QDomNamedNodeMap &element, QTreeWidgetItem *parent, int parentLevel) {
    for (unsigned int n=0;n<element.length(); n++) {
        QDomNode node=element.item(n);
        Q_ASSERT(node.isAttr());
        QTreeWidgetItem *childitem=new QTreeWidgetItem(parent, QStringList(QString("Attribute: %1=%2").arg(node.nodeName()).arg(node.toAttr().nodeValue())));
        parseEntry(node.toElement(), childitem,++parentLevel);
    }
}
