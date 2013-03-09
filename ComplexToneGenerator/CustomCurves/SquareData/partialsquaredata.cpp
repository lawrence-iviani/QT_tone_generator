#include "partialsquaredata.h"


//---------- FRONTEND ----------
void PartialSquareData::init(qreal amplitude,qreal frequency, qreal meanValue) {
    PartialSquareParams* _derivedProp=new PartialSquareParams((QObject*)this);
    getDelegate()->replacePropertiesAndUI(dynamic_cast<DataUiHandlerProperty*>(_derivedProp),
                                      dynamic_cast<DataUiHandlerUI*> (new PartialSquareUI() ));

    //Set any eventual default parameters or passed by the constructor argmuents
    PartialSquareParams* _psp=dynamic_cast<PartialSquareParams*>(getDataParameters());
    Q_ASSERT(_psp);
    _psp->setAmplitude(amplitude);
    _psp->setFrequency(frequency);
    _psp->setMeanValue(meanValue);
    connectSignals();
}

void PartialSquareData::connectSignals() {
    PartialTimeData::connectSignals();
    PartialSquareParams* _psp=dynamic_cast<PartialSquareParams*>(getDataParameters());
    Q_ASSERT(_psp);
    Q_ASSERT(connect(_psp,SIGNAL(amplitudeChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_psp,SIGNAL(frequencyChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_psp,SIGNAL(meanValueChanged(qreal)),this,SLOT(updateData())));
}

void PartialSquareData::recalc() {
    PartialSquareParams* _psp=dynamic_cast<PartialSquareParams*>(getDataParameters());
    Q_ASSERT(_psp);

    //Verify going to recalc with the enable flag true
    Q_ASSERT(isEnableRecalc());

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ RECALC %1 with SR=%2-----")
                        .arg(_psp->name())
                        .arg(_psp->sampleRate()));

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
    qreal _ampl=_psp->amplitude();
    qreal _period=1.0/_psp->frequency();
    qreal _mean=_psp->meanValue();
    qreal _hiVal=_mean+_ampl;
    qreal _lowVal=_mean-_ampl;
    for (quint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(insertSignalValue(n,qFloor(t[n]/_period) % 2 ? _lowVal : _hiVal));
    }
}


