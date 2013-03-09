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

GenericTimeData* CustomCurveFactory::newCurve(QString className) {//, TimePlotParams* timeParams, QWidget *parent) {

    if (className=="GenericSinusData")
        return (GenericTimeData*) new GenericSinusData();//timeParams,parent);

    if (className=="PartialSinusData")
        return (GenericTimeData*) new PartialSinusData();//timeParams,parent);

    if (className=="RepeatedSinusData")
        return (GenericTimeData*) new RepeatedSinusData();//timeParams,parent);

    if (className=="GenericSquareData")
        return (GenericTimeData*) new GenericSquareData();//timeParams,parent);

    if (className=="PartialSquareData")
        return (GenericTimeData*) new PartialSquareData();//timeParams,parent);

    if (className=="RepeatedSquareData")
        return (GenericTimeData*) new RepeatedSquareData();//timeParams,parent);

    return NULL;
}

GenericTimeData* CustomCurveFactory::newDialogCurve(QWidget* widget, WidgetStyle *style) {
    SelectCurveWindowHelper* selectCurveHelper=SelectCurveWindowDialog::getDialogCurveHelper();
    CustomCurveFactory::setupCurves(selectCurveHelper);
    SelectCurveWindowDialog selectDialog(selectCurveHelper,widget);
    style->setStyle(&selectDialog);
    selectDialog.exec();

    QString curveName=selectCurveHelper->getSelectedDataCurve().name;
    delete selectCurveHelper;

    return CustomCurveFactory::instance()->newCurve(curveName);//,_tParams);
}

void CustomCurveFactory::setupCurves(SelectCurveWindowHelper * selectCurveHelper) {
    //TODO, this should be decoded with a file xml or similar

    S_DataCurve t;

    t.name="PartialSinusData";
    t.description="A tone generator with a limited duration";
    selectCurveHelper->addData(t);

    t.name="GenericSinusData";
    t.description="A tone generator";
    selectCurveHelper->addData(t);

    t.name="RepeatedSinusData";
    t.description="A pure tone repeated multiple times";
    selectCurveHelper->addData(t);

    t.name="PartialSquareData";
    t.description="A square wave generator with a limited duration";
    selectCurveHelper->addData(t);

    t.name="GenericSquareData";
    t.description="A  square wave generator";
    selectCurveHelper->addData(t);

    t.name="RepeatedSquareData";
    t.description="A  square wave repeated multiple times";
    selectCurveHelper->addData(t);
}


