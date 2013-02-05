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
    TimePlotParams *_params=dynamic_cast< TimePlotParams*> (getDataParameters());
    Q_ASSERT(_params!=NULL);

    //Keep consitent duration and sample rate
    Q_ASSERT(connect(_params,SIGNAL(maxDurationChanged(qreal)),this,SLOT(setAllCurvesMaxDuration(qreal))));
    Q_ASSERT(connect(_params,SIGNAL(sampleRateChanged(qreal)),this,SLOT(setAllCurvesSampleRate(qreal))));

    //Signal arriving only from UI  (show/hide all)
    TimePlotWidgetUI *_ui=dynamic_cast< TimePlotWidgetUI*> (getControlWidget());
    Q_ASSERT(_ui!=NULL);
    Q_ASSERT(connect(_ui,SIGNAL(showAllCurvesToggled()),this,SLOT(showAllCurves())));
    Q_ASSERT(connect(_ui,SIGNAL(hideAllCurvesToggled()),this,SLOT(hideAllCurves())));

}

void TimePlotWidget::setAllCurvesSampleRate(qreal samplerate) {
    //Block digest update
    bool _digestPrevValue=m_digestCurve->setEnableRecalc(false);
    //Block plot update
    bool _plotPrevValue=setEnablePlot(false);

    int n=0;
    GenericTimeData * gtd=this->getTimeData(n);
    while (gtd!=NULL) {
        GenericTimeDataParams *_gtdParams=dynamic_cast< GenericTimeDataParams*> (gtd->getDataParameters());
        Q_ASSERT(_gtdParams!=NULL);
        _gtdParams->setSampleRate(samplerate);
       // gtd->sampleRateHasChanged(samplerate);//This should be automatic... but i need to force it (BUG)
        gtd=this->getTimeData(++n);
    }
    //reset everything to previous state
    setEnablePlot(_plotPrevValue);
    m_digestCurve->setEnableRecalc(_digestPrevValue);

    GenericTimeDataParams *_digestParams=dynamic_cast< GenericTimeDataParams*> (m_digestCurve->getDataParameters());
    Q_ASSERT(_digestParams!=NULL);
    _digestParams->setSampleRate(samplerate);

    //Replot and recalc digest
    this->updatePlot();
    //emit (sampleRateChanged(_SR));
}

void TimePlotWidget::setAllCurvesMaxDuration(qreal maxduration) {
    //Block digest update
    bool _digestPrevValue=m_digestCurve->setEnableRecalc(false);
    //Block plot update
    bool _plotPrevValue=setEnablePlot(false);
    int n=0;
    GenericTimeData * gtd=this->getTimeData(n);
    while (gtd!=NULL) {
        GenericTimeDataParams *_gtdParams=dynamic_cast< GenericTimeDataParams*> (gtd->getDataParameters());
        Q_ASSERT(_gtdParams!=NULL);
        _gtdParams->setMaxDuration(maxduration);
        gtd=this->getTimeData(++n);
    }
    //reset everything to previous state
    setEnablePlot(_plotPrevValue);
    m_digestCurve->setEnableRecalc(_digestPrevValue);

    //update the digest curve
    GenericTimeDataParams *_digestParams=dynamic_cast<GenericTimeDataParams*> (m_digestCurve->getDataParameters());
    Q_ASSERT(_digestParams!=NULL);
    _digestParams->setMaxDuration(maxduration);

    this->setAxisScale(xBottom, this->axisInterval(xBottom).minValue(), 1.1*maxduration);
    //Replot and recalc digest
    this->updatePlot();
  //  emit (duartionChanged(_maxDuration));
}

void TimePlotWidget::showAllCurves(bool show) {
    //Block digest update
    bool _digestPrevValue=m_digestCurve->setEnableRecalc(false);
    //Block plot update
    bool _plotPrevValue=setEnablePlot(false);
    int n=0;
    GenericTimeData * gtd=this->getTimeData(n);
    while (gtd!=NULL) {
        GenericTimeDataParams *_gtdParams=dynamic_cast< GenericTimeDataParams*> (gtd->getDataParameters());
        Q_ASSERT(_gtdParams!=NULL);
        _gtdParams->setShowCurve(show);
        gtd=this->getTimeData(++n);
    }
    //reset everything to previous state
    setEnablePlot(_plotPrevValue);
    m_digestCurve->setEnableRecalc(_digestPrevValue);
    //Replot and recalc digest
    this->updatePlot();
}

void TimePlotWidget::enableAllCurves(bool enable) {
    //Block digest update
    bool _digestPrevValue=m_digestCurve->setEnableRecalc(false);
    //Block plot update
    bool _plotPrevValue=setEnablePlot(false);
    int n=0;
    GenericTimeData * gtd=this->getTimeData(n);
    while (gtd!=NULL) {
        GenericTimeDataParams *_gtdParams=dynamic_cast<GenericTimeDataParams*> (gtd->getDataParameters());
        Q_ASSERT(_gtdParams!=NULL);
        _gtdParams->setEnableCurve(enable);
        gtd=this->getTimeData(++n);
    }
    //reset everything to previous state
    setEnablePlot(_plotPrevValue);
    m_digestCurve->setEnableRecalc(_digestPrevValue);
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
