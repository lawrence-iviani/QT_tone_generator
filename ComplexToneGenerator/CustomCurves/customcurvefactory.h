#ifndef CUSTOMCURVEFACTORY_H
#define CUSTOMCURVEFACTORY_H

#include <QObject>
#include <QApplication>
#include "abstractcurvesfactory.h"
#include "selectcurvewindowdialog.h"
#include "widgetstyle.h"


/**
 * @brief The CustomCurveFactory class
 * This class implemnts the Factory pattern in order
 */
class CustomCurveFactory : public QObject, public AbstractCurveFactory {
    Q_OBJECT
public:
    static CustomCurveFactory* instance();
    virtual GenericTimeData* newCurve (QString className);


     static GenericTimeData* newDialogCurve(QWidget* widget, WidgetStyle* style);
protected:
    CustomCurveFactory() {}

private:
    //THESE FUNCTION MUST BE INSERTED IN SOME KIND OF EXTERNAL UTILITY
    static void setupCurves(SelectCurveWindowHelper * selectCurveHelper);//This method creates all the possible tone generator and generate the help for the dialog window

};

#endif // CUSTOMCURVEFACTORY_H
