#include "sinusdataparams.h"


SinusDataParams::SinusDataParams( QObject *object) :
    QObject(object)
{
    m_initPhase=SINUSDATA_DEFAULT_INITPHASE;
    m_frequency=SINUSDATA_DEFAULT_FREQUENCY;
    m_amplitude=SINUSDATA_DEFAULT_AMPLITUDE;
}

SinusDataParams::~SinusDataParams() {

}

SinusDataParams::SinusDataParams( qreal amplitude, qreal frequency, qreal initPhase, QObject *object) :
    QObject(object)
{
    m_initPhase=initPhase;
    m_frequency=frequency;
    m_amplitude=amplitude;
}

void SinusDataParams::setAmplitudeFrequencyAndPhase(qreal amplitude,qreal frequency,qreal initPhase) {
    m_amplitude=amplitude;
    m_frequency=frequency;
    m_initPhase=initPhase;
    emit(dataUpdated());
}

void SinusDataParams::setAmplitude(qreal amplitude) {
    m_amplitude=amplitude;
    emit(dataUpdated());
}

void SinusDataParams::setFrequency(qreal frequency) {
    m_frequency=frequency;
    emit(dataUpdated());
}

void SinusDataParams::setInitPhase(qreal initPhase) {
    m_initPhase=initPhase;
    emit(dataUpdated());
}

