#include "timeplotwidget.h"

TimePlotWidget::TimePlotWidget(QWidget *parent, int xScaleType, int yScaleType) :
    PlotWidget(parent,  xScaleType,  yScaleType)
{
    m_timePlotDelegate=new DataUiHandlerDelegate(
                dynamic_cast<DataUiHandlerProperty*>(new TimePlotParams((QObject*)parent)),
                dynamic_cast<DataUiHandlerUI*>(new TimePlotWidgetUI(new ZMP_Handler(this->canvas()))),
                (QObject*)parent);

    TimePlotParams *_params=dynamic_cast< TimePlotParams*> (getDataParameters());
    Q_ASSERT(_params!=NULL);
    _params->setMaxDuration(TIMEDATA_DEFAULT_PROJECT_TIME);
    _params->setSampleRate(TIMEDATA_DEFAULT_SR);
    m_digestCurve=new DigestTimeData(&m_curveList,_params);
    m_digestCurve->getCurve()->attach(this);
    this->setRubberBandPosition(0);
    //connect show all and enable all
    connectSignals();
}

TimePlotWidget::~TimePlotWidget() { }

void TimePlotWidget::connectSignals() {
//    TimePlotParams * tpp=(TimePlotParams*) getTimePlotParams();
//    TimePlotWidgetUI * tpUI=(TimePlotWidgetUI*) getControlWidget();
//    //Connect sample rate change
//    connect(tpp,SIGNAL(sampleRateChanged(qreal)),this,SLOT(setAllCurvesSampleRate(qreal)));
//    //Connect max duration change
//    connect(tpp,SIGNAL(maxDurationChanged(qreal)),this,SLOT(setAllCurvesMaxDuration(qreal)));
//    //Connect show all
//    connect(tpUI,SIGNAL(showAllCurvesToggled()),this,SLOT(showAllCurves()));
//    //Connect hide all
//    connect(tpUI,SIGNAL(hideAllCurvesToggled()),this,SLOT(hideAllCurves()));
}

bool TimePlotWidget::setEnableUpdate(bool enable) {
    bool retval=PlotWidget::setEnableUpdate(enable);
    if (enable!=m_enableUpdate) {
        enable ? m_digestCurve->enableUpdate() : m_digestCurve->inihbitUpdate();
    }
    return retval;
}

void TimePlotWidget::forceRecreateAll() {
    if (m_enableUpdate) {
        PlotWidget::forceRecreateAll();
        m_digestCurve->createData();
    }
}

void TimePlotWidget::forceUpdateAll() {
    if (m_enableUpdate) {
        PlotWidget::forceUpdateAll();
        m_digestCurve->createData(); 
    }
}

void TimePlotWidget::setAllCurvesSampleRate(qreal samplerate) {
    int n=0;
    GenericTimeData * gtd=this->getTimeData(n);

    //Block digest, i don't want recalc digest before all the curves was updated
    bool sigStatus=m_digestCurve->setEnableRecalc(false);// setInhibitRecalc(true);
    while (gtd!=NULL) {
        GenericTimeDataParams *_gtdParams=dynamic_cast< GenericTimeDataParams*> (gtd->getDataParameters());
        Q_ASSERT(_gtdParams!=NULL);
        _gtdParams->setSampleRate(samplerate);
       // gtd->sampleRateHasChanged(samplerate);//This should be automatic... but i need to force it (BUG)
        gtd=this->getTimeData(++n);
    }
    //update the digest curve
    m_digestCurve->setEnableRecalc(sigStatus);
    //this call should update everything
    GenericTimeDataParams *_digestParams=dynamic_cast< GenericTimeDataParams*> (m_digestCurve->getDataParameters());
    Q_ASSERT(_digestParams!=NULL);
    _digestParams->setSampleRate(samplerate);

    //Replot and recalc digest
    this->updatePlot();
    //emit (sampleRateChanged(_SR));
}

void TimePlotWidget::setAllCurvesMaxDuration(qreal maxduration) {
    int n=0;
    GenericTimeData * gtd=this->getTimeData(n);

    //Block digest, i don't want recalc digest before all the curves was updated
    bool sigStatus=m_digestCurve->setEnableRecalc(false);//setInhibitRecalc(true);
    while (gtd!=NULL) {
        GenericTimeDataParams *_gtdParams=dynamic_cast< GenericTimeDataParams*> (gtd->getDataParameters());
        Q_ASSERT(_gtdParams!=NULL);
        _gtdParams->setMaxDuration(maxduration);
        gtd=this->getTimeData(++n);
    }
    m_digestCurve->setEnableRecalc(sigStatus);

    //update the digest curve
    GenericTimeDataParams *_digestParams=dynamic_cast<GenericTimeDataParams*> (m_digestCurve->getDataParameters());
    Q_ASSERT(_digestParams!=NULL);
    _digestParams->setMaxDuration(maxduration);

    //Update UI

    this->setAxisScale(xBottom, this->axisInterval(xBottom).minValue(), 1.1*maxduration);
    //Replot and recalc digest
    this->updatePlot();
  //  emit (duartionChanged(_maxDuration));
}

void TimePlotWidget::showAllCurves(bool show) {
    bool sigStatus=m_digestCurve->setEnableRecalc(false);//setInhibitRecalc(true);
    int n=0;
    GenericTimeData * gtd=this->getTimeData(n);
    while (gtd!=NULL) {
        GenericTimeDataParams *_gtdParams=dynamic_cast< GenericTimeDataParams*> (gtd->getDataParameters());
        Q_ASSERT(_gtdParams!=NULL);
        _gtdParams->setShowCurve(show);
        gtd=this->getTimeData(++n);
    }
    m_digestCurve->setEnableRecalc(sigStatus);
    //Replot and recalc digest
    this->updatePlot();
}

void TimePlotWidget::enableAllCurves(bool enable) {
    bool sigStatus=m_digestCurve->setEnableRecalc(false);//setInhibitRecalc(true);
    int n=0;
    GenericTimeData * gtd=this->getTimeData(n);
    while (gtd!=NULL) {
        GenericTimeDataParams *_gtdParams=dynamic_cast<GenericTimeDataParams*> (gtd->getDataParameters());
        Q_ASSERT(_gtdParams!=NULL);
        _gtdParams->setEnableCurve(enable);
        gtd=this->getTimeData(++n);
    }
    m_digestCurve->setEnableRecalc(sigStatus);
    m_digestCurve->updateData();
}

void TimePlotWidget::setRubberBandPosition(qreal position) {
    m_scrollRubberBand->setValue(position);
}

//const  QDomDocument* TimePlotWidget::getTimePlotParametersDomDocument() {
//    if ( m_params.getDomDocument()->isNull() || m_params.getDomDocument()->firstChild().isNull()  ) {
//        qWarning() << "TimePlotWidget::getTimePlotParametersDomDocument  FORCE REGENERATE!!!";
//        m_params.regenerateDomDocument();
//    }
//   // qDebug() << "TimePlotWidget::getTimePlotParametersDomDocument \n" << m_params.getDomDocument()->toString(2);
//    return (const QDomDocument *) m_params.getDomDocument();
//    return NULL;
//}
