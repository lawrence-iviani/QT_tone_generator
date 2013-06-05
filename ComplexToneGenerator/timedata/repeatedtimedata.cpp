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
    PartialTimeData::connectSignals();

    RepeatedTimeDataParams* _rtd=dynamic_cast<RepeatedTimeDataParams*>(getDataParameters());
    Q_ASSERT(_rtd);
    RepeatedTimeDataUI* _rtdUI=dynamic_cast<RepeatedTimeDataUI*>(this->getDelegate()->getUI());
    Q_ASSERT(_rtdUI);

    //Q_ASSERT(connect(_rtd,SIGNAL(blankTimeChanged(qreal)),this,SLOT(createData())));
    Q_ASSERT(connect(_rtd,SIGNAL(blankTimeChanged(qreal)),this,SLOT(updateBlankTime())));

    //Connect all the signal that can potentially change the number of repetitions
    Q_ASSERT(connect(_rtd ,SIGNAL(maxDurationChanged(qreal)),this,SLOT(updateRepetitions())));
    Q_ASSERT(connect(_rtd ,SIGNAL(t0Changed(qreal)),this,SLOT(updateRepetitions())));
    Q_ASSERT(connect(_rtd ,SIGNAL(durationChanged(qreal)),this,SLOT(updateRepetitions())));

    //if maxduration changed update the blank scale
    Q_ASSERT(connect(_rtd,SIGNAL(maxDurationChanged(qreal)),_rtdUI,SLOT(setBlankTimeScale(qreal))));

    //setting scale for UI
    _rtdUI->setBlankTimeScale(_rtd->maxDuration());
}

void RepeatedTimeData::updateBlankTime() {
    updateRepetitions(true);
}

void RepeatedTimeData::updateRepetitions(bool forceUpdate) {
    RepeatedTimeDataParams* _params=dynamic_cast<RepeatedTimeDataParams*>(getDataParameters());
    Q_ASSERT(_params);
    //Get the total time available for all the repetitions (including the first and orginal one)
    qreal _availableDuration=_params->maxDuration()-_params->t0()+_params->blankTime();

    //Calculate one shot duration
    qreal _singleShotDuration=_params->duration()+_params->blankTime();
    quint64 repetitions=qFloor(_availableDuration/(_singleShotDuration));
    if(repetitions!=m_repetitions || forceUpdate) {
        m_repetitions=repetitions;
        PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"m_repetitions _availableDuration(%1)/_singleShotDuration(%2)=%3 repetitions (m_repetitions=%4)")
                            .arg(_availableDuration)
                            .arg(_singleShotDuration)
                            .arg(_availableDuration/_singleShotDuration)
                          .arg(m_repetitions));
        //recall an update
        createData();
    } else {
        PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"m_repetitions=%1 doesn't change").arg(m_repetitions));
    }
}

bool RepeatedTimeData::insertSignalValue(quint64 index, qreal value) {
    bool retval=false;
    const qreal * envelopData;
    qreal *_s=signalData();
    RepeatedTimeDataParams* _params=dynamic_cast<RepeatedTimeDataParams*>(getDataParameters());
    Q_ASSERT(_params);
    quint64 _sampleSingleShotDuration=qFloor((_params->duration()+_params->blankTime())*_params->sampleRate());

    quint64 lowestIndex=lowestSampleIndexForModification();
    bool _envEnable=getEnvelopeParameters()->isEnabledEnvelope();
    if (_envEnable) {
        envelopData=getEnvelope()->getEnvelope();
    }
    qreal _tVal;
    if ((lowestIndex<=index) && (index<=highestSampleIndexForModification())  ) {
        _tVal=(_envEnable ? value*envelopData[index-lowestIndex] : value);
        quint64 i;
        for (unsigned int _rep=0; _rep < m_repetitions; _rep++) {
            i=_rep*_sampleSingleShotDuration+index;
            // Q_ASSERT(i<=getSampleNumber());
            if (i<=getSampleNumber())
                _s[i]= _tVal;
            else
                break;
        }
        retval=true;
    }
    return retval;
}
