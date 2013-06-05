#include "dataenvelopeparameters.h"
#include <QDebug>

DataEnvelopeParameters::DataEnvelopeParameters(QObject *object) :
        DataUiHandlerProperty(object)
{
    m_totalTime=5.0;
    this->setTimeParameters(1.0,1.0,1.0,1.0,1.0);
    m_holdLevel=DATAENVELOPE_DEFAULT_HOLDLEVEL;
    m_sustainLevel=DATAENVELOPE_DEFAULT_SUSTAINLEVEL;
    m_envelopeType=DATAENVELOPE_DEFAULT_TYPE;
    m_enable=false;
    connectSignals();
}

DataEnvelopeParameters::DataEnvelopeParameters(qreal attack, qreal hold, qreal decay ,qreal sustain, qreal release, QObject *object) :
        DataUiHandlerProperty(object)
{
    m_totalTime=attack+hold+decay+sustain+release;
    this->setTimeParameters(attack,hold,decay,sustain,release);
    m_holdLevel=DATAENVELOPE_DEFAULT_HOLDLEVEL;
    m_sustainLevel=DATAENVELOPE_DEFAULT_SUSTAINLEVEL;
    m_envelopeType=DATAENVELOPE_DEFAULT_TYPE;
    m_enable=false;
    connectSignals();
}

void  DataEnvelopeParameters::connectSignals() {

    //Connect amplitude changed
    Q_ASSERT(connect(this,SIGNAL(holdLevelChanged(qreal)),this,SIGNAL(amplitudeParametersChanged())));
    Q_ASSERT(connect(this,SIGNAL(sustainLevelChanged(qreal)),this,SIGNAL(amplitudeParametersChanged())));

    //Connect time duration changed
    Q_ASSERT(connect(this,SIGNAL(attackDurationChanged(qreal)),this,SIGNAL(timeParametersChanged())));
    Q_ASSERT(connect(this,SIGNAL(holdDurationChanged(qreal)),this,SIGNAL(timeParametersChanged())));
    Q_ASSERT(connect(this,SIGNAL(decayDurationChanged(qreal)),this,SIGNAL(timeParametersChanged())));
    Q_ASSERT(connect(this,SIGNAL(sustainDurationChanged(qreal)),this,SIGNAL(timeParametersChanged())));
    Q_ASSERT(connect(this,SIGNAL(releaseDurationChanged(qreal)),this,SIGNAL(timeParametersChanged())));

}

void DataEnvelopeParameters::setTimeLength(qreal length) {
    qreal _attackPercentile;
    qreal _holdPercentile;
    qreal _decayPercentile;
    qreal _sustainPercentile;
    qreal _releasePercentile;

    //If the total previous length was 0, update with the previous percentile
    //Other get the actual percentile
    if (m_totalTime==0) {
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

    m_totalTime=(length>=0.0 ? length : 0.0);

    if (m_totalTime==0.0) {
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
    Q_ASSERT(setTimeParameters(_attackPercentile*m_totalTime,
                               _holdPercentile*m_totalTime,
                               _decayPercentile*m_totalTime,
                               _sustainPercentile*m_totalTime,
                               _releasePercentile*m_totalTime));
    emit (timeParamsChanged(this));
}

bool DataEnvelopeParameters::setLevelParameters(qreal holdLevel,qreal sustainLevel) {
    bool retval=false;
    if (holdLevel!=m_holdLevel) {
        m_holdLevel=(holdLevel > DATAENVELOPE_UPPERBOUND_AMPLITUDE ? DATAENVELOPE_UPPERBOUND_AMPLITUDE : holdLevel );
        m_holdLevel=(holdLevel < DATAENVELOPE_LOWERBOUND_AMPLITUDE ? DATAENVELOPE_LOWERBOUND_AMPLITUDE : holdLevel );
        retval=true;
    }

    if (sustainLevel!=m_sustainLevel) {
        m_sustainLevel=(sustainLevel > DATAENVELOPE_UPPERBOUND_AMPLITUDE ? DATAENVELOPE_UPPERBOUND_AMPLITUDE : sustainLevel );
        m_sustainLevel=(sustainLevel < DATAENVELOPE_LOWERBOUND_AMPLITUDE ? DATAENVELOPE_LOWERBOUND_AMPLITUDE : sustainLevel );
        retval=true;
    }
    return retval;
}

bool DataEnvelopeParameters::setTimeParameters(qreal attack, qreal hold, qreal decay ,qreal sustain, qreal release) {
    bool retval=false;
    attack=(attack>=0 ? attack : 0);
    hold=(hold>=0 ? hold : 0);
    decay=(decay>=0 ? decay : 0);
    sustain=(sustain>=0 ? sustain : 0);
    release=(release>=0 ? release : 0);
    qreal _total=attack+hold+decay+sustain+release;

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"try to set attack=%1 hold=%2 decay=%3 sustain=%4 release=%5")
                      .arg(attack)
                      .arg(hold)
                      .arg(decay)
                      .arg(sustain)
                      .arg(release));
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"with _total=%1 m_total=%2 comparison (_total <= m_total) is %3")
                      .arg(_total)
                      .arg(m_totalTime)
                      .arg((_total <= m_totalTime)));

    //To avoid prolem of rounding the total is compared with rounding after a multplication by 1000000
    //Set only if _total is adequate
    if (qFloor(1000000*_total) <= qFloor(1000000*m_totalTime)) {
        m_attack=attack;
        m_hold=hold;
        m_decay=decay;
        m_sustain=sustain;
        m_release=release;
        retval=true;
        PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"set to m_attack=%1 m_hold=%2 m_decay=%3 m_sustain=%4 m_release=%5")
                          .arg(m_attack)
                          .arg(m_hold)
                          .arg(m_decay)
                          .arg(m_sustain)
                          .arg(m_release));
        //emit (timeParametersChanged());
    } else {
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"dataset invalid %1+%2+%3+%4+%5=%6").arg(attack).arg(hold).arg(decay).arg(sustain).arg(release).arg(_total));
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"should be %1 <=%2").arg(_total).arg(m_totalTime));
    }
    return retval;
}

//Single Setter
void DataEnvelopeParameters::setEnvelopeType(QString type) {
    if (type!=m_envelopeType)
        if (DataEnvelope::isValidEnvelopeType(type)) {
            m_envelopeType=type;
            emit (envelopeTypeChanged(type));
        }
        else
            PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"Unsupported type"));
}

void DataEnvelopeParameters::setAttackDuration(qreal attack) {
    if (attack==m_attack) return;
    bool retval=setTimeParameters(attack,m_hold,m_decay,m_sustain,m_release);
    if (retval) {
        emit (attackDurationChanged(attack));
    } else {
        emit (timeParamsChanged(this));
    }

}
void DataEnvelopeParameters::setHoldDuration(qreal hold) {
    if (hold==m_hold) return;
    bool retval=this->setTimeParameters(m_attack,hold,m_decay,m_sustain,m_release);
    if (retval) {
        emit (holdDurationChanged(hold));
    } else {
        emit (timeParamsChanged(this));
    }
}
void DataEnvelopeParameters::setDecayDuration(qreal decay) {
    if (decay==m_decay) return;
    bool retval=this->setTimeParameters(m_attack,m_hold,decay,m_sustain,m_release);
    if (retval) {
        emit (decayDurationChanged(decay));
    } else {
        emit (timeParamsChanged(this));
    }

}
void DataEnvelopeParameters::setSustainDuration(qreal sustain) {
    if (sustain==m_sustain) return;
    bool retval=this->setTimeParameters(m_attack,m_hold,m_decay,sustain,m_release);
    if (retval) {
        emit (sustainDurationChanged(sustain));
    } else {
        emit (timeParamsChanged(this));
    }

}
void DataEnvelopeParameters::setReleaseDuration(qreal release) {
    if (release==m_release) return;
    bool retval=this->setTimeParameters(m_attack,m_hold,m_decay,m_sustain,release);
    if (retval) {
        emit (releaseDurationChanged( release));
    } else {
        emit (timeParamsChanged(this));
    }
}
void DataEnvelopeParameters::setHoldLevel(qreal holdLevel) {
    if (holdLevel==m_holdLevel) return;
    bool retval=setLevelParameters( holdLevel, m_sustainLevel);
    if (retval) emit (holdLevelChanged(holdLevel));
}
void DataEnvelopeParameters::setSustainLevel(qreal sustainLevel) {
    if (sustainLevel==m_sustainLevel) return;
    bool retval=setLevelParameters( m_holdLevel, sustainLevel);
    if (retval) emit (sustainLevelChanged(sustainLevel));
}
void DataEnvelopeParameters::setEnableEnvelope(bool enable) {
    if(m_enable!=enable) {
        m_enable=enable;
        emit (enableEnvelopeChanged(enable));
    }
}

const qreal DataEnvelopeParameters::spareTimeAvailable() {
    //Setting scale
    qreal _allocatedTime=m_attack + m_hold + m_decay + m_sustain + m_release;
    qreal _remainingTime=(m_totalTime-_allocatedTime);
    //    //Avoid a problem rounding. Assuming remainingTime is 0.0 if smaller than an arbitraty small amount
    _remainingTime=(qAbs(_remainingTime) < (1.0e-6) ? 0.0 : _remainingTime);
    Q_ASSERT( _remainingTime>=0.0);
    Q_ASSERT( _remainingTime<=m_totalTime);
    return  _remainingTime;
}

