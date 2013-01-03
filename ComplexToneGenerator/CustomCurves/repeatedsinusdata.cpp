#include "repeatedsinusdata.h"


RepeatedSinusData::RepeatedSinusData(QWidget *widget) :
    RepeatedTimeData (widget)
{
    //Create the storage class
    m_sinusDataParams=new SinusDataParams((QObject*)widget);
    //Connect a signal to call an update if the parameters are changed
    connect(m_sinusDataParams,SIGNAL(dataUpdated()),this,SLOT(updateData()));
    //Create a sinusdata UI, connecting the parameters
    m_sinusDataUI=new SinusDataUI(m_sinusDataParams,widget);
    //Register the UI for call general update when something change.
    this->getControlWidget()->addControlFrame((CustomCurveUI*) m_sinusDataUI, "RepeatedSinusData control");
}

RepeatedSinusData::RepeatedSinusData(TimePlotParams *timePlotParams, QWidget *widget) :
    RepeatedTimeData (timePlotParams)
{
    //Create the storage class
    m_sinusDataParams=new SinusDataParams((QObject*)widget);
    //Connect a signal to call an update if the parameters are changed
    connect(m_sinusDataParams,SIGNAL(dataUpdated()),this,SLOT(updateData()));
    //Create a sinusdata UI, connecting the parameters
    m_sinusDataUI=new SinusDataUI(m_sinusDataParams,widget);
    //Register the UI for call general update when something change.
    this->getControlWidget()->addControlFrame((CustomCurveUI*) m_sinusDataUI, "RepeatedSinusData control");
}

RepeatedSinusData::~RepeatedSinusData() {

}

RepeatedSinusData::RepeatedSinusData(TimePlotParams * timePlotParams,qreal amplitude, qreal frequency, qreal initPhase, QWidget *widget) :
    RepeatedTimeData (timePlotParams)
{
    //Create the storage class
    m_sinusDataParams=new SinusDataParams(amplitude,frequency,initPhase, (QObject*)widget);
    //Connect a signal to call an update if the parameters are changed
    connect(m_sinusDataParams,SIGNAL(dataUpdated()),this,SLOT(updateData()));
    //Create a sinusdata UI, connecting the parameters
    m_sinusDataUI=new SinusDataUI(m_sinusDataParams,widget);
    //Register the UI for call general update when something change.
    this->getControlWidget()->addControlFrame((CustomCurveUI*) m_sinusDataUI, "RepeatedSinusData control");
}

void RepeatedSinusData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz")  << " - RepeatedSinusData::recalc() is enabled---------------- " << this->name();
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz")  << " - RepeatedSinusData::recalc() SR=" << this->sampleRate()
            << " nsamples="<< this->sampleNumber();

    const qreal *t=this->getTimeData();
    qreal phase=SinusDataParams::deg2rad(m_sinusDataParams->initPhase());

    qint64 n_dw=this->lowestSampleIndexForModification();
    qint64 n_up=this->highestSampleIndexForModification();
    qDebug() << "RepeatedSinusData::recalc() m_max_Duration=" << this->maxDuration() <<" m_duration=" << this->duration()  << " n_dw=" << n_dw << " n_up=" << n_up << " nsample=" << this->sampleNumber();

    for (qint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(this->insertSignalValue(n,m_sinusDataParams->amplitude()*sin(2*M_PI*m_sinusDataParams->frequency()*t[n]+phase)));
    }
}
