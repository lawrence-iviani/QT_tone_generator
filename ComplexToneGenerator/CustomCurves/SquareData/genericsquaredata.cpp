#include "genericsquaredata.h"


//---------- FRONTEND ----------
void GenericSquareData::init(qreal amplitude, qreal frequency, qreal meanValue) {
    GenericSquareParams* _derivedProp=new GenericSquareParams((QObject*)this);
    getDelegate()->replacePropertiesAndUI(dynamic_cast<DataUiHandlerProperty*>(_derivedProp),
                                      dynamic_cast<DataUiHandlerUI*> (new GenericSquareUI() ));

    //Set any eventual default parameters or passed by the constructor argmuents
    GenericSquareParams* _gsp=dynamic_cast<GenericSquareParams*>(getDataParameters());
    Q_ASSERT(_gsp);
    _gsp->setAmplitude(amplitude);
    _gsp->setFrequency(frequency);
    _gsp->setMeanValue(meanValue);
    connectSignals();
}

void GenericSquareData::connectSignals() {
    GenericTimeData::connectSignals();
    GenericSquareParams* _gsp=dynamic_cast<GenericSquareParams*>(getDataParameters());
    Q_ASSERT(_gsp);
    Q_ASSERT(connect(_gsp,SIGNAL(amplitudeChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_gsp,SIGNAL(frequencyChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_gsp,SIGNAL(meanValueChanged(qreal)),this,SLOT(updateData())));

}

void GenericSquareData::recalc() {
    GenericSquareParams* _gsp=dynamic_cast<GenericSquareParams*>(getDataParameters());
    Q_ASSERT(_gsp);

    //Verify going to recalc with the enable flag true
    Q_ASSERT(isEnableRecalc());

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ RECALC %1 with SR=%2-----")
                        .arg(_gsp->name())
                        .arg(_gsp->sampleRate()));

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
    qreal _ampl=_gsp->amplitude();
    qreal _period=1.0/_gsp->frequency();
    qreal _mean=_gsp->meanValue();
    qreal _hiVal=_mean+_ampl;
    qreal _lowVal=_mean-_ampl;
    for (quint64 n=n_dw; n < n_up; n++) {     
        Q_ASSERT(insertSignalValue(n,qFloor(t[n]/_period) % 2 ? _lowVal : _hiVal));
    }
}



