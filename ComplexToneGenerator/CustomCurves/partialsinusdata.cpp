#include "partialsinusdata.h"




PartialSinusData::PartialSinusData(qreal duration, qreal SRGen, QWidget *widget) :
    PartialTimeData (duration,SRGen)
{
    //Create the storage class
    m_sinusData=new SinusData((QObject*)widget);
    //Connect a signal to call an update if the parameters are changed
    connect(m_sinusData,SIGNAL(dataUpdated()),this,SLOT(updateData()));
    //Create a sinusdata UI, connecting the parameters
    m_sinusDataUI=new SinusDataUI(m_sinusData,widget);
    //Register the UI for call general update when something change.
    this->getControlWidget()->addControlFrame((CustomCurveUI*) m_sinusDataUI, "GenricSinusData control");
}

PartialSinusData::~PartialSinusData() {

}

PartialSinusData::PartialSinusData(qreal duration, qreal SRGen, qreal amplitude, qreal frequency, qreal initPhase, QWidget *widget) :
    PartialTimeData (duration,SRGen)
{
    //Create the storage class
    m_sinusData=new SinusData(amplitude,frequency,initPhase, (QObject*)widget);
    //Connect a signal to call an update if the parameters are changed
    connect(m_sinusData,SIGNAL(dataUpdated()),this,SLOT(updateData()));
    //Create a sinusdata UI, connecting the parameters
    m_sinusDataUI=new SinusDataUI(m_sinusData,widget);
    //Register the UI for call general update when something change.
    this->getControlWidget()->addControlFrame((CustomCurveUI*) m_sinusDataUI, "GenricSinusData control");
}


void PartialSinusData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz") << " - PartialSinusData::recalc() ---------------- " << this->name();
    const qreal *t=this->getTimeData();
    qreal phase=SinusData::deg2rad(m_sinusData->initPhase());

    qint64 n_dw=this->lowestSampleIndexForModification();
    qint64 n_up=this->highestSampleIndexForModification();
    qDebug() << "PartialSinusData::recalc() m_max_Duration=" << this->maxDuration() <<" m_duration=" << this->duration()  << " n_dw=" << n_dw << " n_up=" << n_up << " nsample=" << this->sampleNumber();

    for (qint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(this->insertSignalValue(n,m_sinusData->amplitude()*sin(2*M_PI*m_sinusData->frequency()*t[n]+phase)));
    }
}


