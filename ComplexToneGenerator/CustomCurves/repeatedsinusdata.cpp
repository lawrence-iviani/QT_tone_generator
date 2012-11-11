#include "repeatedsinusdata.h"


RepeatedSinusData::RepeatedSinusData(qreal duration, qreal SRGen, QWidget *widget) :
    RepeatedTimeData(duration,SRGen)
{
    m_initPhase=SINUSDATA_DEFAULT_INITPHASE;
    m_frequency=SINUSDATA_DEFAULT_FREQUENCY;
    m_amplitude=SINUSDATA_DEFAULT_AMPLITUDE;

    m_sinusDataUI=new SinusDataUI(this,widget);
    this->updateData();
}

RepeatedSinusData::~RepeatedSinusData() {

}

RepeatedSinusData::RepeatedSinusData( qreal duration, qreal SRGen,qreal amplitude, qreal frequency, qreal initPhase, QWidget *widget) :
    RepeatedTimeData(duration,SRGen)
{
    m_initPhase=initPhase;
    m_frequency=frequency;
    m_amplitude=amplitude;

    m_sinusDataUI=new SinusDataUI(this,widget);
    this->updateData();
}

void RepeatedSinusData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz") << " - RepeatedSinusData::recalc() ---------------- " << this->name();
    const qreal * t=this->getTimeData();
    qreal phase=RepeatedSinusData::deg2rad(this->initPhase());

    qint64 n_dw=lowestSampleIndexForModification();
    qint64 n_up=highestSampleIndexForModification();
    qDebug() << "RepeatedSinusData::recalc() m_max_Duration=" << this->maxDuration() <<" m_duration=" << this->duration()  << " n_dw=" << n_dw << " n_up=" << n_up << " nsample=" << this->sampleNumber();

    for (qint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(this->insertSignalValue(n,this->amplitude()*sin(2*M_PI*this->frequency()*t[n]+phase)));
    }
}

void RepeatedSinusData::setAmplitudeFrequencyAndPhase(qreal amplitude,qreal frequency,qreal initPhase) {
    m_amplitude=amplitude;
    m_frequency=frequency;
    m_initPhase=initPhase;
    this->updateData();
}

void RepeatedSinusData::setAmplitude(qreal amplitude) {
    m_amplitude=amplitude;
    this->updateData();
}

void RepeatedSinusData::setFrequency(qreal frequency) {
    m_frequency=frequency;
    this->updateData();
}

void RepeatedSinusData::setInitPhase(qreal initPhase) {
    m_initPhase=initPhase;
    this->updateData();
}


