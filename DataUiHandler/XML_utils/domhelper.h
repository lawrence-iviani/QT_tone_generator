#ifndef DOMHELPER_H
#define DOMHELPER_H

#include <QDomDocument>
#include <QMetaProperty>
#include <QObject>
#include <QFile>
#include <QDebug>
#include <errormessage.h>
#include "domhelper_constant.h"



/**
 * @brief The DomHelper class is an helper class that provides functionalities in order to manage an XML data format rappresenting the class.
 * The main funcionality are read and store instance object and properties of the class and recall the functions when data are read from an xml format.
 */
class DomHelper
{
public:
    /**
     * @brief DomHelper init the class with the object you want to "DOMify". This instance will insert in a DOM document all the properties of the
     * base class.
     * @param hostObj
     */
    explicit DomHelper(QObject *hostDelegate,QString docType=DOMHELPER_OBJECTTYPE_TAG, QString rootTag=DOMHELPER_DEFAULT_ROOT_TAG,QString fileExtension=DOMHELPER_DEFAULT_FILE_SUFFIX);
    explicit DomHelper();
    virtual ~DomHelper();
    
    /**
     * @brief getRootNode, return a clone of the root node.
     * @return
     */
    const QDomNode getRootNode() { return (const QDomNode) m_document->firstChild().cloneNode(true); }

    const QDomDocument getDomDocument();

    /**
     * @brief setClassByDomData Set the class by importing a QDomNode, there must be compatibility in the ROOT tag of the relative XML
     * @param doc
     * @param allowUpdate by default true, this means that signals are emitted for every properties change
     * @param errMessage
     * @return
     */
    bool setClassByDomData(const QDomNode& doc, bool allowUpdate=true, ErrorMessage* errMessage=NULL);

    /**
     * @brief setClassByDomData Set the class by importing a QDomNode, there must be compatibility in the ROOT tag of the relative XML
     * @param doc
     * @param allowUpdate by default true, this means that signals are emitted for every properties change
     * @param errMessage
     * @return
     */
    bool setClassByDomData(const QDomNode* doc, bool allowUpdate=true, ErrorMessage* errMessage=NULL);

    /**
     * @brief setClassByDomData setClassByDomData Set the class by importing a QDomNode, there must be compatibility in the ROOT tag of the relative XML
     * and in the document type specified for this class
     * @param doc the reference to doc
     * @param allowUpdate by default true, this means that signals are emitted for every properties change
     * @param errMessage
     * @return
     */
    bool setClassByDomData(const QDomDocument& doc, bool allowUpdate=true, ErrorMessage* errMessage=NULL);

    /**
     * @brief setClassByDomData setClassByDomData Set the class by importing a QDomNode, there must be compatibility in the ROOT tag of the relative XML
     * and in the document type specified for this class
     * @param doc the pointer to doc
     * @param allowUpdate by default true, this means that signals are emitted for every properties change
     * @param errMessage
     * @return
     */
    bool setClassByDomData(const QDomDocument* doc, bool allowUpdate=true, ErrorMessage* errMessage=NULL);


    /**
     * @brief setClassByXMLFile
     * @param nameFile
     * @param errMessage
     * @return
     */
    bool setClassByXMLFile(QString& nameFile, ErrorMessage* errMessage=NULL);

 //   virtual bool isImportableByDomData(const QDomDocument & doc) {Q_UNUSED(doc);return true;}
 //   virtual bool isImportableByDomData(const QDomDocument * doc) {Q_UNUSED(doc);return true;}

    /**
     * @brief isImportableByDomData Check if the data is importable, this function check if the root element is valid.
     * In order to improve the importability test the member is declared virtual, thus you can write your own test.
     * @param node
     * @param err
     * @return
     */
    virtual bool isImportableByDomData(const QDomNode& node, ErrorMessage* errMessage=NULL);

    /**
     * @brief isImportableByDomData Check if the data is importable, this function check if the root element is valid.
     * In order to improve the importability test the member is declared virtual, thus you can write your own test.
     * @param node
     * @param err
     * @return
     */
    virtual bool isImportableByDomData(const QDomNode* node, ErrorMessage* errMessage=NULL);

    bool isImportingDomData() {return m_importingDomData;}
    //OLD
    bool isSameObjectType(const QString objectType) { return (QString::compare(objectType,m_hostObject->metaObject()->className())==0);}
   // QString objectType() {return m_hostObject->metaObject()->className();}

//    static bool isSameObjectType(const QDomDocument *doc, QObject *obj);

public slots:


protected:
    /**
     * @brief setHostObject If for some reason the object host change the super inherit class MUST change the reference host object, otherwise the dom helper will have a unpredictable beahvior.
     * @param obj
     */
    void setHostObject(QObject* obj);

    /**
     * @brief isSameObjectType test if the tag objectType is of the same objetType descripted in the XML
     * @param element  the element need to be verified (should be a node) if is compatible with this class.
     * @return
     */
    bool isSameObjectType(const QDomElement &element);

    /**
     * @brief DomHelper::selfObjectData extract the classname and the object properties from itself obj and stores it in DOM root element
     * every time is called the internal DOM structure is deleted and recalculated
     * @return true if the process was fine
     */
    bool selfObjectData();

private:
    /**
     * @brief m_rootNode The root node describing the properties in this object
     */
    QDomDocument *m_document;

    /**
     * @brief m_obj The host object that has to be handled with a DOM struture
     */
    QObject * m_hostObject;

    /**
     * @brief m_docTypeTag The document type generated and accepted by this class
     */
    QString m_docType;

    /**
     * @brief m_rootTag The root tag node indentify this class
     */
    QString m_rootTag;

    /**
     * @brief m_fileSuffix The suffix file when this class is exported/imported to/from file
     */
    QString m_fileSuffix;

    /**
     * @brief m_importingDomData This flag report is set to true every time this class is importing data with the appropriate methods.
     */
    bool m_importingDomData;

    void initDomDocument();
    void deleteDomDocument();

    /**
     * @brief parseEntry the generic parser, it's called by iteration to descent into node by calling itself with the following node.
     * Except the node is a class property (att1) or another recognized property (objectType)
     * @param element the generic element (a node).
     */
    void parseEntry(const QDomElement &element);

    /**
     * @brief parseAndSetProperty find and set a node if it has the correct value for the properties needed to set
     * @param element the element need to parse and set (should be a node).
     * @param metaProperties the properties that has to be set.
     * @return
     */
    bool parseAndSetProperty(const QDomElement &element, QMetaProperty &metaProperties);

    /**
     * @brief parseAndVerifyAttributeVersion verify it the attributes contain a  version attribute and validate it.
     * @param element
     * @return true if the version is compatible and if tag version is not found.
     */
    bool parseAndVerifyAttributeVersion(const QDomNamedNodeMap &element);

    /**
     * @brief removeAllChildNodes Remove all childs nodes (if any) from node
     * @param node
     */
    void removeAllChildNodes(QDomNode& node);

    /**
     * @brief removeAllChildNodes Remove all childs node from the root node
     */
    void removeAllDocumentChildNodes();

};


#endif // DOMHELPER_H
