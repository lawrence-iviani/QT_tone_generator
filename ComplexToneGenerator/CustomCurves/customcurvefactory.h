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
    virtual GenericTimeData * newCurve (QString className, TimePlotParams *timeParams , QWidget *parent=0);

protected:
    CustomCurveFactory() {}
};

#endif // CUSTOMCURVEFACTORY_H
