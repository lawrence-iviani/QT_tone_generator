#ifndef DOMHELPER_H
#define DOMHELPER_H

#include <QDomDocument>
#include <QMetaProperty>
#include <QDebug>
#include <QObject>
#include "XML_utils/readandwritexml.h"
#include "CTG_constants.h"

static const QString DOMHELPER_OBJECTTYPE_TAG="objectType";
static const QString DOMHELPER_VERSION_TAG="version";
static const QString DOMHELPER_ROOT_TAG="rootTag";

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
    explicit DomHelper(QObject *hostObj);
    
    const QDomDocument* getDomDocument() { return (const QDomDocument*) m_doc; }
    //const QDomDocumentFragment getDomDocumentFragment() { return (const QDomDocumentFragment) m_doc->createDocumentFragment();}

    bool setClassByDomData(const QDomDocument & doc);
    //bool setClassByDomData(const QDomDocumentFragment & docfrag);
    bool isSameObjectType(const QString objectType) { return (QString::compare(objectType,m_obj->metaObject()->className())==0);}
    QString objectType() {return m_obj->metaObject()->className();}

    static bool isSameObjectType(const QDomDocument *doc, QObject *obj);
    static QString getObjectType(const QDomDocument & doc);
    static QString getObjectType(const QDomDocument *doc);

protected slots:
     //void appendDomDocument(const QDomDocumentFragment & docfrag);
     bool appendDomDocument(const QDomDocument *doc);
     void generateDomDocument(const QString &rootTag);
     void generateDomDocument();

protected:
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
     * @brief isSameObjectType test if the tag objectType is of the same objetType descripted in the XML
     * @param element  the element need to be verified (should be a node) if is compatible with this class.
     * @return
     */
    bool isSameObjectType(const QDomElement &element);

    /**
     * @brief parseAndVerifyAttributeVersion verify it the attributes contain a  version attribute and validate it.
     * @param element
     * @return true if the version is compatible and if tag version is not found.
     */
    bool parseAndVerifyAttributeVersion(const QDomNamedNodeMap &element);

    /**
     * @brief m_doc The DOM document of this host object
     */
    QDomDocument * m_doc;
    /**
     * @brief m_obj The host object that has to be handled with a DOM struture
     */
    QObject * m_obj;

private:
    void initDomDocument(const QString &rootTag);

};

#endif // DOMHELPER_H
