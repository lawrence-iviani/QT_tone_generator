#include "partialsinusdata.h"



PartialSinusData::PartialSinusData(qreal duration, qreal SRGen, QWidget *widget) :
    PartialTimeData(duration,SRGen)
{
    m_initPhase=SINUSDATA_DEFAULT_INITPHASE;
    m_frequency=SINUSDATA_DEFAULT_FREQUENCY;
    m_amplitude=SINUSDATA_DEFAULT_AMPLITUDE;

    m_sinusDataUI=new SinusDataUI(this,widget);
    this->updateData();
}

PartialSinusData::~PartialSinusData() {

}

PartialSinusData::PartialSinusData( qreal duration, qreal SRGen,qreal amplitude, qreal frequency, qreal initPhase, QWidget *widget) :
    PartialTimeData(duration,SRGen)
{
    m_initPhase=initPhase;
    m_frequency=frequency;
    m_amplitude=amplitude;

    m_sinusDataUI=new SinusDataUI(this,widget);
    this->updateData();
}

void PartialSinusData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz") << " - PartialSinusData::recalc() ---------------- " << this->name();
    const qreal * t=this->getTimeData();
    qreal phase=PartialSinusData::deg2rad(this->initPhase());

    qint64 n_dw=this->lowestSampleIndexForModification();
    qint64 n_up=this->highestSampleIndexForModification();
    qDebug() << "PartialSinusData::recalc() m_max_Duration=" << this->maxDuration() <<" m_duration=" << this->duration()  << " n_dw=" << n_dw << " n_up=" << n_up << " nsample=" << this->sampleNumber();

    for (qint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(this->insertSignalValue(n,this->amplitude()*sin(2*M_PI*this->frequency()*t[n]+phase)));
    }
}

void PartialSinusData::setAmplitudeFrequencyAndPhase(qreal amplitude,qreal frequency,qreal initPhase) {
    m_amplitude=amplitude;
    m_frequency=frequency;
    m_initPhase=initPhase;
    this->updateData();
}

void PartialSinusData::setAmplitude(qreal amplitude) {
    m_amplitude=amplitude;
    this->updateData();
}

void PartialSinusData::setFrequency(qreal frequency) {
    m_frequency=frequency;
    this->updateData();
}

void PartialSinusData::setInitPhase(qreal initPhase) {
    m_initPhase=initPhase;
    this->updateData();
}


