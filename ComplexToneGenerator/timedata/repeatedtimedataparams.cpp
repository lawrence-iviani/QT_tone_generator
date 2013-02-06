#include "repeatedtimedataparams.h"

RepeatedTimeDataParams::RepeatedTimeDataParams(QObject *parent) :
    PartialTimeDataParams(parent),
    m_blankTime(0)
{}

RepeatedTimeDataParams::RepeatedTimeDataParams(PartialTimeDataParams * baseProperty, TimePlotParams* params,QObject *parent) :
    PartialTimeDataParams(baseProperty,params,parent),
    m_blankTime(0)
{}

void RepeatedTimeDataParams::setBlankTime(qreal blankTime) {
    if (m_blankTime!=blankTime && m_blankTime >=0) {
        m_blankTime=blankTime;
        emit (blankTimeChanged(blankTime));
    }
}
