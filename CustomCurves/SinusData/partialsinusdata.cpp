#include "partialsinusdata.h"
#include "genericsinusdata.h"


//---------- FRONTEND ----------
void PartialSinusData::init(qreal amplitude,qreal frequency, qreal initPhase) {
    PartialSinusParams* _derivedProp=new PartialSinusParams((QObject*)this);
    getDelegate()->replacePropertiesAndUI(dynamic_cast<DataUiHandlerProperty*>(_derivedProp),
                                      dynamic_cast<DataUiHandlerUI*> (new PartialSinusUI() ));

    //Set any eventual default parameters or passed by the constructor argmuents
    PartialSinusParams* _psp=dynamic_cast<PartialSinusParams*>(getDataParameters());
    Q_ASSERT(_psp);
    _psp->setAmplitude(amplitude);
    _psp->setFrequency(frequency);
    _psp->setInitPhase(initPhase);
    connectSignals();
}

void PartialSinusData::connectSignals() {
    PartialTimeData::connectSignals();

    PartialSinusParams* _psp=dynamic_cast<PartialSinusParams*>(getDataParameters());
    Q_ASSERT(_psp);
    Q_ASSERT(connect(_psp,SIGNAL(amplitudeChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_psp,SIGNAL(frequencyChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_psp,SIGNAL(initPhaseChanged(qreal)),this,SLOT(updateData())));
}

void PartialSinusData::recalc() {
    PartialSinusParams* _psp=dynamic_cast<PartialSinusParams*>(getDataParameters());
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
    qreal _freq=_psp->frequency();
    qreal _phase=M_RADIANS(_psp->initPhase());
    for (quint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(insertSignalValue(n,M_SIN(_ampl,_freq,_phase,t[n])));
    }
}


