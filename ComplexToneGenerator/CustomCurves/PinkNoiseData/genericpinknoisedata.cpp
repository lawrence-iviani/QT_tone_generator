#include "genericpinknoisedata.h"

//---------- FRONTEND ----------
void GenericPinkNoiseData::init(qreal average, qreal gain) {
    GenericPinkNoiseParams* _derivedProp=new GenericPinkNoiseParams((QObject*)this);
    getDelegate()->replacePropertiesAndUI(dynamic_cast<DataUiHandlerProperty*>(_derivedProp),
                                      dynamic_cast<DataUiHandlerUI*> (new GenericPinkNoiseUI() ));

    //Set any eventual default parameters or passed by the constructor argmuents
    GenericPinkNoiseParams* _gpnp=dynamic_cast<GenericPinkNoiseParams*>(getDataParameters());
    Q_ASSERT(_gpnp);
    _gpnp->setAverage(average);
    _gpnp->setGain(gain);
    connectSignals();
}

void GenericPinkNoiseData::connectSignals() {
    GenericTimeData::connectSignals();
    GenericPinkNoiseParams* _gpnp=dynamic_cast<GenericPinkNoiseParams*>(getDataParameters());
    Q_ASSERT(_gpnp);
    Q_ASSERT(connect(_gpnp,SIGNAL(averageChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_gpnp,SIGNAL(gainChanged(qreal)),this,SLOT(updateData())));
}

void GenericPinkNoiseData::recalc() {
    GenericPinkNoiseParams* _gpnp=dynamic_cast<GenericPinkNoiseParams*>(getDataParameters());
    Q_ASSERT(_gpnp);

    //Verify going to recalc with the enable flag true
    Q_ASSERT(isEnableRecalc());

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ RECALC %1 with SR=%2-----")
                        .arg(_gpnp->name())
                        .arg(_gpnp->sampleRate()));

    //Getting the minimum and maximum value where recalculate the data
    quint64 n_dw=this->lowestSampleIndexForModification();
    quint64 n_up=this->highestSampleIndexForModification();
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ indexlow=%1/%2 indexhi=%3/%2")
                        .arg(n_dw)
                        .arg(getSampleNumber())
                        .arg(n_up));

    qreal _average=_gpnp->average();
    qreal _gain=_gpnp->gain();
    for (quint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(insertSignalValue(n,_gain*m_generator.generatePinkValue()+_average));
    }
}



