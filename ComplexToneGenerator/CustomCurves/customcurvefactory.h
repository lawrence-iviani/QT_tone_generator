#ifndef CUSTOMCURVEFACTORY_H
#define CUSTOMCURVEFACTORY_H

#include <QObject>
#include <QApplication>
#include "abstractcurvesfactory.h"
#include "timedata/generictimedata.h"

class CustomCurveFactory : public QObject, public AbstractCurveFactory {
    Q_OBJECT
public:
    static CustomCurveFactory* instance();
    virtual GenericTimeData * newObject (QString className);

protected:
    CustomCurveFactory() {}
};

#endif // CUSTOMCURVEFACTORY_H
