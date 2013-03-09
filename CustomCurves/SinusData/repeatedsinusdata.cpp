#include "repeatedsinusdata.h"
#include "Repeatedsinusdata.h"
#include "genericsinusdata.h"



//---------- FRONTEND ----------
void RepeatedSinusData::init(qreal amplitude,qreal frequency, qreal initPhase) {
    RepeatedSinusParams* _derivedProp=new RepeatedSinusParams((QObject*)this);
    getDelegate()->replacePropertiesAndUI(dynamic_cast<DataUiHandlerProperty*>(_derivedProp),
                                      dynamic_cast<DataUiHandlerUI*> (new RepeatedSinusUI() ));


    //Set any eventual default parameters or passed by the constructor argmuents
    RepeatedSinusParams* _rsp=dynamic_cast<RepeatedSinusParams*>(getDataParameters());
    Q_ASSERT(_rsp);
    _rsp->setAmplitude(amplitude);
    _rsp->setFrequency(frequency);
    _rsp->setInitPhase(initPhase);
    connectSignals();
}

void RepeatedSinusData::connectSignals() {
    RepeatedTimeData::connectSignals();

    RepeatedSinusParams* _rsp=dynamic_cast<RepeatedSinusParams*>(getDataParameters());
    Q_ASSERT(_rsp);
    Q_ASSERT(connect(_rsp,SIGNAL(amplitudeChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_rsp,SIGNAL(frequencyChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_rsp,SIGNAL(initPhaseChanged(qreal)),this,SLOT(updateData())));
}

void RepeatedSinusData::recalc() {
    RepeatedSinusParams* _rsp=dynamic_cast<RepeatedSinusParams*>(getDataParameters());
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
    qreal _ampl=_rsp->amplitude();
    qreal _freq=_rsp->frequency();
    qreal _phase=M_RADIANS(_rsp->initPhase());
    for (quint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(insertSignalValue(n,_ampl*sin(2*M_PI*_freq*t[n]+_phase)));
    }
}


