#include "dataenvelope.h"
#define ENV_EXP(x) pow(10000,x)
#define ENV_EXP_INV(x) pow(0.00001,x)
#define ENV_LOG(x) log(x)

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
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"Envelope has changed, recalc envelope"));
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
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"Envelope has toggled, envelope is %1")
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
    Q_ASSERT(connect(_ep,SIGNAL(envelopeTypeChanged(QString)),this,SLOT(envelopeHasChanged())));

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



    if (m_sampleNumber==0) {
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"Zero length envelope!"));
        return;
    }

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"sampleNumber=%1 SR=%2 Hz m_envelopeParams->total()=%3 m_envelopeParams->total()*m_SR=%4 equals qRound64( m_length*m_SR)=%5 ?")
                      .arg(m_sampleNumber)
                      .arg(m_SR)
                      .arg(_ep->totalTimeLength())
                      .arg(_ep->totalTimeLength()*m_SR)
                      .arg(qRound64(_ep->totalTimeLength()*m_SR)));

    //Verify length
    Q_ASSERT( m_sampleNumber==(quint64)qRound64(_ep->totalTimeLength()*m_SR) );

    //calculate envelope time

    switch (DataEnvelope::envelopeType2Enum( _ep->envelopeType())) {
    case Linear:
        recalculateLinearEnvelope((quint64)(_ep->attackPercentile()*m_sampleNumber),
                                (quint64)(_ep->holdPercentile()*m_sampleNumber),
                                (quint64)(_ep->decayPercentile()*m_sampleNumber),
                                (quint64)(_ep->sustainPercentile()*m_sampleNumber),
                                (quint64)(_ep->releasePercentile()*m_sampleNumber));
        break;
    case Exponential:
        recalculateExponentialEnvelope((quint64)(_ep->attackPercentile()*m_sampleNumber),
                                (quint64)(_ep->holdPercentile()*m_sampleNumber),
                                (quint64)(_ep->decayPercentile()*m_sampleNumber),
                                (quint64)(_ep->sustainPercentile()*m_sampleNumber),
                                (quint64)(_ep->releasePercentile()*m_sampleNumber));
        break;
    case InvertedExp:
        recalculateExponentialEnvelope((quint64)(_ep->attackPercentile()*m_sampleNumber),
                                (quint64)(_ep->holdPercentile()*m_sampleNumber),
                                (quint64)(_ep->decayPercentile()*m_sampleNumber),
                                (quint64)(_ep->sustainPercentile()*m_sampleNumber),
                                (quint64)(_ep->releasePercentile()*m_sampleNumber),true);
        break;
    case Logarithmic:
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"Not yet implemented log envelope"));
        break;
    default:
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"Unknown %1 envelope").arg(_ep->envelopeType()));
    }

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"END RECALCULATE ENVELOPE -- for %1 ").arg(parent()->metaObject()->className()));
    emit (envelopeChanged());
}

void DataEnvelope::exponetialCoeff(qreal& slope, qreal& c, const qreal& x1, const qreal& y1, const qreal& x2, const qreal& y2,bool inverted) {
    qDebug() << Q_FUNC_INFO << "x1="<<x1 << "y1="<<y1 << "x2="<<x2 << "y2="<< y2;
    qreal _deltaY=y2-y1;
    qreal _deltaX=x2-x1;
    if (inverted)
        slope=(_deltaY > 0.0 ? (_deltaY)/(ENV_EXP_INV(_deltaX)-1.0) : (_deltaY)/(ENV_EXP(_deltaX)-1.0 ) );
    else
        slope=(_deltaY > 0.0 ? (_deltaY)/(ENV_EXP(_deltaX)-1.0) : (_deltaY)/(ENV_EXP_INV(_deltaX)-1.0 ) );
    c=y1-slope;
    qDebug() << Q_FUNC_INFO << "slope=" << slope << "c="<< c;
}

void DataEnvelope::recalculateExponentialEnvelope(quint64 attack, quint64 hold, quint64 decay, quint64 sustain, quint64 release, bool invertedExponential) {
    DataEnvelopeParameters *_ep=dynamic_cast<DataEnvelopeParameters*> (m_envelopeDelegate->getProperty());
    Q_ASSERT(_ep!=NULL);

    //Test total envelope time length in sample (make sure aproximation doesn't leads to error)
    Q_ASSERT( (attack+hold+decay+sustain+release)<=m_sampleNumber);//Better if Should be equal or is enough assure is minor? (condiser aproximation).

    quint64 _index=0;
    qreal _nSample=(qreal)m_sampleNumber;
    qreal _holdlevel=_ep->holdLevel();
    qreal _sustainlevel=_ep->sustainLevel();
    //Attack phase
    if (attack>0) {
        qreal _attackSlope;
        qreal _c;
        exponetialCoeff(_attackSlope,_c,0.0,0.0,((qreal)attack)/_nSample,_holdlevel,invertedExponential);
        for (; _index< attack; _index++) {
            m_envelope[_index]=invertedExponential ? ((qreal)_attackSlope*ENV_EXP_INV((qreal)(_index/_nSample)))+_c:((qreal)_attackSlope*ENV_EXP((qreal)(_index/_nSample)))+_c;
        }
    }

    //Hold phase
    if (hold>0) {
        quint64 _endholdtime=_index+hold;
        for (; _index< _endholdtime ; _index++) {
            m_envelope[_index]=_holdlevel;
        }
    }

    //decay phase
    if (decay>0) {
        qreal _decayslope;//=(_ep->sustainLevel()-_ep->holdLevel())/((qreal) decay);
        qreal _c;
        quint64 _endDecayIndex=_index+decay;
        quint64 _startDecayIndex=_index;
        //exponetialCoeff(_decayslope,_c,(qreal)(_index)/_sample,_holdlevel,((qreal)(_enddecaytime))/_sample,_sustainlevel);
        exponetialCoeff(_decayslope,_c,(qreal)(_index)/_nSample,_holdlevel,((qreal)(_endDecayIndex))/_nSample,_sustainlevel,invertedExponential);
        if (_sustainlevel > _holdlevel)
            for (; _index< _endDecayIndex ; _index++) {
                qreal _sample=((qreal)(_index-_startDecayIndex))/_nSample;
                m_envelope[_index]=invertedExponential ? (_decayslope*ENV_EXP_INV(_sample))+_c:(_decayslope*ENV_EXP(_sample))+_c;
            }
        else
            for (; _index< _endDecayIndex ; _index++) {
                qreal _sample=((qreal)(_index-_startDecayIndex))/_nSample;
                m_envelope[_index]=invertedExponential ? (_decayslope*ENV_EXP(_sample))+_c:(_decayslope*ENV_EXP_INV(_sample))+_c;
            }
    }

    //Sustain phase
    if (sustain>0) {

        quint64 _endsustaintime=_index+sustain;
        for (; _index< _endsustaintime ; _index++) {
            m_envelope[_index]=_sustainlevel;
        }
    }

    //release phase
    if (release>0) {
        qreal _releaseslope;
        qreal _c;
        quint64 _endreleasetime=_index+release;
        quint64 _startReleaseIndex=_index;
        exponetialCoeff(_releaseslope,_c,(qreal)(_index)/_nSample,_sustainlevel,((qreal)(_endreleasetime))/_nSample,0.0,invertedExponential);
        Q_ASSERT(_endreleasetime <= m_sampleNumber);
        for (; _index< _endreleasetime ; _index++) {
            qreal _sample=((qreal)(_index-_startReleaseIndex))/_nSample;
            m_envelope[_index]=invertedExponential ? (_releaseslope*ENV_EXP(_sample))+_c:(_releaseslope*ENV_EXP_INV(_sample))+_c;
        }
    }
}

void DataEnvelope::recalculateLinearEnvelope(quint64 attack, quint64 hold, quint64 decay, quint64 sustain, quint64 release) {
    DataEnvelopeParameters *_ep=dynamic_cast<DataEnvelopeParameters*> (m_envelopeDelegate->getProperty());
    Q_ASSERT(_ep!=NULL);

    //Test total envelope time length in sample (make sure aproximation doesn't leads to error)
    Q_ASSERT( (attack+hold+decay+sustain+release)<=m_sampleNumber);//Better if Should be equal or is enough assure is minor? (condiser aproximation).

    quint64 _index=0;
    //Attack phase
    if (attack>0) {
        qreal _attackSlope=_ep->holdLevel()/((qreal) attack);
        for (; _index< attack; _index++) {
            m_envelope[_index]=_attackSlope*(qreal)_index;
        }
    }

    //Hold phase
    if (hold>0) {
        qreal _holdlevel=_ep->holdLevel();
        quint64 _endholdtime=_index+hold;
        for (; _index< _endholdtime ; _index++) {
            m_envelope[_index]=_holdlevel;
        }
    }

    //decay phase
    if (decay>0) {
        qreal _decayslope=(_ep->sustainLevel()-_ep->holdLevel())/((qreal) decay);
        quint64 _enddecaytime=_index+decay;
        qreal _startdecayQ=_ep->sustainLevel()-_decayslope*_enddecaytime;
        for (; _index< _enddecaytime ; _index++) {
            m_envelope[_index]=_decayslope*(qreal)_index+_startdecayQ;
        }
    }

    //Sustain phase
    if (sustain>0) {
        qreal _sustainlevel=_ep->sustainLevel();
        quint64 _endsustaintime=_index+sustain;
        for (; _index< _endsustaintime ; _index++) {
            m_envelope[_index]=_sustainlevel;
        }
    }

    //release phase
    if (release>0) {
        qreal _releaseslope=(0-_ep->sustainLevel())/((qreal) release);
        quint64 _endreleasetime=_index+release;
        qreal _startreleaseQ=0-_releaseslope*_endreleasetime;
        Q_ASSERT(_endreleasetime <= m_sampleNumber);
        for (; _index< _endreleasetime ; _index++) {
            m_envelope[_index]=_releaseslope*(qreal)_index+_startreleaseQ;
        }
    }

}

void DataEnvelope::logCoeff(qreal& slope, qreal& c, const qreal& x1, const qreal& y1, const qreal& x2, const qreal& y2) {
    qDebug() << Q_FUNC_INFO << "y1="<<y1 << "y2="<< y2<< "x1="<<x1 <<"x2="<<x2;
    slope=(y2-y1)/((qreal)(ENV_LOG(x2)-ENV_LOG(x1)));
    c=y2-slope*((qreal)ENV_LOG(x2));
    qDebug() << Q_FUNC_INFO << "slope="<<slope << "c="<< c;
}

void DataEnvelope::recalculateLogEnvelope(quint64 attack, quint64 hold, quint64 decay, quint64 sustain, quint64 release) {
    DataEnvelopeParameters *_ep=dynamic_cast<DataEnvelopeParameters*> (m_envelopeDelegate->getProperty());
    Q_ASSERT(_ep!=NULL);

    //Test total envelope time length in sample (make sure aproximation doesn't leads to error)
    Q_ASSERT( (attack+hold+decay+sustain+release)<=m_sampleNumber);//Better if Should be equal or is enough assure is minor? (condiser aproximation).

    quint64 _index=0;
    qreal _sample=(qreal)m_sampleNumber;
    qreal _holdlevel=_ep->holdLevel();
    qreal _sustainlevel=_ep->sustainLevel();
    //Attack phase
    if (attack>0) {
        qreal _attackSlope;
        qreal _c;
        logCoeff(_attackSlope,_c,0.0,0.0,((qreal)attack)/_sample,_holdlevel);
        for (; _index< attack; _index++) {
            m_envelope[_index]=((qreal)_attackSlope*ENV_LOG(((qreal)_index)/_sample))+  _c;
        }
    }

    //Hold phase
    if (hold>0) {
        quint64 _endholdtime=_index+hold;
        for (; _index< _endholdtime ; _index++) {
            m_envelope[_index]=_holdlevel;
        }
    }

    //decay phase
    if (decay>0) {
        qreal _decayslope;//=(_ep->sustainLevel()-_ep->holdLevel())/((qreal) decay);
        qreal _c;
        quint64 _enddecaytime=_index+decay;
        logCoeff(_decayslope,_c,(qreal)(_index)/_sample,_holdlevel,((qreal)(_enddecaytime))/_sample,_sustainlevel);
        for (; _index< _enddecaytime ; _index++) {
            m_envelope[_index]=((qreal)_decayslope*ENV_LOG(((qreal)_index)/_sample))+_c;
        }
    }

    //Sustain phase
    if (sustain>0) {

        quint64 _endsustaintime=_index+sustain;
        for (; _index< _endsustaintime ; _index++) {
            m_envelope[_index]=_sustainlevel;
        }
    }

    //release phase
    if (release>0) {
        qreal _releaseslope;
        qreal _c;
        quint64 _endreleasetime=_index+release;
        logCoeff(_releaseslope,_c,(qreal)(_index)/_sample,_sustainlevel,((qreal)(_endreleasetime))/_sample,0.0);
        Q_ASSERT(_endreleasetime <= m_sampleNumber);
        for (; _index< _endreleasetime ; _index++) {
            m_envelope[_index]=((qreal)_releaseslope*ENV_LOG(((qreal)_index)/_sample))+_c;
        }
    }
}

