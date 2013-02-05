#ifndef DATAENVELOPEPARAMETERS_H
#define DATAENVELOPEPARAMETERS_H

#include <QObject>
#include <qmath.h>
#include <DataUiHandler.h>
#include <errormessage.h>

//Some constants
static const qreal DATAENVELOPE_DEFAULT_HOLDLEVEL=1.0;
static const qreal DATAENVELOPE_DEFAULT_SUSTAINLEVEL=0.5;
static const qreal DATAENVELOPE_UPPERBOUND_AMPLITUDE=1.0;
static const qreal DATAENVELOPE_LOWERBOUND_AMPLITUDE=0.0;
static const unsigned short DATAENVELOPE_DIGIT_TIME_ACCURACY=4;//four digit should be enough

class DataEnvelope;

/**
  * This class is a container for the parameters describing the Envelope, the absolute value should be in sec.
  * These params can be returned as percentile (respect to the total of all params inserted) or as original value
  */
class DataEnvelopeParameters : public DataUiHandlerProperty
{
    Q_OBJECT
    Q_PROPERTY(qreal attack READ attack WRITE setAttack NOTIFY attackChanged)
    Q_PROPERTY(qreal hold READ hold WRITE setHold NOTIFY holdChanged)
    Q_PROPERTY(qreal decay READ decay WRITE setDecay NOTIFY decayChanged)
    Q_PROPERTY(qreal sustain READ sustain WRITE setSustain NOTIFY sustainChanged)
    Q_PROPERTY(qreal release READ release WRITE setRelease NOTIFY releaseChanged)
    Q_PROPERTY(qreal holdlevel READ holdLevel WRITE setHoldLevel NOTIFY holdLevelChanged)
    Q_PROPERTY(qreal sustainlevel READ sustainLevel WRITE setSustainLevel NOTIFY sustainLevelChanged)
    Q_PROPERTY(bool enable READ isEnabledEnvelope WRITE setEnableEnvelope NOTIFY enableEnvelopeChanged)

public:

    explicit  DataEnvelopeParameters(QObject *object = 0);

    /**
      * Set the data with the constructor
      */
    explicit  DataEnvelopeParameters(qreal attack, qreal hold, qreal decay , qreal sustain, qreal release, QObject *object = 0);

    friend class DataEnvelope;
    qreal attack() {return m_attack;}
    qreal hold() {return m_hold;}
    qreal decay() {return m_decay;}
    qreal sustain() {return m_sustain;}
    qreal release() {return m_release;}

    /**
      *
      */
    qreal holdLevel() {return m_holdLevel;}

    /**
      *
      */
    qreal sustainLevel() {return m_sustainLevel;}

    /**
      * This function returns the sum of all the time values
      */
    qreal total() {return m_total;}

    /**
     * @brief isEnabledEnvelope tell if the envelope is enabled
     * @return true if enabled
     */
    bool isEnabledEnvelope() {return m_enable;}

signals:
    void attackChanged(qreal);
    void holdChanged(qreal);
    void decayChanged(qreal);
    void sustainChanged(qreal);
    void releaseChanged(qreal);
    void holdLevelChanged(qreal);
    void sustainLevelChanged(qreal);
    void enableEnvelopeChanged(bool);

//    /**
//     * @brief amplitudeParametersChanged,    Emitted if some amplitude params are changhed.
//     */
//    void amplitudeParametersChanged();

//    /**
//     * @brief timeParametersChanged, Emitted if some time params are changhed
//     */
//    void timeParametersChanged();

//    /**
//     * @brief enabledToggle, the envelope enabled was toggled
//     * @param toggle, true if envelope is enabled
//     */
//    void enableToggled(bool enable);

public slots:
    /**
     * @brief setAttack
     * @param attack
     * @return
     */
    bool setAttack(qreal attack);
    bool setHold(qreal hold);
    bool setDecay(qreal decay);
    bool setSustain(qreal sustain);
    bool setRelease(qreal release);

    void setHoldLevel(qreal holdLevel);
    void setSustainLevel(qreal sustainLevel);


    /**
      * Set the max length of all the parameters in this class, can be any number (sample, msec whatever).
      */
    void setTimeLength(qreal length);

    /**
     * @brief set if enable/disable the envelopeUI or not
     * @param enable true enable the UI, false disable UI
     */
    void setEnableEnvelope(bool enable);

protected slots:


private:
    /**
      * Set hold and sustain level parameters
      */
    void setLevelParameters(qreal holdLevel,qreal sustainLevel);

    /**
      * Set all the duration parameters. No matter if they are in time or sample, the total must be less of the length set.
      * Return a true if the operation was made (summing time < total time set) or false if it's not possible  to set these values.
      */
    bool setTimeParameters(qreal attack, qreal hold, qreal decay ,qreal sustain, qreal release);


    void connectSignals();
    qreal attackPercentile() {return (m_total>0.0 ? m_attack/m_total : 0.0 );}
    qreal holdPercentile() {return (m_total>0.0 ? m_hold/m_total : 0.0);}
    qreal decayPercentile() {return (m_total>0.0 ? m_decay/m_total : 0.0);}
    qreal sustainPercentile() {return (m_total>0.0 ? m_sustain/m_total : 0.0);}
    qreal releasePercentile() {return (m_total>0.0 ? m_release/m_total : 0.0);}


    bool m_enable;
    qreal m_total;
    qreal m_attack;
    qreal m_hold;
    qreal m_decay;
    qreal m_sustain;
    qreal m_release;
    qreal m_holdLevel;
    qreal m_sustainLevel;

    qreal m_previousAttackPercentile;
    qreal m_previousholdPercentile;
    qreal m_previousdecayPercentile;
    qreal m_previoussustainPercentile;
    qreal m_previousreleasePercentile;
};



#endif // DATAENVELOPEPARAMETERS_H
