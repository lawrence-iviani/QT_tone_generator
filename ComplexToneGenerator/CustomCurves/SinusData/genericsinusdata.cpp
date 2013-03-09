#include "genericsinusdata.h"


//---------- FRONTEND ----------

void GenericSinusData::init(qreal amplitude, qreal frequency, qreal initPhase) {
    GenericSinusParams* _derivedProp=new GenericSinusParams((QObject*)this);
    getDelegate()->replacePropertiesAndUI(dynamic_cast<DataUiHandlerProperty*>(_derivedProp),
                                      dynamic_cast<DataUiHandlerUI*> (new GenericSinusUI() ));

    //Set any eventual default parameters or passed by the constructor argmuents
    GenericSinusParams* _gsp=dynamic_cast<GenericSinusParams*>(getDataParameters());
    Q_ASSERT(_gsp);
    _gsp->setAmplitude(amplitude);
    _gsp->setFrequency(frequency);
    _gsp->setInitPhase(initPhase);
    connectSignals();
}

void GenericSinusData::connectSignals() {
    GenericTimeData::connectSignals();
    GenericSinusParams* _gsp=dynamic_cast<GenericSinusParams*>(getDataParameters());
    Q_ASSERT(_gsp);
    Q_ASSERT(connect(_gsp,SIGNAL(amplitudeChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_gsp,SIGNAL(frequencyChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_gsp,SIGNAL(initPhaseChanged(qreal)),this,SLOT(updateData())));
}

void GenericSinusData::recalc() {
    GenericSinusParams* _gsp=dynamic_cast<GenericSinusParams*>(getDataParameters());
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
    qreal _freq=_gsp->frequency();
    qreal _phase=M_RADIANS(_gsp->initPhase());
    for (quint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(insertSignalValue(n,M_SIN(_ampl,_freq,_phase,t[n])));//_ampl*sin(2*M_PI*_freq*t[n]+_phase)));
    }
}



