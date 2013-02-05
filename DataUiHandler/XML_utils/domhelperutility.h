#ifndef DOMHELPERUTILITY_H
#define DOMHELPERUTILITY_H

#include <QDomDocument>
#include <QMetaProperty>
//#include <QDebug>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <errormessage.h>
#include <QString>
#include "domhelper.h"

class DomHelper;

class DomHelperUtility
{
public:
    DomHelperUtility();

    static QString getObjectType(const QDomDocument & doc);
    static QString getObjectType(const QDomDocument *doc);

    static bool save(const QString namefile, const QDomDocument* doc, ErrorMessage* errMessage=NULL);
    static bool save(const QString namefile, const QDomDocument& doc, ErrorMessage* errMessage=NULL);
    static bool load(const QString namefile,  QDomDocument *doc, ErrorMessage* errMessage=NULL);
    static bool load(const QString namefile,  QDomDocument &doc, ErrorMessage *errMessage);
    static bool parseDOMToQTreeWidget(const QDomDocument *doc, QTreeWidget * treeWidget, ErrorMessage* errMessage=NULL);
    static bool parseDOMToQTreeWidget(const QDomNode* rootNode , QTreeWidget * treeWidget, ErrorMessage *errMessage=NULL);
    static bool parseDOMToQTreeWidget(DomHelper *dh, QTreeWidget * treeWidget,  ErrorMessage* errMessage=NULL);
    static void parseEntryToQTreeWidget(const QDomElement &element, QTreeWidgetItem *parent, int parentLevel);
    static void parseAttributeToQTreeWidget(const QDomNamedNodeMap &element, QTreeWidgetItem *parent, int parentLevel);

    /**
     * @brief getNodeValue return the value stored in node element if is a valid and text element.
     * @param element
     * @return the string value of the stored value
     */
    static QString getNodeValue(const QDomNode &node);

    static const int defaultIndentation = 4;

};

#endif // DOMHELPERUTILITY_H
