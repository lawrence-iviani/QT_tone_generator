#ifndef DOMHELPERUTILITY_H
#define DOMHELPERUTILITY_H

#include <QDomDocument>
#include <QMetaProperty>
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

    /**
     * @brief getAttribute look for an attribute into a Map
     * @param element
     * @param attributeName
     * @return The valid attribute node if found it else a null node
     */
    static QDomNode getAttribute(const QDomNamedNodeMap &element,QString attributeName);

    static bool save(const QString namefile, const QDomDocument* doc, ErrorMessage* errMessage=NULL);
    static bool save(const QString namefile, const QDomDocument& doc, ErrorMessage* errMessage=NULL);
    static bool load(const QString namefile,  QDomDocument *doc, ErrorMessage* errMessage=NULL);
    static bool load(const QString namefile,  QDomDocument &doc, ErrorMessage *errMessage=NULL);
    /**
     * @brief createDocFromNodesList Starting from a list of node pointer create a DomDOcument
     * @param nodeList
     * @param documentType
     * @param rootTag
     * @param version
     * @return
     */
    static QDomDocument createDocFromNodesList(const QList<QDomNode*> &nodeList, const QString& documentType, const QString& rootTag , uint version );

    /**
     * @brief createDocFromNodesList Starting from a list of node pointer create a DomDOcument
     * @param nodeList
     * @param documentType
     * @param rootTag
     * @param version
     * @return
     */
    static QDomDocument createDocFromNodesList(const QList<QDomNode>& nodeList,const QString& documentType, const QString& rootTag , uint version );

    /**
     * @brief nodeListByTagName create a list of node checking for roots node with tagName from doc. Check also the document type compatiblity and the version (0 no check).
     * @param nodeList The reference to the nodeList return information
     * @param doc
     * @param documentType
     * @param tagName
     * @param version
     * @return
     */
    static bool nodeListByTagName(QDomNodeList& nodeList, const QDomDocument& doc, const QString& documentType, const QString& tagName,uint version=0,ErrorMessage * err=NULL);

    /**
     * @brief nodeListByTagName create a list of node checking for roots node with tagName from a root node. Also check the version (0 no check).
     * @param nodeList The reference to the nodeList return information
     * @param rootNode
     * @param tagName
     * @param version
     * @param err
     * @return
     */
    static bool nodeListByTagName(QDomNodeList& nodeList, const QDomNode& rootNode, const QString& tagName,uint version=0,ErrorMessage * err=NULL);

    /**
     * @brief getNodeByTagName Return a node identified with tag if only one node is found
     * @param node The reference to the node found (if any)
     * @param rootNode the root node where to lookup if a relative tag node is present.
     * @param tag
     * @param err
     * @return
     */
    static bool nodeByTagName(QDomNode& node, const QDomNode &rootNode,QString tag, ErrorMessage *err=NULL );

    /**
     * @brief nodeToString Convert the whole node and relative subnode attributes etc to a readable string
     * @param rootNode
     * @return
     */
    static QString nodeToString(const QDomNode* rootNode );

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
