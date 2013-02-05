#include "timeplotwidgetparams.h"

TimePlotParams::TimePlotParams(QObject *object) :
    DataUiHandlerProperty(object),
    m_t0(TIMEDATA_DEFAULT_MIN_TIME)
{
    setMaxDuration(TIMEDATA_DEFAULT_PROJECT_TIME);
    setSampleRate(TIMEDATA_DEFAULT_SR);
}

TimePlotParams::TimePlotParams(qreal duration, qreal sampleRate, DataUiHandlerProperty * baseProperty, QObject *object) :
    DataUiHandlerProperty(baseProperty, object),
    m_t0(TIMEDATA_DEFAULT_MIN_TIME)
{
    setMaxDuration(duration);
    setSampleRate(sampleRate);
}

void TimePlotParams::setMaxDuration(qreal maxduration) {
    if ( (maxduration < 0) || (maxduration > TIMEDATA_DEFAULT_MAX_TIME)) return;
    if (maxduration!=m_maxDuration) {
        m_maxDuration=maxduration;
        emit (maxDurationChanged(maxduration));
    }
}

void TimePlotParams::setSampleRate(qreal sampleRate) {
    if (sampleRate < TIMEDATA_DEFAULT_MIN_SR || sampleRate > TIMEDATA_DEFAULT_MAX_SR ) return;
    if (sampleRate!=m_sampleRate) {
        m_sampleRate=sampleRate;
        emit (sampleRateChanged(sampleRate));
    }
}
