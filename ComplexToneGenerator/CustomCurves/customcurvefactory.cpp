#include "customcurvefactory.h"
#include "customcurvesheader.h"

CustomCurveFactory* CustomCurveFactory::instance() {
    static CustomCurveFactory* singleton = 0;
    if (singleton == 0) {
        singleton =  new CustomCurveFactory();
        singleton->setParent(qApp);
    }
    return singleton;
}

GenericTimeData* CustomCurveFactory::newCurve(QString className, TimePlotParams* timeParams, QWidget *parent) {
   // GenericTimeData* retval = NULL;

    if (className=="GenericSinusData")
        return (GenericTimeData*) new GenericSinusData(timeParams,parent); //m_plotTime->getTimePlotParams() , m_plotTime);

    if (className=="PartialSinusData")
        return (GenericTimeData*) new PartialSinusData(timeParams,parent);

    if (className=="RepeatedSinusData")
        return (GenericTimeData*) new RepeatedSinusData(timeParams,parent);

    return NULL;
//    if (className == "InerithedClass") {
//        retval = (InterfaceClass*) new InerithedClass(this);
//    } else if (className == "BaseClass") {
//        retval = (InterfaceClass*) new BaseClass(this);
//    } else {
//        //qDebug() << QString("Generic PropsMap created for new %1") .arg(className);
//    retval=new GenericTimeData(this);
//    }
//    return retval;
}
