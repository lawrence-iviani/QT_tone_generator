#ifndef DATAENVELOPE_H
#define DATAENVELOPE_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QDebug>
#include "dataenvelopeparameters.h"
#include "dataenvelopeui.h"
#include <DataUiHandler.h>

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
    explicit DataEnvelope(QObject *parent=0);
    explicit DataEnvelope(qreal SR, QObject *parent=0);
    explicit DataEnvelope(quint64 length , qreal SR, QObject *parent=0);

    enum TransientFunction {Linear, Exponential, Logarithmic};//NOT IMPLEMENTED!!
    virtual ~DataEnvelope();

    /**
      * Return a const pointer to the envelope data
      */
    inline const qreal * getEnvelope() {return (const qreal*) m_envelope;}

    /**
      * Return the length in samples of the whole envelope data.
      */
    inline quint64 sampleNumber() {return m_sampleNumber;}

    /**
     * @brief sampleRate return the actual sample rate used by this class
     * @return
     */
    inline qreal sampleRate() {return m_SR;}


    /**
     * @brief getControlWidget Return the ui to control this envelope
     * @return
     */
    inline QWidget * getControlWidget() {return dynamic_cast<QWidget*>(m_envelopeDelegate->getUI()); }

    /**
     * @brief getDataParameters Return the data class with envelope parameters
     * @return
     */
    inline DataUiHandlerProperty* getDataParameters() {return dynamic_cast<DataUiHandlerProperty*>(m_envelopeDelegate->getProperty());}

    /**
     * @brief getDelegate Retrun the delegate of this class
     * @return
     */
    inline DataUiHandlerDelegate* getDelegate() {return dynamic_cast<DataUiHandlerDelegate*>(m_envelopeDelegate);}

    /**
     * @brief replacePropertyAndUI If for some reason property or UI change (due to inheritance for example) is possible to set again both two
     * @param params
     * @param ui
     */
    void replacePropertyAndUI(DataUiHandlerProperty *params, DataUiHandlerUI *ui);

signals:
    /**
     * @brief enabledToggle, the envelope enabled was toggled
     * @param toggle, true if envelope is enabled
     */
 //   void enableToggled(bool enable);
    //VERIFICARE, NON DOVREBBE PIU' SERVIRE

    /**
     * @brief envelopeChanged, emitted whenever the enable is modified (enable, length or parameters)
     */
    void envelopeChanged();

public slots:

    /**
     * @brief setSampleNumber Set the total length in sample of the envelope
     * @param length
     */
    void setSampleNumber(quint64 length);

    /**
     * @brief setSampleRate Set the sample rate, this is needed for the UI sample length.
     * @param SR
     */
    void setSampleRate(qreal SR);

    /**
     * @brief setSampleRateAndSampleNumber
     * @param SR
     * @param length
     */
    void setSampleRateAndSampleNumber(qreal SR,quint64 length);

private:
    DataUiHandlerDelegate *m_envelopeDelegate;

   // GenericTimeData *m_genericTimeData;
    quint64 m_sampleNumber;
    qreal * m_envelope;
    qreal m_SR;
    void init(QObject *parent);
    void recalculateEnvelope();
    void connectingSignals();

private slots:

    void envelopeHasToggledEnable();
    /**
     * @brief envelopeHasChanged This method should be called when the envelope has changed and need to be recalc
     */
    void envelopeHasChanged();
};

#endif // DATAENVELOPE_H
