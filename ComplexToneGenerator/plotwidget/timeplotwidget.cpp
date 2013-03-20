#include "timeplotwidget.h"

TimePlotWidget::TimePlotWidget(QWidget *parent, int xScaleType, int yScaleType) :
    PlotWidget(parent,  xScaleType,  yScaleType),
    m_freqPlot(NULL),
    m_enablePlotUpdate(true)
{
    m_timePlotDelegate=new DataUiHandlerDelegate(
                dynamic_cast<DataUiHandlerProperty*>(new TimePlotParams((QObject*)parent)),
                dynamic_cast<DataUiHandlerUI*>(new TimePlotWidgetUI(new ZMP_Handler(this->canvas()))),
                PROJECTTIMEPARAMS_DOCTYPE,
                PROJECTTIMEPARAMETERS_TAG,
                PROJECTTIMEPARAMS_DOCVERSION,
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

    //set title
    this->setPlotTitle("Time curves plot");
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
    Q_ASSERT(connect(this,SIGNAL(curveListChanged()),this,SLOT(recalcAndUpdatePlot())));
}

int TimePlotWidget::addTimeData(GenericTimeData * gtd) {
    if (!gtd) return -1;
    m_curveList.append(gtd);
    gtd->getCurve()->setZ(m_curveList.length());
    gtd->getCurve()->attach(this);
    Q_ASSERT(connect(gtd,SIGNAL(dataChanged()),this,SLOT(recalcAndUpdatePlot())));
    Q_ASSERT(connect(gtd,SIGNAL(curveAttributeChanged()),this,SLOT(updatePlot())));
    emit curveListChanged();
    return (m_curveList.length()-1);
}

bool TimePlotWidget::removeTimeData(int index) {
    bool retval=false;
    if (  (0 <= index) && (index < m_curveList.length()) ) {
        GenericTimeData *  gtd=this->getTimeData(index);
        gtd->getCurve()->detach();
        disconnect(gtd,SIGNAL(dataChanged()),this,SLOT(recalcAndUpdatePlot()));
        disconnect(gtd,SIGNAL(curveAttributeChanged()),this,SLOT(updatePlot()));
        Q_ASSERT(gtd!=NULL);
        delete gtd;
        m_curveList.removeAt(index);
        retval=true;
        emit curveListChanged();
    } else {
        qWarning() << "PlotWidget::removeTimeData trying remove curve index "<< index <<", is out of range " << " lenCurveList=" << m_curveList.length();
    }
    return retval;
}

GenericTimeData *TimePlotWidget::getTimeData(int index) {
    GenericTimeData * retval=NULL;
    if (  (0 <= index) && (index < m_curveList.length()) ) {
        retval=m_curveList.at(index);
    }
    return retval;
}

bool TimePlotWidget::setEnablePlot(bool enable) {
    bool retval=m_enablePlotUpdate;
    if (enable!=m_enablePlotUpdate) {
        m_enablePlotUpdate=enable;
    }
    return retval;
}

QStringList  TimePlotWidget::getTimeDataStringList() {
    QStringList retval;
    foreach(GenericTimeData* p, m_curveList) {
        GenericTimeDataParams* _params=dynamic_cast<GenericTimeDataParams*> (p->getDataParameters());
        retval << _params->name();
    }
    return retval;
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
    if (m_freqPlot) m_freqPlot->dataUpdated();
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
    if (m_freqPlot) m_freqPlot->dataUpdated();
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

void TimePlotWidget::setFreqWidget(FreqPlotWidget* freqPlotWidget) {
    if (!freqPlotWidget) return;
    m_freqPlot=freqPlotWidget;
    m_freqPlot->setTimeData(m_digestCurve);//(m_digestCurve);
}
