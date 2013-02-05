#include "partialtimedata.h"

PartialTimeData::PartialTimeData(QObject *parent) :
    GenericTimeData(parent)
{ init(NULL); }

PartialTimeData::PartialTimeData(TimePlotParams *timePlotParams, QObject *parent) :
    GenericTimeData(timePlotParams,parent)
{    init(timePlotParams); }

void PartialTimeData::init(TimePlotParams *timePlotParams) {
    GenericTimeDataParams* _baseProp=dynamic_cast<GenericTimeDataParams*>(GenericTimeData::getDataParameters());
    Q_ASSERT(_baseProp);
    DataUiHandlerDelegate* _delegate=getDelegate();
    PartialTimeDataParams* _derivedProp=new PartialTimeDataParams( _baseProp,timePlotParams,(QObject*)this);
    DataUiHandlerProperty* _castedDerivedProp=dynamic_cast<DataUiHandlerProperty*>(_derivedProp);
    _delegate->replacePropertiesAndUI(_castedDerivedProp,
                                      dynamic_cast<DataUiHandlerUI*> (new PartialTimeDataUI() ));
    this->connectSignal();
    PartialTimeDataParams* _ptp=dynamic_cast<PartialTimeDataParams*>(getDataParameters());
    Q_ASSERT(_ptp);
    _ptp->sett0( PARTIALTIMEDATA_DEFAULT_T0);
    _ptp->setDuration(PARTIALTIMEDATA_DEFAULT_DURATION);
}

void PartialTimeData::connectSignal() {
    //Connect the slot to change the max duration
    //connect(this ,SIGNAL(maxDurationChanged(qreal)),this,SLOT(maxDurationChange(qreal)));//This grant when never SR o duration is changed the appropriate methods are called!
    //EXAMPLE, to connect your class signal to update UI, use this example, connecting to the main control widget.
    //connect(this ,SIGNAL(SOME SIGNAL),getControlWidget(),SLOT(updateUI());
}

//void PartialTimeData::setDuration(qreal duration) {
//    if (duration!=m_duration) {
//        privateSetDuration(duration);
//        emit (durationChanged(duration));
//        this->createData();//Data need to be recalculated, not only updated
//    }
//}

//void PartialTimeData::privateSetDuration(qreal duration) {
//    if (duration < 0) {
//        duration=0;
//    }

//    /*This avoid start time happens afer max duration.
//      */
//    if (this->startTime() > this->maxDuration()) this->setStartTime(this->maxDuration());

//    qreal maxtime=this->minStartTime()+this->maxDuration();//The max time allowed by the base class
//    m_duration=( ((this->startTime()+duration) > maxtime) ? maxtime-this->startTime() : duration );

//}

//void PartialTimeData::setStartTime(qreal t0) {
//    qreal maxtime=this->minStartTime()+this->maxDuration();//The max time allowed by the base class

//    if (t0 < this->minStartTime()) {
//        m_t0=this->minStartTime();
//    } else if ( t0>maxtime) {
//        m_t0=maxtime;
//    } else {
//        m_t0=t0;
//    }
//    emit(startTimeChanged(t0));

//    //Call set duration to verify if the duration must be clipped and to recalc  the curve
//    if ( (m_t0+m_duration) > maxtime ) {
//           privateSetDuration(m_duration);
//    }
//    createData();
//}

void PartialTimeData::maxDurationChange(qreal maxDuration) {
    PartialTimeDataParams* _params=dynamic_cast<PartialTimeDataParams*> (getDataParameters());
    Q_ASSERT(_params);
    if ( (_params->t0()+_params->duration())>maxDuration ) {
        _params->setDuration(maxDuration-_params->t0());
    }
}

quint64 PartialTimeData::lowestSampleIndexForModification()  {
    PartialTimeDataParams* _params=dynamic_cast<PartialTimeDataParams*> (getDataParameters());
    Q_ASSERT(_params);
    qreal _minDuration=qMax(_params->startTime() ,_params->t0()-_params->startTime());
    quint64 retval=_minDuration*_params->sampleRate();
    Q_ASSERT(retval<=getSampleNumber() );
    return retval;
}

quint64 PartialTimeData::highestSampleIndexForModification() {
    PartialTimeDataParams* _params=dynamic_cast<PartialTimeDataParams*> (getDataParameters());
    Q_ASSERT(_params);
    qreal _maxDuration=qMin(_params->startTime()+_params->maxDuration(),_params->startTime()+_params->duration());
    quint64 retval=_maxDuration*_params->sampleRate();
    Q_ASSERT(retval<=getSampleNumber());
    return retval;
}
