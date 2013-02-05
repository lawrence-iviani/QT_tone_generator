#include "repeatedtimedata.h"

RepeatedTimeData::RepeatedTimeData(QObject *parent) :
    PartialTimeData(parent)
{ init(NULL); }

RepeatedTimeData::RepeatedTimeData(TimePlotParams * timePlotParams ,QObject *parent) :
    PartialTimeData(timePlotParams,parent)
{ init(timePlotParams); }

void RepeatedTimeData::init(TimePlotParams *timePlotParams) {
    PartialTimeDataParams* _baseProp=dynamic_cast<PartialTimeDataParams*>(GenericTimeData::getDataParameters());
    Q_ASSERT(_baseProp);
    DataUiHandlerDelegate* _delegate=getDelegate();
    RepeatedTimeDataParams* _derivedProp=new RepeatedTimeDataParams( _baseProp,timePlotParams,(QObject*)this);
    DataUiHandlerProperty* _castedDerivedProp=dynamic_cast<DataUiHandlerProperty*>(_derivedProp);
    _delegate->replacePropertiesAndUI(_castedDerivedProp,
                                      dynamic_cast<DataUiHandlerUI*> (new RepeatedTimeDataUI() ));
    RepeatedTimeDataParams* _rtp=dynamic_cast<RepeatedTimeDataParams*>(getDataParameters());
    Q_ASSERT(_rtp);
    _rtp->setBlankTime(REPEATEDTIMEDATA_DEFAULT_BLANK);
    _rtp->setDuration(PARTIALTIMEDATA_DEFAULT_DURATION/(REPEATEDTIMEDATA_DEFAULT_BLANK*2));
    updateRepetitions();
    connectSignals();
}

void RepeatedTimeData::connectSignals() {
    RepeatedTimeData::connectSignals();
    //Connect the slot to change the max duration
 //   connect(this ,SIGNAL(maxDurationChanged(qreal)),this,SLOT(updateRepetitions()));
 //   connect(this ,SIGNAL(startTimeChanged(qreal)),this,SLOT(updateRepetitions()));
 //   connect(this ,SIGNAL(durationChanged(qreal)),this,SLOT(updateRepetitions()));
    //EXAMPLE, to connect your class signal to update UI, use this example, connecting to the main control widget.
    //connect(this ,SIGNAL(SOME SIGNAL),getControlWidget(),SLOT(updateUI());
}

void RepeatedTimeData::updateRepetitions() {
    RepeatedTimeDataParams* _params=dynamic_cast<RepeatedTimeDataParams*>(getDataParameters());
    Q_ASSERT(_params);
    //Get the total time available for all the repetitions (including the first and orginal one)
    qreal _availableDuration=_params->maxDuration()-_params->t0();

    //Calculate one shot duration
    qreal _singleShotDuration=_params->duration()+_params->blankTime();
    m_repetitions=qFloor(_availableDuration/(_singleShotDuration));

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"_availableDuration(%1)/_singleShotDuration(%2)=")
                        .arg(_availableDuration)
                        .arg(_singleShotDuration)
                        .arg(_availableDuration/_singleShotDuration));
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"m_repetitions=%1").arg(m_repetitions));

}


//void RepeatedTimeData::setBlankTime(qreal blanktime) {
//    //the max  blank time must be less of the total length minus the length and start time of one shot signal (PartialTimeData)
//    qreal _maxBlankTime=maxDuration()-duration()+startTime();
//    Q_ASSERT(_maxBlankTime>=0);

//    blanktime=(blanktime>=0 ? blanktime:0);
//    //Clip to the max blanktime available
//    m_blankTime=(blanktime < _maxBlankTime ? blanktime : _maxBlankTime);
//    updateRepetitions();
//    createData();
//}

bool RepeatedTimeData::insertSignalValue(quint64 index, qreal value) {
    bool retval=false;
    const qreal * envelopData;
    qreal *_s=signalData();
    RepeatedTimeDataParams* _params=dynamic_cast<RepeatedTimeDataParams*>(getDataParameters());
    Q_ASSERT(_params);
    quint64 _sampleShotDuration=qFloor((_params->duration()+_params->blankTime())*_params->sampleRate());

    quint64 lowestIndex=lowestSampleIndexForModification();
    bool _envEnable=getEnvelopeData()->isEnableEnvelope();
    if (_envEnable) {
        envelopData=getEnvelopeData()->getEnvelope();
    }
    qreal _tVal;
    if ((lowestIndex<=index) && (index<=highestSampleIndexForModification())  ) {
        _tVal=(_envEnable ? value*envelopData[index-lowestIndex] : value);
        quint64 i;
        for (unsigned int _rep=0; _rep < m_repetitions; _rep++) {
            i=_rep*_sampleShotDuration+index;
            Q_ASSERT(i<=getSampleNumber());
            _s[i]=_tVal;
        }
        retval=true;
    }
    return retval;
}
