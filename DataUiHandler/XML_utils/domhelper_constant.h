#ifndef DOMHELPER_CONSTANT_H
#define DOMHELPER_CONSTANT_H

class QString;

//The version of the generated XML data structure
const uint static DOMHELPER_VERSION=1;

//TAG for XML data
const static QString DOMHELPER_DEFAULT_ROOT_TAG="DOMHelperDefaultParams";
const static QString DOMHELPER_OBJECTTYPE_TAG="ObjectType";
const static QString DOMHELPER_VERSION_ATTRIBUTE="Version";

//DOCTYPE
const static QString DOMHELPER_DEFAULT_DOCTYPE="DOMHelperDefaultDocument";

//FILE DESCRIPTORS
const static QString DOMHELPER_DEFAULT_FILE_SUFFIX=".xml";
const static QString DOMHELPER_DEFAULT_FILE_DESCRIPTOR="Generic DOM Helper file XML";




#endif // DOMHELPER_CONSTANT_H
