#ifndef ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_H

#include "../ComplexToneGenerator/timedata/generictimedata.h"

class AbstractCurveFactory {
public:
    virtual GenericTimeData * newCurve (QString className,QObject *parent=0) {Q_UNUSED(className); Q_UNUSED(parent);return NULL;}
    virtual ~AbstractCurveFactory() {}
};

#endif // ABSTRACTFACTORY_H
