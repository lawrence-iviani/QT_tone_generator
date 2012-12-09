#include "dataenvelopeparameters.h"
#include <QDebug>

DataEnvelopeParameters::DataEnvelopeParameters(QObject *parent) :
        QObject(parent),
        DomHelper(this)
    {
        m_total=5.0;
        this->setTimeParameters(1.0,1.0,1.0,1.0,1.0);
        m_holdLevel=DATAENVELOPE_DEFAULT_HOLDLEVEL;
        m_sustainLevel=DATAENVELOPE_DEFAULT_SUSTAINLEVEL;
        m_enable=false;
        connectSignals();
    }

DataEnvelopeParameters::DataEnvelopeParameters(qreal attack, qreal hold, qreal decay ,qreal sustain, qreal release, QObject *parent) :
        QObject(parent),
        DomHelper(this)
    {
        m_total=attack+hold+decay+sustain+release;
        this->setTimeParameters(attack,hold,decay,sustain,release);
        m_holdLevel=DATAENVELOPE_DEFAULT_HOLDLEVEL;
        m_sustainLevel=DATAENVELOPE_DEFAULT_SUSTAINLEVEL;
        m_enable=false;
        connectSignals();
    }

void  DataEnvelopeParameters::connectSignals() {
    connect(this,SIGNAL(amplitudeParametersChanged()),this,SLOT(regenerateDomDocument()));
    connect(this,SIGNAL(timeParametersChanged()),this,SLOT(regenerateDomDocument()));
    connect(this,SIGNAL(enableToggled(bool)),this,SLOT(regenerateDomDocument()));
}

void DataEnvelopeParameters::setEnableEnvelope(bool enable) {
    if(m_enable!=enable) {
        m_enable=enable;
        emit (enableToggled(enable));
    }
}

void DataEnvelopeParameters::setTimeLength(qreal length) {

    qreal _attackPercentile;
    qreal _holdPercentile;
    qreal _decayPercentile;
    qreal _sustainPercentile;
    qreal _releasePercentile;

    //If the total previous length was 0, update with the previous percentile
    //Other get the actual percentile
    if (m_total==0) {
        _attackPercentile=m_previousAttackPercentile;
        _holdPercentile=m_previousholdPercentile;
        _decayPercentile=m_previousdecayPercentile;
        _sustainPercentile=m_previoussustainPercentile;
        _releasePercentile=m_previousreleasePercentile;
    } else {
        _attackPercentile=attackPercentile();
        _holdPercentile=holdPercentile();
        _decayPercentile=decayPercentile();
        _sustainPercentile=sustainPercentile();
        _releasePercentile=releasePercentile();
    }

    m_total=(length>=0.0 ? length : 0.0);

    if (m_total==0.0) {
        _attackPercentile=(_attackPercentile<=0.0 ? m_previousAttackPercentile : _attackPercentile);
        _holdPercentile=(_holdPercentile<=0.0 ? m_previousholdPercentile : _holdPercentile);
        _decayPercentile=(_decayPercentile<=0.0 ? m_previousdecayPercentile : _decayPercentile);
        _sustainPercentile=(_sustainPercentile<=0.0 ? m_previoussustainPercentile : _sustainPercentile);
        _releasePercentile=(_releasePercentile<=0.0 ? m_previousreleasePercentile : _releasePercentile);
    }

    m_previousAttackPercentile=_attackPercentile;
    m_previousholdPercentile=_holdPercentile;
    m_previousdecayPercentile=_decayPercentile;
    m_previoussustainPercentile=_sustainPercentile;
    m_previousreleasePercentile=_releasePercentile;


    //This set must be always true! Otherwise something is wrong
    Q_ASSERT(setTimeParameters(_attackPercentile*m_total,
                               _holdPercentile*m_total,
                               _decayPercentile*m_total,
                               _sustainPercentile*m_total,
                               _releasePercentile*m_total));
}

void DataEnvelopeParameters::setLevelParameters(qreal holdLevel,qreal sustainLevel) {
    m_holdLevel=(holdLevel > DATAENVELOPE_UPPERBOUND_AMPLITUDE ? DATAENVELOPE_UPPERBOUND_AMPLITUDE : holdLevel );
    m_holdLevel=(holdLevel < DATAENVELOPE_LOWERBOUND_AMPLITUDE ? DATAENVELOPE_LOWERBOUND_AMPLITUDE : holdLevel );

    m_sustainLevel=(sustainLevel > DATAENVELOPE_UPPERBOUND_AMPLITUDE ? DATAENVELOPE_UPPERBOUND_AMPLITUDE : sustainLevel );
    m_sustainLevel=(sustainLevel < DATAENVELOPE_LOWERBOUND_AMPLITUDE ? DATAENVELOPE_LOWERBOUND_AMPLITUDE : sustainLevel );

    emit (amplitudeParametersChanged());
}


bool DataEnvelopeParameters::setTimeParameters(qreal attack, qreal hold, qreal decay ,qreal sustain, qreal release) {
    bool retval=false;
    attack=(attack>=0 ? attack : 0);
    hold=(hold>=0 ? hold : 0);
    decay=(decay>=0 ? decay : 0);
    sustain=(sustain>=0 ? sustain : 0);
    release=(release>=0 ? release : 0);
    qreal _total=attack+hold+decay+sustain+release;
   // qDebug() << "DataEnvelopeParameters::setTimeParameters _total=" << _total << " m_total=" << m_total << " comparison (_total <= m_total) is " <<(_total <= m_total);

    //To avoid prolem of rounding the total is compared with rounding after a multplication by 1000000
    //Set only if _total is adequate
    if (qFloor(1000000*_total) <= qFloor(1000000*m_total)) {
        m_attack=attack;
        m_hold=hold;
        m_decay=decay;
        m_sustain=sustain;
        m_release=release;
        retval=true;
        emit (timeParametersChanged());
    } else {
        qWarning() << "DataEnvelopeParameters::setTimeParameters dataset invalid " << "+" <<attack <<"+" << hold << "+" <<decay << "+" <<sustain << "+" <<release <<"=" <<_total;
        qWarning() << "DataEnvelopeParameters::setTimeParameters should be "<< _total << "<=" <<m_total;
    }
    return retval;
}

void DataEnvelopeParameters::regenerateDomDocument()
{
    //Get DOM document of this object QPROPERTY
    if (m_doc==NULL) m_doc=new QDomDocument();
    this->selfObjectData(m_doc,ENEVELOPEPARAMETERS_TAG);
    Q_ASSERT(!m_doc->isNull());
    Q_ASSERT(m_doc->isDocument());
}
