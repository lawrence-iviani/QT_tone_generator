#include "partialpinknoisedata.h"

//---------- FRONTEND ----------
void PartialPinkNoiseData::init(qreal average, qreal gain) {
    PartialPinkNoiseParams* _derivedProp=new PartialPinkNoiseParams((QObject*)this);
    getDelegate()->replacePropertiesAndUI(dynamic_cast<DataUiHandlerProperty*>(_derivedProp),
                                      dynamic_cast<DataUiHandlerUI*> (new PartialPinkNoiseUI() ));

    //Set any eventual default parameters or passed by the constructor argmuents
    PartialPinkNoiseParams* _ppnp=dynamic_cast<PartialPinkNoiseParams*>(getDataParameters());
    Q_ASSERT(_ppnp);
    _ppnp->setAverage(average);
    _ppnp->setGain(gain);
    connectSignals();
}

void PartialPinkNoiseData::connectSignals() {
    PartialTimeData::connectSignals();
    PartialPinkNoiseParams* _ppnp=dynamic_cast<PartialPinkNoiseParams*>(getDataParameters());
    Q_ASSERT(_ppnp);
    Q_ASSERT(connect(_ppnp,SIGNAL(averageChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_ppnp,SIGNAL(gainChanged(qreal)),this,SLOT(updateData())));
}

void PartialPinkNoiseData::recalc() {
    PartialPinkNoiseParams* _ppnp=dynamic_cast<PartialPinkNoiseParams*>(getDataParameters());
    Q_ASSERT(_ppnp);

    //Verify going to recalc with the enable flag true
    Q_ASSERT(isEnableRecalc());

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ RECALC %1 with SR=%2-----")
                        .arg(_ppnp->name())
                        .arg(_ppnp->sampleRate()));

    //Getting the minimum and maximum value where recalculate the data
    quint64 n_dw=this->lowestSampleIndexForModification();
    quint64 n_up=this->highestSampleIndexForModification();
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ indexlow=%1/%2 indexhi=%3/%2")
                        .arg(n_dw)
                        .arg(getSampleNumber())
                        .arg(n_up));

    qreal _average=_ppnp->average();
    qreal _gain=_ppnp->gain();
    for (quint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(insertSignalValue(n,_gain*m_generator.generatePinkValue()+_average));
    }
}
