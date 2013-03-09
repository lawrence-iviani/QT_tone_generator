#include "partialtimedata.h"

PartialTimeData::PartialTimeData(QObject *parent) :
    GenericTimeData(parent)
{ init(); }

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
    PartialTimeDataParams* _ptp=dynamic_cast<PartialTimeDataParams*>(getDataParameters());
    Q_ASSERT(_ptp);

    //setting default values for t0 and duration
    _ptp->setT0(PARTIALTIMEDATA_DEFAULT_T0);
    _ptp->setDuration(PARTIALTIMEDATA_DEFAULT_DURATION);

    connectSignals();
}

void PartialTimeData::connectSignals() {
    GenericTimeData::connectSignals();

    PartialTimeDataParams* _ptd=dynamic_cast<PartialTimeDataParams*>(getDataParameters());
    Q_ASSERT(_ptd);
    PartialTimeDataUI* _ptdUI=dynamic_cast<PartialTimeDataUI*>(this->getDelegate()->getUI());
    Q_ASSERT(_ptdUI);

    //if parameters change, update plot
    Q_ASSERT(connect(_ptd,SIGNAL(durationChanged(qreal)),this,SLOT(createData())));
    Q_ASSERT(connect(_ptd,SIGNAL(t0Changed(qreal)),this,SLOT(createData())));


    //if maxduration change update t0 scale and duration scale
    Q_ASSERT(connect(_ptd,SIGNAL(maxDurationChanged(qreal)),_ptdUI,SLOT(setT0Scale(qreal))));
    //if t0 changed update the duration scale
    Q_ASSERT(connect(_ptd,SIGNAL(t0Changed(qreal)),_ptdUI,SLOT(setDurationScale())));

    //setting scale for UI
    _ptdUI->setT0Scale(_ptd->maxDuration());
    _ptdUI->setT0Scale(_ptd->maxDuration());
}

void PartialTimeData::maxDurationHasChanged(qreal maxDuration) {
    PartialTimeDataParams* _params=dynamic_cast<PartialTimeDataParams*> (getDataParameters());
    Q_ASSERT(_params);
    if ( (_params->t0()+_params->duration())>maxDuration ) {
        _params->setDuration(maxDuration-_params->t0());
    }
    GenericTimeData::maxDurationHasChanged(maxDuration);
}

const quint64 PartialTimeData::lowestSampleIndexForModification()  {
    PartialTimeDataParams* _params=dynamic_cast<PartialTimeDataParams*> (getDataParameters());
    Q_ASSERT(_params);
    qreal _minDuration=qMax(_params->startTime() ,_params->t0()-_params->startTime());
    quint64 retval=_minDuration*_params->sampleRate();
    //Q_ASSERT(retval<=getSampleNumber() );
    return (qMin(retval,getSampleNumber()));
}

const quint64 PartialTimeData::highestSampleIndexForModification() {
    PartialTimeDataParams* _params=dynamic_cast<PartialTimeDataParams*> (getDataParameters());
    Q_ASSERT(_params);
    qreal _maxDuration=qMin(_params->startTime()+_params->maxDuration(),_params->t0()+_params->duration());
    quint64 retval=_maxDuration*_params->sampleRate();
 //   Q_ASSERT(retval<=getSampleNumber());
    return  (qMin(retval,getSampleNumber()));;
}
