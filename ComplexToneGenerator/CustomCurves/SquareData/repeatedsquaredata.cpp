#include "repeatedsquaredata.h"


//---------- FRONTEND ----------
void RepeatedSquareData::init(qreal amplitude,qreal frequency, qreal meanValue) {
    RepeatedSquareParams* _derivedProp=new RepeatedSquareParams((QObject*)this);
    getDelegate()->replacePropertiesAndUI(dynamic_cast<DataUiHandlerProperty*>(_derivedProp),
                                      dynamic_cast<DataUiHandlerUI*> (new RepeatedSquareUI() ));


    //Set any eventual default parameters or passed by the constructor argmuents
    RepeatedSquareParams* _rsp=dynamic_cast<RepeatedSquareParams*>(getDataParameters());
    Q_ASSERT(_rsp);
    _rsp->setAmplitude(amplitude);
    _rsp->setFrequency(frequency);
    _rsp->setMeanValue(meanValue);
    connectSignals();
}

void RepeatedSquareData::connectSignals() {
    RepeatedTimeData::connectSignals();

    RepeatedSquareParams* _rsp=dynamic_cast<RepeatedSquareParams*>(getDataParameters());
    Q_ASSERT(_rsp);
    Q_ASSERT(connect(_rsp,SIGNAL(amplitudeChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_rsp,SIGNAL(frequencyChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_rsp,SIGNAL(meanValueChanged(qreal)),this,SLOT(updateData())));
}

void RepeatedSquareData::recalc() {
    RepeatedSquareParams* _rsp=dynamic_cast<RepeatedSquareParams*>(getDataParameters());
    Q_ASSERT(_rsp);

    //Verify going to recalc with the enable flag true
    Q_ASSERT(isEnableRecalc());

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ RECALC %1 with SR=%2-----")
                        .arg(_rsp->name())
                        .arg(_rsp->sampleRate()));

    //Getting the minimum and maximum value where recalculate the data
    quint64 n_dw=this->lowestSampleIndexForModification();
    quint64 n_up=this->highestSampleIndexForModification();
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ indexlow=%1/%2 indexhi=%3/%2")
                        .arg(n_dw)
                        .arg(getSampleNumber())
                        .arg(n_up));

    //Gettine time base
    const qreal *t=this->getTimeData();

    //Getting params
    //Getting params
    qreal _ampl=_rsp->amplitude();
    qreal _period=0.5/_rsp->frequency();
    qreal _mean=_rsp->meanValue();
    qreal _hiVal=_mean+_ampl;
    qreal _lowVal=_mean-_ampl;
    for (quint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(insertSignalValue(n,qFloor(t[n]/_period) % 2 ? _lowVal : _hiVal));
    }
}


