#include "dataenvelope.h"

DataEnvelope::DataEnvelope(GenericTimeData *parent, QWidget *widget):
    QObject((QObject*)parent),
    m_envelopeParams(new DataEnvelopeParameters((QObject*)parent)),
    m_genericTimeData(parent),
    m_TotalLength(0),
    m_envelope(NULL),
    m_SR(TIMEDATA_DEFAULT_SR)
{
    init(widget);
}

DataEnvelope::DataEnvelope(qreal SR, GenericTimeData *parent, QWidget *widget) :
    QObject((QObject*)parent),
    m_envelopeParams(new DataEnvelopeParameters((QObject*)parent)),
    m_genericTimeData(parent),
    m_TotalLength(0),
    m_envelope(NULL),
    m_SR(SR)
{
    init(widget);
}

DataEnvelope::DataEnvelope(DataEnvelopeParameters *params, qint64 length, qreal SR, GenericTimeData *parent, QWidget *widget) :
    QObject((QObject*)parent),
    m_envelopeParams(new DataEnvelopeParameters(params)),
    m_genericTimeData(parent),
    m_envelope(NULL),
    m_SR(SR)
{   
    m_TotalLength=(length >0 ? length : 0);
    init(widget);
}

void DataEnvelope::init(QWidget *widget) {
    m_envelopeParams->setTimeLength(((qreal)m_TotalLength)/m_SR);
    m_envelopeUI=new DataEnvelopeUI(m_envelopeParams,widget);
    m_genericTimeData->getControlWidget()->addControlFrame((CustomCurveUI*)m_envelopeUI,"ENVELOPE control");
    this->connectingSignals();
    this->recalculateEnvelope();
}

DataEnvelope::~DataEnvelope() {
    if (m_envelope) free(m_envelope);
    if (m_envelopeParams) delete m_envelopeParams;
}

void DataEnvelope::setTotalLength(qint64 length) {
   // qDebug() << "DataEnvelope::setLength() called with len=" << length;
    if (length >=0 && length!=m_TotalLength) {
        m_TotalLength=length;
        m_envelopeParams->setTimeLength(((qreal)m_TotalLength)/m_SR);//This function already call recalcuateEnvelope()
    }
}

void DataEnvelope::setEnvelopeParams(DataEnvelopeParameters * params) {
 //   qDebug() << "DataEnvelope::setEnvelopeParams() called";
    if (params!=NULL) {
        m_envelopeParams=params;
        recalculateEnvelope();
    }
}

bool DataEnvelope::setEnvelopeParams(QDomNode &node) {
    bool _prevStatus=m_envelopeParams->blockSignals(true);
    bool retval=m_envelopeParams->setClassByDomData(node);
    m_envelopeParams->blockSignals(_prevStatus);
    recalculateEnvelope();
    forceRegenerateDomDocument();
    return retval;
}

void DataEnvelope::setEnvelopeParamsAndLength(DataEnvelopeParameters *params,qint64 length) {
    //qDebug() << "DataEnvelope::setEnvelopeParamsAndLength() called with len=" << length;
    bool _needRecalculate=false;
    if (params!=NULL) {
        m_envelopeParams=params;
        _needRecalculate=true;
    }
    if (length >=0 && length!=m_TotalLength) {
        m_TotalLength=length;
        m_envelopeParams->setTimeLength(((qreal)m_TotalLength)/m_SR);
        _needRecalculate=false;//The function above has already generated a recalculate envelope
    }
    if (_needRecalculate) recalculateEnvelope();
}

void DataEnvelope::amplitudeEnvelopeChanged() {
    this->recalculateEnvelope();
}

void DataEnvelope::timeEnvelopeChanged() {
    this->recalculateEnvelope();
}

void DataEnvelope::setSampleRate(qreal SR) {
    m_SR=SR;
    m_envelopeParams->setTimeLength(((qreal)m_TotalLength)/SR);
}

void DataEnvelope::connectingSignals() {
    //Connect the parameter modification to the slot of this instance
    connect(m_envelopeParams,SIGNAL(amplitudeParametersChanged()),this,SLOT(amplitudeEnvelopeChanged()));
    connect(m_envelopeParams,SIGNAL(timeParametersChanged()),this,SLOT(timeEnvelopeChanged()) );

    //Connect this class to the UI
    connect(m_envelopeParams,SIGNAL(enableToggled(bool)), this, SIGNAL(enableToggled(bool)));
    connect(m_envelopeParams,SIGNAL(enableToggled(bool)), m_envelopeUI, SLOT(setEnableEnvelopeUI(bool)));
}

void DataEnvelope::recalculateEnvelope() {
    qDebug() << "DataEnvelope::recalculateEnvelope() for "<< parent()->metaObject()->className() <<" RECALCULATE ENVELOPE ------------- " ;
    //delete old envelope
    if (m_envelope) free(m_envelope);

    //init new envelope
    m_envelope=new qreal[m_TotalLength];
    memset(m_envelope,0,sizeof(qreal)*m_TotalLength);
    qDebug() << "DataEnvelope::recalculateEnvelope() length=" << m_TotalLength << " m_envelopeParams->total()=" << m_envelopeParams->total() <<" SR="<< m_SR << "(m_envelopeParams->total()*m_SR )=" <<(m_envelopeParams->total()*m_SR);
    //calculate envelope time
    qint64 _attack=(qint64)(m_envelopeParams->attackPercentile()*m_TotalLength);
    qint64 _hold=(qint64)(m_envelopeParams->holdPercentile()*m_TotalLength);
    qint64 _decay=(qint64)(m_envelopeParams->decayPercentile()*m_TotalLength);
    qint64 _sustain=(qint64)(m_envelopeParams->sustainPercentile()*m_TotalLength);
    qint64 _release=(qint64)(m_envelopeParams->releasePercentile()*m_TotalLength);

    Q_ASSERT( qRound64( m_TotalLength)==qRound64(m_envelopeParams->total()*m_SR) );
    Q_ASSERT( (_attack+_hold+_decay+_sustain+_release)<=m_TotalLength);//Better if Should be equal or is enough assure is minor? (condiser aproximation).

    qint64 _index=0;
    //Attack phase
    if (_attack>0) {
        qreal _attackSlope=m_envelopeParams->holdLevel()/((qreal) _attack);
        for (; _index< _attack; _index++) {
            m_envelope[_index]=_attackSlope*(qreal)_index;
        }
    }

    //Hold phase
    if (_hold>0) {
        qreal _holdlevel=m_envelopeParams->holdLevel();
        qint64 _endholdtime=_index+_hold;
        for (; _index< _endholdtime ; _index++) {
            m_envelope[_index]=_holdlevel;
        }
    }

    //decay phase
    if (_decay>0) {
        qreal _decayslope=(m_envelopeParams->sustainLevel()-m_envelopeParams->holdLevel())/((qreal) _decay);
        qint64 _enddecaytime=_index+_decay;
        qreal _startdecayQ=m_envelopeParams->sustainLevel()-_decayslope*_enddecaytime;
        for (; _index< _enddecaytime ; _index++) {
            m_envelope[_index]=_decayslope*(qreal)_index+_startdecayQ;
        }
    }

    //Sustain phase
    if (_sustain>0) {
        qreal _sustainlevel=m_envelopeParams->sustainLevel();
        qint64 _endsustaintime=_index+_sustain;
        for (; _index< _endsustaintime ; _index++) {
            m_envelope[_index]=_sustainlevel;
        }
    }

    //release phase
    if (_release>0) {
        qreal _releaseslope=(0-m_envelopeParams->sustainLevel())/((qreal) _release);
        qint64 _endreleasetime=_index+_release;
        qreal _startreleaseQ=0-_releaseslope*_endreleasetime;
        Q_ASSERT(_endreleasetime <= m_TotalLength);
        for (; _index< _endreleasetime ; _index++) {
            m_envelope[_index]=_releaseslope*(qreal)_index+_startreleaseQ;
        }
    }
    emit (envelopeChanged());
}
