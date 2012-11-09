#ifndef DATAENVELOPE_H
#define DATAENVELOPE_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QDebug>
#include "dataenvelopeparameters.h"
#include "dataenvelopeui.h"
#include "timedata/generictimedata.h"

class GenericTimeData;

/**
  * The DataEnvelope class is a that contains the amplitude envelope of a signal in terms of description but also in terms of data.
  * The envelope is what I call AHDSR envelop (Attack, Hold, Decay, Sustain, Release)
  */
class DataEnvelope : public QObject
{
    Q_OBJECT
    Q_ENUMS(TransientFunction)
public:
    explicit DataEnvelope(GenericTimeData *parent=0, QWidget *widget=0);
    explicit DataEnvelope(qreal SR, GenericTimeData *parent=0, QWidget *widget=0);
    explicit DataEnvelope(DataEnvelopeParameters *params, qint64 length , qreal SR, GenericTimeData *parent=0, QWidget *widget = 0 );

    enum TransientFunction {Linear, Exponential, Logarithmic};
    virtual ~DataEnvelope();

    /**
      * Return a const pointer to the envelope data
      */
    const qreal * getEnvelope() {return (const qreal*) m_envelope;}

    /**
      * Return the length in samples of the whole envelope data.
      */
    qint64 length();

    /**
      * Return the ui to control this envelope
      */
    CustomCurveUI * getEnvelopeUI() {return m_envelopeUI;}

    /**
      * Return the data class with envelope parameters
      */
    const DataEnvelopeParameters * getEnvelopeParameters() {return (const DataEnvelopeParameters *) m_envelopeParams;}

    /**
     * @brief isEnableEnvelope tell if the envelope is enabled
     * @return true if enabled
     */
    bool isEnableEnvelope() {return m_envelopeParams->isEnableEnvelope();}

signals:
    /**
     * @brief enabledToggle, the envelope enabled was toggled
     * @param toggle, true if envelope is enabled
     */
    void enableToggled(bool enable);

    /**
     * @brief envelopeChanged, emitted whenever the enable is modified (length or parameters)
     */
    void envelopeChanged();

public slots:

    /**
      * Set the length in sample of the envelope
      */
    void setLength(qint64 length);

    /**
      * Set the params of the whole envelope
      */
    void setEnvelopeParams(DataEnvelopeParameters * params);

    /**
      * Set the params and length of the envelope
      */
    void setEnvelopeParamsAndLength(DataEnvelopeParameters *params,qint64 length);

    /**
      * Set the sample rate, this is needed for the UI sample length.
      */
    void setSampleRate(qreal SR);

    /**
      * Call this if the amplitude parameters are changed and/or need recalculation of the amplitude values and relative calls to UI etc.
      */
    void amplitudeEnvelopeChanged();

    /**
      * Call this if the time parameters are changed and/or need recalculation of the amplitude values and relative calls to UI etc.
      */
    void timeEnvelopeChanged();

    /**
     * @brief enabledToggle, the envelope enabled was toggled
     * @param toggle, true if envelope is enabled
     */
    void enableEnvelope(bool enable) {m_envelopeParams->setEnableEnvelope(enable);}

private:
    DataEnvelopeParameters * m_envelopeParams;
    DataEnvelopeUI *m_envelopeUI;
    GenericTimeData *m_genericTimeData;
    qint64 m_length;
    qreal * m_envelope;
    qreal m_SR;
    void init(QWidget *widget);
    void recalculateEnvelope();
    void connectingSignals();
};

#endif // DATAENVELOPE_H
