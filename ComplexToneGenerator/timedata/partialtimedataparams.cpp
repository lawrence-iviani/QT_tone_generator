#include "partialtimedataparams.h"

PartialTimeDataParams::PartialTimeDataParams(QObject *parent) :
    GenericTimeDataParams(parent),
    m_t0(0),
    m_duration(0)
{}


PartialTimeDataParams::PartialTimeDataParams(GenericTimeDataParams * baseProperty, TimePlotParams* params,QObject *parent) :
    GenericTimeDataParams(baseProperty,params,parent),
    m_t0(0),
    m_duration(0)
{}

void PartialTimeDataParams::setT0(qreal t0) {
    if (m_t0!=t0) {
        m_t0=t0;
        emit (t0Changed(t0));
    }
}

void PartialTimeDataParams::setDuration(qreal duration) {
    if (m_duration != duration  && duration >= 0) {
        m_duration=duration;
        emit (durationChanged(duration));
    }
}
