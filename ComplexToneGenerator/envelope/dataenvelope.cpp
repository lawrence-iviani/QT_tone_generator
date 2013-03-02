#include "dataenvelope.h"

DataEnvelope::DataEnvelope(QObject *parent):
    QObject(parent),
    //m_genericTimeData(parent),
    m_sampleNumber(0),
    m_envelope(NULL),
    m_SR(TIMEDATA_DEFAULT_SR)
{  init((QObject*)parent); this->envelopeHasChanged();  }

DataEnvelope::DataEnvelope(qreal SR, QObject *parent) :
    QObject(parent),
   // m_genericTimeData(parent),
    m_sampleNumber(0),
    m_envelope(NULL),
    m_SR(SR)
{
    init((QObject*)parent);
    DataEnvelopeParameters *_ep=dynamic_cast<DataEnvelopeParameters*> (getDataParameters());
    Q_ASSERT(_ep!=NULL);
    _ep->setTimeLength(0);
    this->envelopeHasChanged();
}

DataEnvelope::DataEnvelope(quint64 length, qreal SR, QObject *parent) :
    QObject(parent),
   // m_genericTimeData(parent),
    m_envelope(NULL),
    m_SR(SR)
{   
    init((QObject*)parent);
    setSampleNumber(length);
    this->envelopeHasChanged();
}

void DataEnvelope::init(QObject *parent) {
    m_envelopeDelegate=new DataUiHandlerDelegate(
                dynamic_cast<DataUiHandlerProperty*> (new DataEnvelopeParameters(parent)),
                dynamic_cast<DataUiHandlerUI*>(new DataEnvelopeUI()),
                ENVELOPE_DOCTYPE,
                ENVELOPE_TAG,
                ENVELOPE_PARAMSVERSION,
                ENVELOPE_SUFFIX,
                parent);
    this->connectingSignals();

}

DataEnvelope::~DataEnvelope() {
    if (m_envelope) free(m_envelope);
    if (m_envelopeDelegate) delete m_envelopeDelegate;
}

void DataEnvelope::envelopeHasChanged() {
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"Envelope has changed, recalc envelope"));
    DataEnvelopeParameters *_ep=dynamic_cast<DataEnvelopeParameters*> (m_envelopeDelegate->getProperty());
    Q_ASSERT(_ep!=NULL);
    DataEnvelopeUI *_epUI=dynamic_cast<DataEnvelopeUI*> (m_envelopeDelegate->getUI());
    Q_ASSERT(_epUI!=NULL);
    _epUI->setParamsTime(_ep);
    recalculateEnvelope();
}

void DataEnvelope::envelopeHasToggledEnable() {
    DataEnvelopeParameters *_ep=dynamic_cast<DataEnvelopeParameters*> (m_envelopeDelegate->getProperty());
    Q_ASSERT(_ep!=NULL);
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"Envelope has toggled, envelope is %1")
                      .arg(_ep->isEnabledEnvelope() ? "enabled" : "disabled" ));
    emit (envelopeChanged());
}

void DataEnvelope::connectingSignals() {
    DataEnvelopeParameters *_ep=dynamic_cast<DataEnvelopeParameters*> (m_envelopeDelegate->getProperty());
    Q_ASSERT(_ep!=NULL);
    DataEnvelopeUI *_epUI=dynamic_cast<DataEnvelopeUI*> (m_envelopeDelegate->getUI());
    Q_ASSERT(_epUI!=NULL);

    //Connect the parameter modification to the slot of this instance
    Q_ASSERT(connect(_ep,SIGNAL(amplitudeParametersChanged()),this,SLOT(envelopeHasChanged())));
    Q_ASSERT(connect(_ep,SIGNAL(timeParametersChanged()),this,SLOT(envelopeHasChanged())));

    //Connect this class to the UI
    Q_ASSERT(connect(_ep,SIGNAL(enableEnvelopeChanged(bool)), this, SLOT(envelopeHasToggledEnable())));

    //If for some reason a set time params failed it's necessary restore the value in the params
    Q_ASSERT(connect(_ep,SIGNAL(timeParamsChanged(DataEnvelopeParameters*)), _epUI, SLOT(setParamsTime(DataEnvelopeParameters*))));
}

void DataEnvelope::replacePropertyAndUI(DataUiHandlerProperty *params, DataUiHandlerUI *ui) {
    Q_ASSERT(params);
    Q_ASSERT(ui);
    m_envelopeDelegate->replacePropertiesAndUI(params,ui);
}

void DataEnvelope::setSampleNumber(quint64 length) {
    DataEnvelopeParameters *_ep=dynamic_cast<DataEnvelopeParameters*> (m_envelopeDelegate->getProperty());
    Q_ASSERT(_ep!=NULL);  
    DataEnvelopeUI *_epUI=dynamic_cast<DataEnvelopeUI*> (m_envelopeDelegate->getUI());
    Q_ASSERT(_epUI!=NULL);

    if (length!=m_sampleNumber) {
        m_sampleNumber=length;
        _ep->setTimeLength(((qreal)m_sampleNumber)/m_SR);//This function already call recalcuateEnvelope()
        _epUI->setParamsTime(_ep);
        recalculateEnvelope();
    }
}

void DataEnvelope::setSampleRate(qreal SR) {
    DataEnvelopeParameters *_ep=dynamic_cast<DataEnvelopeParameters*> (m_envelopeDelegate->getProperty());
    Q_ASSERT(_ep!=NULL);
    DataEnvelopeUI *_epUI=dynamic_cast<DataEnvelopeUI*> (m_envelopeDelegate->getUI());
    Q_ASSERT(_epUI!=NULL);

    m_SR=SR;
}

void DataEnvelope::setSampleRateAndSampleNumber(qreal SR, quint64 length) {
    setSampleRate(SR);
    setSampleNumber(length);
}

void DataEnvelope::recalculateEnvelope() {
    DataEnvelopeParameters *_ep=dynamic_cast<DataEnvelopeParameters*> (m_envelopeDelegate->getProperty());
    Q_ASSERT(_ep!=NULL);
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"RECALCULATE ENVELOPE -- for %1 ").arg(parent()->metaObject()->className()));
    //delete old envelope
    if (m_envelope) free(m_envelope);

    //init new envelope
    m_envelope=new qreal[m_sampleNumber];
    memset(m_envelope,0,sizeof(qreal)*m_sampleNumber);

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"sampleNumber=%1 SR=%2 Hz m_envelopeParams->total()=%3 m_envelopeParams->total()*m_SR=%4 ")
                      .arg(m_sampleNumber)
                      .arg(m_SR)
                      .arg(_ep->totalTimeLength())
                      .arg(_ep->totalTimeLength()*m_SR));

    if (m_sampleNumber==0) {
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"Zero length envelope!"));
        return;
    }
    //calculate envelope time
    quint64 _attack=(quint64)(_ep->attackPercentile()*m_sampleNumber);
    quint64 _hold=(quint64)(_ep->holdPercentile()*m_sampleNumber);
    quint64 _decay=(quint64)(_ep->decayPercentile()*m_sampleNumber);
    quint64 _sustain=(quint64)(_ep->sustainPercentile()*m_sampleNumber);
    quint64 _release=(quint64)(_ep->releasePercentile()*m_sampleNumber);

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"qRound64( m_length)=%1 qRound64(m_envelopeParams->total()*m_SR)=%2, equals? %3")
                        .arg(m_sampleNumber)
                        .arg(qRound64(_ep->totalTimeLength()*m_SR))
                        .arg(qRound64(_ep->totalTimeLength()*m_SR)==qRound64(_ep->totalTimeLength()*m_SR)  ));


    Q_ASSERT( m_sampleNumber==(quint64)qRound64(_ep->totalTimeLength()*m_SR) );
    Q_ASSERT( (_attack+_hold+_decay+_sustain+_release)<=m_sampleNumber);//Better if Should be equal or is enough assure is minor? (condiser aproximation).

    quint64 _index=0;
    //Attack phase
    if (_attack>0) {
        qreal _attackSlope=_ep->holdLevel()/((qreal) _attack);
        for (; _index< _attack; _index++) {
            m_envelope[_index]=_attackSlope*(qreal)_index;
        }
    }

    //Hold phase
    if (_hold>0) {
        qreal _holdlevel=_ep->holdLevel();
        quint64 _endholdtime=_index+_hold;
        for (; _index< _endholdtime ; _index++) {
            m_envelope[_index]=_holdlevel;
        }
    }

    //decay phase
    if (_decay>0) {
        qreal _decayslope=(_ep->sustainLevel()-_ep->holdLevel())/((qreal) _decay);
        quint64 _enddecaytime=_index+_decay;
        qreal _startdecayQ=_ep->sustainLevel()-_decayslope*_enddecaytime;
        for (; _index< _enddecaytime ; _index++) {
            m_envelope[_index]=_decayslope*(qreal)_index+_startdecayQ;
        }
    }

    //Sustain phase
    if (_sustain>0) {
        qreal _sustainlevel=_ep->sustainLevel();
        quint64 _endsustaintime=_index+_sustain;
        for (; _index< _endsustaintime ; _index++) {
            m_envelope[_index]=_sustainlevel;
        }
    }

    //release phase
    if (_release>0) {
        qreal _releaseslope=(0-_ep->sustainLevel())/((qreal) _release);
        quint64 _endreleasetime=_index+_release;
        qreal _startreleaseQ=0-_releaseslope*_endreleasetime;
        Q_ASSERT(_endreleasetime <= m_sampleNumber);
        for (; _index< _endreleasetime ; _index++) {
            m_envelope[_index]=_releaseslope*(qreal)_index+_startreleaseQ;
        }
    }
    emit (envelopeChanged());
}

//void DataEnvelope::setEnvelopeParams(DataEnvelopeParameters * params) {
// //   qDebug() << "DataEnvelope::setEnvelopeParams() called";
//    if (params!=NULL) {
//        m_envelopeParams=params;
//        recalculateEnvelope();
//    }
//}

//bool DataEnvelope::setEnvelopeParams(QDomNode &node) {
//    bool _prevStatus=m_envelopeParams->blockSignals(true);
//    bool retval=m_envelopeParams->setClassByDomData(node);
//    m_envelopeParams->blockSignals(_prevStatus);
//    recalculateEnvelope();
//    forceRegenerateDomDocument();
//    return retval;
//}

//void DataEnvelope::setEnvelopeParamsAndLength(DataEnvelopeParameters *params,qint64 length) {
//    //qDebug() << "DataEnvelope::setEnvelopeParamsAndLength() called with len=" << length;
//    bool _needRecalculate=false;
//    if (params!=NULL) {
//        m_envelopeParams=params;
//        _needRecalculate=true;
//    }
//    if (length >=0 && length!=m_length) {
//        m_length=length;
//        m_envelopeParams->setTimeLength(((qreal)m_length)/m_SR);
//        _needRecalculate=false;//The function above has already generated a recalculate envelope
//    }
//    if (_needRecalculate) recalculateEnvelope();
//}

//void DataEnvelope::amplitudeEnvelopeChanged() {
//    this->recalculateEnvelope();
//}

//void DataEnvelope::timeEnvelopeChanged() {
//    this->recalculateEnvelope();
//}
