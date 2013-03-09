#ifndef CUSTOMCURVEFACTORY_H
#define CUSTOMCURVEFACTORY_H

#include <QObject>
#include <QApplication>
#include "abstractcurvesfactory.h"

/**
 * @brief The CustomCurveFactory class
 * This class implemnts the Factory pattern in order
 */
class CustomCurveFactory : public QObject, public AbstractCurveFactory {
    Q_OBJECT
public:
    static CustomCurveFactory* instance();
    virtual GenericTimeData * newCurve (QString className, TimePlotParams *timeParams , QWidget *parent=0);

protected:
    CustomCurveFactory() {}
};

#endif // CUSTOMCURVEFACTORY_H
