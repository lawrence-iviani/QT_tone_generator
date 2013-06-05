#ifndef DATAENVELOPE_H
#define DATAENVELOPE_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QDebug>
#include "dataenvelopeparameters.h"
#include "dataenvelopeui.h"
#include <DataUiHandler.h>

#define ENVELOPE_TYPE "Linear" << "Exponential" << "Inverted exponential"<< "Logarithmic"


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

    enum TransientFunction {Linear, Exponential, InvertedExp, Logarithmic};
    static QStringList transientFunctionList() {
        QStringList _retval;
        _retval << ENVELOPE_TYPE;
        return _retval;
    }

    static bool isValidEnvelopeType(QString type) {
        QStringList list;
        list << ENVELOPE_TYPE;
        if (list.contains(type)) return true;
        else return false;
    }

    static uint envelopeType2Enum(QString type) {
        uint retval=-1;
        if (type=="Linear") retval=Linear;
        if (type=="Exponential") retval=Exponential;
        if (type=="Logarithmic") retval=Logarithmic;
        if (type=="Inverted exponential") retval=InvertedExp;
        return retval;
    }

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

    void recalculateLinearEnvelope(quint64 attack, quint64 hold, quint64 decay, quint64 sustain, quint64 release);
    void recalculateExponentialEnvelope(quint64 attack, quint64 hold, quint64 decay, quint64 sustain, quint64 release, bool invertedExponential=false);
    void recalculateLogEnvelope(quint64 attack, quint64 hold, quint64 decay, quint64 sustain, quint64 release);
    void recalculateEnvelope();
    void connectingSignals();

    /**
     * @brief exponetialCoeff Return an exp functiom in the form slope*pow(x)+c with P1(x1,y1) and P2(x2,y2) belongings to the function. The base is arbitrary
     * @param slope
     * @param c
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     */
    static inline void exponetialCoeff(qreal& slope, qreal& c, const qreal& x1, const qreal& y1, const qreal& x2, const qreal& y2, bool inverted=false);

    /**
     * @brief logCoeff Return a log functiom in the form slope*log(x)+c with P1(x1,y1) and P2(x2,y2) belongings to the function. The base is arbitrary
     * @param slope
     * @param c
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     */
    static inline void logCoeff(qreal& slope, qreal& c, const qreal& x1, const qreal& y1, const qreal& x2, const qreal& y2);

private slots:

    void envelopeHasToggledEnable();
    /**
     * @brief envelopeHasChanged This method should be called when the envelope has changed and need to be recalc
     */
    void envelopeHasChanged();
};

#endif // DATAENVELOPE_H
