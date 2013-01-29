#include "partialtimedata.h"

PartialTimeData::PartialTimeData(QWidget *widget) :
    GenericTimeData(widget)
{
    m_t0=PARTIALTIMEDATA_DEFAULT_INITTIME;
    this->connectSignal();
    this->setDuration(PARTIALTIMEDATA_DEFAULT_DURATION);
    m_partialTimeDataUI=new PartialTimeDataUI(this,widget);
}

PartialTimeData::PartialTimeData(TimePlotParams *timePlotParams, QWidget *widget) :
    GenericTimeData(timePlotParams,widget)
{
    m_t0=PARTIALTIMEDATA_DEFAULT_INITTIME;
    this->connectSignal();
    this->setDuration(timePlotParams->duration());
    m_partialTimeDataUI=new PartialTimeDataUI(this,widget);
}

void PartialTimeData::connectSignal() {
    //Connect the slot to change the max duration
    connect(this ,SIGNAL(maxDurationChanged(qreal)),this,SLOT(maxDurationChange(qreal)));
    //EXAMPLE, to connect your class signal to update UI, use this example, connecting to the main control widget.
    //connect(this ,SIGNAL(SOME SIGNAL),getControlWidget(),SLOT(updateUI());
}

void PartialTimeData::setDuration(qreal duration) {
    if (duration!=m_duration) {
        privateSetDuration(duration);
        emit (durationChanged(duration));
        this->createData();//Data need to be recalculated, not only updated
    }
}

void PartialTimeData::privateSetDuration(qreal duration) {
    if (duration < 0) {
        duration=0;
    }

    /*This avoid start time happens afer max duration.
      */
    if (this->startTime() > this->maxDuration()) this->setStartTime(this->maxDuration());

    qreal maxtime=this->minStartTime()+this->maxDuration();//The max time allowed by the base class
    m_duration=( ((this->startTime()+duration) > maxtime) ? maxtime-this->startTime() : duration );

}

void PartialTimeData::setStartTime(qreal t0) {
    qreal maxtime=this->minStartTime()+this->maxDuration();//The max time allowed by the base class

    if (t0 < this->minStartTime()) {
        m_t0=this->minStartTime();
    } else if ( t0>maxtime) {
        m_t0=maxtime;
    } else {
        m_t0=t0;
    }
    emit(startTimeChanged(t0));

    //Call set duration to verify if the duration must be clipped and to recalc  the curve
    if ( (m_t0+m_duration) > maxtime ) {
           privateSetDuration(m_duration);
    }
    createData();
}

void PartialTimeData::maxDurationChange(qreal maxDuration) {
    if ( (this->startTime()+this->duration())>maxDuration ) {
        this->setDuration(maxDuration-this->startTime());
    }
    m_partialTimeDataUI->durationChange(maxDuration);
}

qint64 PartialTimeData::lowestSampleIndexForModification()  {
    qreal _minDuration=qMax(this->minStartTime() ,this->startTime()-this->minStartTime());
    qint64 retval=_minDuration*this->sampleRate();
    Q_ASSERT(retval >=0);
    Q_ASSERT(retval <=this->sampleNumber());
    return retval;
}

qint64 PartialTimeData::highestSampleIndexForModification() {
    qreal _maxDuration=qMin(this->minStartTime()+this->maxDuration(),this->startTime()+this->duration());
    qint64 retval=_maxDuration*this->sampleRate();
    Q_ASSERT(retval >=0);
    Q_ASSERT(retval <=this->sampleNumber());
    return retval;
}
