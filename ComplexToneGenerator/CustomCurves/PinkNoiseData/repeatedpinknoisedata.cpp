#include "repeatedpinknoisedata.h"

//---------- FRONTEND ----------
void RepeatedPinkNoiseData::init(qreal average, qreal gain) {
    RepeatedPinkNoiseParams* _derivedProp=new RepeatedPinkNoiseParams((QObject*)this);
    getDelegate()->replacePropertiesAndUI(dynamic_cast<DataUiHandlerProperty*>(_derivedProp),
                                      dynamic_cast<DataUiHandlerUI*> (new RepeatedPinkNoiseUI() ));

    //Set any eventual default parameters or passed by the constructor argmuents
    RepeatedPinkNoiseParams* _rpnp=dynamic_cast<RepeatedPinkNoiseParams*>(getDataParameters());
    Q_ASSERT(_rpnp);
    _rpnp->setAverage(average);
    _rpnp->setGain(gain);
    connectSignals();
}

void RepeatedPinkNoiseData::connectSignals() {
    RepeatedTimeData::connectSignals();
    RepeatedPinkNoiseParams* _rpnp=dynamic_cast<RepeatedPinkNoiseParams*>(getDataParameters());
    Q_ASSERT(_rpnp);
    Q_ASSERT(connect(_rpnp,SIGNAL(averageChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_rpnp,SIGNAL(gainChanged(qreal)),this,SLOT(updateData())));
}

void RepeatedPinkNoiseData::recalc() {
    RepeatedPinkNoiseParams* _rpnp=dynamic_cast<RepeatedPinkNoiseParams*>(getDataParameters());
    Q_ASSERT(_rpnp);

    //Verify going to recalc with the enable flag true
    Q_ASSERT(isEnableRecalc());

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ RECALC %1 with SR=%2-----")
                        .arg(_rpnp->name())
                        .arg(_rpnp->sampleRate()));

    //Getting the minimum and maximum value where recalculate the data
    quint64 n_dw=this->lowestSampleIndexForModification();
    quint64 n_up=this->highestSampleIndexForModification();
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ indexlow=%1/%2 indexhi=%3/%2")
                        .arg(n_dw)
                        .arg(getSampleNumber())
                        .arg(n_up));

    qreal _average=_rpnp->average();
    qreal _gain=_rpnp->gain();
    for (quint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(insertSignalValue(n,_gain*m_generator.generatePinkValue()+_average));
    }
}
