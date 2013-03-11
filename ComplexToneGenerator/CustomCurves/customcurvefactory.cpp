#include "customcurvefactory.h"
#include "customcurvesheader.h"

CustomCurveFactory::CustomCurveFactory() {
    QDomDocument _doc;
    ErrorMessage _err;
    if (!DomHelperUtility::load(TIMEDATACURVEFACTORY_FILENAME,_doc,&_err)) {
        PRINT_WARNING(ErrorMessage::ERROR(Q_FUNC_INFO,"Error loading curves file\n\t %1").arg(_err.message()));
        exit(-1);
        //return NULL;
    }

    //DEBUG
    QDomElement _element=_doc.firstChild().toElement();
    Q_ASSERT(!_element.isNull());
    QDomNodeList _nl=_element.elementsByTagName(TIMEDATACURVEFACTORY_CURVE_TAG);
    qDebug() <<Q_FUNC_INFO<< " _element.nodeName()="<< _element.nodeName() <<" _nl.length()="<< _nl.length();
    //DEBUG

    QDomNodeList _nodesList;
    Q_ASSERT(DomHelperUtility::nodeListByTagName(_nodesList,_doc.firstChild(),TIMEDATACURVEFACTORY_CURVE_TAG,0,&_err));
    for (uint n=0; n <_nodesList.length(); n++) {
        QDomNode _node=_nodesList.at(n);
        qDebug() <<Q_FUNC_INFO<< " _node="<< DomHelperUtility::nodeToString(&_node);
        QDomNode _nodeName;
        QDomNode _nodeDescription;
        QDomNode _nodeClassName;
        Q_ASSERT(DomHelperUtility::nodeByTagName(_nodeClassName,_node,TIMEDATACURVEFACTORY_CLASSNAME_TAG));
        Q_ASSERT(DomHelperUtility::nodeByTagName(_nodeName,_node,TIMEDATACURVEFACTORY_NAME_TAG));
        Q_ASSERT(DomHelperUtility::nodeByTagName(_nodeDescription,_node,TIMEDATACURVEFACTORY_DESCRIPTION_TAG));
        s_dataCurve t;
        t.name=DomHelperUtility::getNodeValue(_nodeName);
        t.description=DomHelperUtility::getNodeValue(_nodeDescription);
        t.className=DomHelperUtility::getNodeValue(_nodeClassName);
        m_selectCurveHelper.addData(t);
    }
}

CustomCurveFactory* CustomCurveFactory::instance() {
    static CustomCurveFactory* singleton = 0;
    if (singleton == 0) {
        singleton =  new CustomCurveFactory();
        singleton->setParent(qApp);
    }
    return singleton;
}

GenericTimeData* CustomCurveFactory::newCurve(QString className) {


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
    SelectCurveWindowHelper* _curveHelper=CustomCurveFactory::instance()->getCurveHelper();
    SelectCurveWindowDialog selectDialog(_curveHelper,widget);
    style->setStyle(&selectDialog);
    selectDialog.exec();

    QString curveName=_curveHelper->getSelectedDataCurve().className;
    return CustomCurveFactory::instance()->newCurve(curveName);//,_tParams);
}

//void CustomCurveFactory::setupCurves(SelectCurveWindowHelper * selectCurveHelper) {

//    s_dataCurve t;

//    t.name="PartialSinusData";
//    t.description="A tone generator with a limited duration";
//    selectCurveHelper->addData(t);

//    t.name="GenericSinusData";
//    t.description="A tone generator";
//    selectCurveHelper->addData(t);

//    t.name="RepeatedSinusData";
//    t.description="A pure tone repeated multiple times";
//    selectCurveHelper->addData(t);

//    t.name="PartialSquareData";
//    t.description="A square wave generator with a limited duration";
//    selectCurveHelper->addData(t);

//    t.name="GenericSquareData";
//    t.description="A  square wave generator";
//    selectCurveHelper->addData(t);

//    t.name="RepeatedSquareData";
//    t.description="A  square wave repeated multiple times";
//    selectCurveHelper->addData(t);
//}


