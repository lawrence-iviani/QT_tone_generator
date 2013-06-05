#include "customcurvefactory.h"
#include "customcurvesheader.h"
 #include <QMetaType>

CustomCurveFactory::CustomCurveFactory() {
    QDomDocument _doc;
    ErrorMessage _err;
    if (!DomHelperUtility::load(TIMEDATACURVEFACTORY_FILENAME,_doc,&_err)) {
        PRINT_WARNING(ErrorMessage::ERROR(Q_FUNC_INFO,"Error loading curves file\n\t %1").arg(_err.message()));
        exit(-1);
        //return NULL;
    }

    QDomNodeList _nodesList;
    Q_ASSERT(DomHelperUtility::nodeListByTagName(_nodesList,_doc.firstChild(),TIMEDATACURVEFACTORY_CURVE_TAG,0,&_err));
    for (uint n=0; n <_nodesList.length(); n++) {
        QDomNode _node=_nodesList.at(n);
        QDomNode _nodeName;
        QDomNode _nodeDescription;
        QDomNode _nodeClassName;
        Q_ASSERT(DomHelperUtility::nodeByTagName(_nodeClassName,_node,TIMEDATACURVEFACTORY_CLASSNAME_TAG));
        Q_ASSERT(DomHelperUtility::nodeByTagName(_nodeName,_node,TIMEDATACURVEFACTORY_NAME_TAG));
        Q_ASSERT(DomHelperUtility::nodeByTagName(_nodeDescription,_node,TIMEDATACURVEFACTORY_DESCRIPTION_TAG));
        s_curveDescriptionData t;
        t.name=DomHelperUtility::getNodeValue(_nodeName);
        t.description=DomHelperUtility::getNodeValue(_nodeDescription);
        t.className=DomHelperUtility::getNodeValue(_nodeClassName);
        m_itemsMap.insert(t.name,t);
        qDebug() << "Registered metatype "<<t.className <<" with id " << qRegisterMetaType(t.className.toAscii().data());
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
    //SIN DATA
    if (className=="GenericSinusData")
        return (GenericTimeData*) new GenericSinusData();//timeParams,parent);
    if (className=="PartialSinusData")
        return (GenericTimeData*) new PartialSinusData();//timeParams,parent);
    if (className=="RepeatedSinusData")
        return (GenericTimeData*) new RepeatedSinusData();//timeParams,parent);

    //SQUARE DATA
    if (className=="GenericSquareData")
        return (GenericTimeData*) new GenericSquareData();//timeParams,parent);
    if (className=="PartialSquareData")
        return (GenericTimeData*) new PartialSquareData();//timeParams,parent);
    if (className=="RepeatedSquareData")
        return (GenericTimeData*) new RepeatedSquareData();//timeParams,parent);

    //PINK NOISE DATA
    if (className=="GenericPinkNoiseData")
        return (GenericTimeData*) new GenericPinkNoiseData();
    if (className=="PartialPinkNoiseData")
        return (GenericTimeData*) new PartialPinkNoiseData();
    if (className=="RepeatedPinkNoiseData")
        return (GenericTimeData*) new RepeatedPinkNoiseData();

    return NULL;
}

GenericTimeData* CustomCurveFactory::newDialogCurve(QWidget* widget, WidgetStyle *style) {
    const QMap<QString,s_curveDescriptionData>* _map=CustomCurveFactory::instance()->getCurvesMap();
    SelectCurveWindowDialog selectDialog(_map,widget);
    style->setStyle(&selectDialog);
    selectDialog.exec();

    QString curveName=selectDialog.getSelectedCurveName();
    QMap<QString, s_curveDescriptionData>::const_iterator i=_map->find(curveName);
    if (i==_map->end()) return NULL;
    s_curveDescriptionData _s=i.value();
    return CustomCurveFactory::instance()->newCurve(_s.className);//,_tParams);
}




