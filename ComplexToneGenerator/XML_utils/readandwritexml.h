#ifndef READANDWRITEXML_H
#define READANDWRITEXML_H

#include <QObject>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QDomNamedNodeMap>



class ReadAndWriteXML : public QObject
{
    Q_OBJECT
public:
    explicit ReadAndWriteXML(QObject *parent = 0);
    //void generate(QDomDocument * doc);
    void save(const QString namefile, const QDomDocument *doc);
    void load(const QString namefile, QDomDocument *doc);
    QTreeWidget* parseXMLToQTreeWidget(const QDomDocument * doc, QTreeWidget * treeWidget);
    QTreeWidget* parseXMLToQTreeWidget(const QDomDocument& doc, QTreeWidget * treeWidget);

    static const int indentation = 4;
signals:
    
public slots:

private:
    void parseEntry(const QDomElement &element, QTreeWidgetItem *parent, int parentLevel=0);
    void parseAttribute(const QDomNamedNodeMap &element, QTreeWidgetItem *parent, int parentLevel);
    QString addSpace(int space);  
};

#endif // READANDWRITEXML_H
