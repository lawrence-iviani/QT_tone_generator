#ifndef ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_H

#include "timedata/generictimedata.h"

class AbstractCurveFactory {
public:
    virtual GenericTimeData * newCurve (QString className) {Q_UNUSED(className); return NULL;}
    virtual ~AbstractCurveFactory() {}
};

#endif // ABSTRACTFACTORY_H
