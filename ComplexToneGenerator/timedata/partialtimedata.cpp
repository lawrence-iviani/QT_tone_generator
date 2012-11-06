#include "partialtimedata.h"

PartialTimeData::PartialTimeData(QObject *parent,QWidget *widget) :
    GenericTimeData(parent,widget)
{
    m_t0=TIMEDATA_DEFAULT_INITTIME;
   // this->connectSignal();
    this->setDuration(TIMEDATA_DEFAULT_DURATION);
    m_partialTimeDataUI=new PartialTimeDataUI(this,widget);
}

PartialTimeData::PartialTimeData(qreal duration, qreal SRGen,QObject *parent,QWidget *widget) :
    GenericTimeData(duration,SRGen,parent,widget)
{
    m_t0=TIMEDATA_DEFAULT_INITTIME;
  //  this->connectSignal();
    this->setDuration(duration);
    m_partialTimeDataUI=new PartialTimeDataUI(this,widget);
}

void PartialTimeData::connectSignal() {
    connect(this,SIGNAL(dataUpdated()),m_partialTimeDataUI,SLOT(updateUI()));
    connect(this,SIGNAL(curveAttributeUpdated()),m_partialTimeDataUI,SLOT(updateUI()));
}

void PartialTimeData::setDuration(qreal duration) {
    if (duration < 0) {
        duration=0;
    }

    /*This avoid start time happens afer max duration.
      */
    if (this->startTime() > this->maxDuration()) this->setStartTime(this->maxDuration());

    qreal maxtime=this->minStartTime()+this->maxDuration();//The max time allowed by the base class
    if ((this->startTime()+duration) > maxtime) {
        m_duration=maxtime-this->startTime();
    } else {
        m_duration=duration;
    }
    this->createData();//Data need to be recalculated, not only updated
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
    //Call set duration to verify if the duration must be clipped and to recalc  the curve
    this->setDuration(m_duration);
}

void PartialTimeData::setMaxDuration(qreal maxDuration) {
    if ( (this->startTime()+this->duration())>maxDuration ) {
        this->setMaxDurationAndUpdate(maxDuration,false);
        this->setDuration(maxDuration-this->startTime());
    } else {
        this->setMaxDurationAndUpdate(maxDuration,true);
    }
}


