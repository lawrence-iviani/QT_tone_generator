#ifndef CUSTOMCURVEFACTORY_H
#define CUSTOMCURVEFACTORY_H

#include <QObject>
#include <QApplication>
#include "XML_utils/domhelperutility.h"
#include "abstractcurvesfactory.h"
#include "widgetstyle.h"

const static QString TIMEDATACURVEFACTORY_FILENAME=":/xml/curveslistdescription.xml";
const static QString TIMEDATACURVEFACTORY_CURVE_TAG="timedatacurve";
const static QString TIMEDATACURVEFACTORY_NAME_TAG="name";
const static QString TIMEDATACURVEFACTORY_CLASSNAME_TAG="classname";
const static QString TIMEDATACURVEFACTORY_DESCRIPTION_TAG="description";

/**
 * @brief The CustomCurveFactory class
 * This class implemnts the Factory pattern in order
 */
class CustomCurveFactory : public QObject, public AbstractCurveFactory {
    Q_OBJECT
public:
    static CustomCurveFactory* instance();
    virtual GenericTimeData* newCurve (QString className);
    virtual SelectCurveWindowHelper* getCurveHelper() {return &m_selectCurveHelper;}
    static GenericTimeData* newDialogCurve(QWidget* widget, WidgetStyle* style);

protected:
    CustomCurveFactory();

private:
    static void setupCurves(SelectCurveWindowHelper * selectCurveHelper);//This method creates all the possible tone generator and generate the help for the dialog window

    SelectCurveWindowHelper m_selectCurveHelper;
};

#endif // CUSTOMCURVEFACTORY_H
