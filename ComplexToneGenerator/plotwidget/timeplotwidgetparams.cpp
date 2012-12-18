#include "timeplotwidgetparams.h"

TimePlotParams::TimePlotParams(QObject *parent) :
    QObject(parent),
    DomHelper(this),
    m_duration(TIMEDATA_DEFAULT_MAX_TIME),
    m_sampleRate(TIMEDATA_DEFAULT_SR),
    m_t0(TIMEDATA_DEFAULT_MIN_TIME)
{
    regenerateDomDocument();
}

TimePlotParams::TimePlotParams(qreal duration, qreal sampleRate, QObject *parent) :
    QObject(parent),
    DomHelper(this),
    m_duration(TIMEDATA_DEFAULT_MAX_TIME),
    m_sampleRate(TIMEDATA_DEFAULT_SR),
    m_t0(TIMEDATA_DEFAULT_MIN_TIME)
{
    setDuration(duration);
    setSampleRate(sampleRate);
    regenerateDomDocument();
}

bool TimePlotParams::setDuration(qreal duration) {
    if (duration < 0.0 || duration > TIMEDATA_DEFAULT_MAX_TIME) return false;
    if (duration==m_duration) return false;
    m_duration=duration;
    regenerateDomDocument();
    return true;
}

bool TimePlotParams::setSampleRate(qreal sampleRate) {
    if (sampleRate < TIMEDATA_DEFAULT_MIN_SR || sampleRate > TIMEDATA_DEFAULT_MAX_SR ) return false;
    if (sampleRate==m_sampleRate) return false;
    m_sampleRate=sampleRate;
    regenerateDomDocument();
    return true;
}

void TimePlotParams::regenerateDomDocument()
{
    //Get DOM document of this object QPROPERTY
    if ( m_doc!=NULL) delete m_doc;
    m_doc=new QDomDocument();
    Q_ASSERT(this->selfObjectData(m_doc,PROJECTPARAMETERS_TAG));
    Q_ASSERT(!m_doc->isNull());
    Q_ASSERT(m_doc->isDocument());
   // qDebug() << "TimePlotParams::regenerateDomDocument \n" << m_doc->toString(2);
}

