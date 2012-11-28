#include "customcurvefactory.h"


CustomCurveFactory* CustomCurveFactory::instance() {
    static CustomCurveFactory* singleton = 0;
    if (singleton == 0) {
        singleton =  new CustomCurveFactory();
        singleton->setParent(qApp);
    }
    return singleton;
}

GenericTimeData* CustomCurveFactory::newObject(QString className) {
    GenericTimeData* retval = NULL;
//    if (className == "InerithedClass") {
//        retval = (InterfaceClass*) new InerithedClass(this);
//    } else if (className == "BaseClass") {
//        retval = (InterfaceClass*) new BaseClass(this);
//    } else {
//        //qDebug() << QString("Generic PropsMap created for new %1") .arg(className);
//    retval=new GenericTimeData(this);
//    }
    return retval;
}
