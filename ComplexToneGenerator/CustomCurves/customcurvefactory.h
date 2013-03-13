#ifndef CUSTOMCURVEFACTORY_H
#define CUSTOMCURVEFACTORY_H

#include <QObject>
#include <QApplication>
#include "XML_utils/domhelperutility.h"
#include "abstractcurvesfactory.h"
#include "widgetstyle.h"



/**
  * This class is a data helper class to store information about available curves, it needs to be init adding data curve information,
  */
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
    virtual const QMap<QString,s_curveDescriptionData>* getCurvesMap() {return (const QMap<QString,s_curveDescriptionData>*) &m_itemsMap;}
    static GenericTimeData* newDialogCurve(QWidget* widget, WidgetStyle* style);

protected:
    CustomCurveFactory();

private:

    QMap<QString, s_curveDescriptionData> m_itemsMap;
};

#endif // CUSTOMCURVEFACTORY_H
