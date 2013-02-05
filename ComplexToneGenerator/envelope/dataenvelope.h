#ifndef DATAENVELOPE_H
#define DATAENVELOPE_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QDebug>
#include "dataenvelopeparameters.h"
#include "dataenvelopeui.h"
#include "dataenvelopedelegate.h"
//#include "timedata/generictimedata.h"
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
    explicit DataEnvelope(GenericTimeData *parent=0);
    explicit DataEnvelope(qreal SR, GenericTimeData *parent=0);
    explicit DataEnvelope(qint64 length , qreal SR, GenericTimeData *parent=0);

    enum TransientFunction {Linear, Exponential, Logarithmic};//NOT IMPLEMENTED!!
    virtual ~DataEnvelope();

    /**
      * Return a const pointer to the envelope data
      */
    inline const qreal * getEnvelope() {return (const qreal*) m_envelope;}
    //OK

    /**
      * Return the length in samples of the whole envelope data.
      */
    inline qint64 length();
    //OK

    /**
      * Return the ui to control this envelope
      */
    QWidget * getEnvelopeUI() {return (QWidget*) m_envelopeUI;}
    //RIVEDERE/IMPLMENETARE CUSTOMCURVEUI

    /**
      * Return the data class with envelope parameters
      */
    inline DataEnvelopeParameters * getEnvelopeParameters() {return m_envelopeParams;}
    //OK

//    /**
//     * @brief getEnvelopeParametersDomDocument Return the envelope parameters in a DomDocument format
//     * @return
//     */
//    const  QDomDocument* getEnvelopeParametersDomDocument() {
//        return (const QDomDocument *) m_envelopeParams->getDomDocument();
//    }

//    /**
//     * @brief forceRegenarateDomDocument
//     */
//    void forceRegenerateDomDocument() {
//        m_envelopeParams->regenerateDomDocument();
//    }

    /**
     * @brief isEnableEnvelope tell if the envelope is enabled
     * @return true if enabled
     */
    inline bool isEnableEnvelope() {return m_envelopeParams->isEnabledEnvelope();}

signals:
    /**
     * @brief enabledToggle, the envelope enabled was toggled
     * @param toggle, true if envelope is enabled
     */
    void enableToggled(bool enable);
    //VERIFICARE, NON DOVREBBE PIU' SERVIRE

    /**
     * @brief envelopeChanged, emitted whenever the enable is modified (length or parameters)
     */
    void envelopeChanged();

public slots:

    /**
      * Set the total length in sample of the envelope
      */
    void setLength(qint64 length);
    //OK

//    /**
//      * Set the params of the whole envelope
//      */
//    void setEnvelopeParams(DataEnvelopeParameters * params);
    //DIRETTAMENTE VIA GET/SET

//    /**
//     * @brief setEnvelopeParams using a well formed DOM node
//     * @param node
//     * @return
//     */
//    bool setEnvelopeParams(QDomNode& node);
    //DIRETTAMENTE VIA GET/SET

//    /**
//      * Set the params and length of the envelope
//      */
//    void setEnvelopeParamsAndLength(DataEnvelopeParameters *params,qint64 length);
    //DIRETTAMENTE VIA GET/SET

    /**
      * Set the sample rate, this is needed for the UI sample length.
      */
    void setSampleRate(qreal SR);
    //OK

    /**
      * Call this if the amplitude parameters are changed and/or need recalculation of the amplitude values and relative calls to UI etc.
      */
 //   void amplitudeEnvelopeChanged();
    //VERIFICARE

    /**
      * Call this if the time parameters are changed and/or need recalculation of the amplitude values and relative calls to UI etc.
      */
 //   void timeEnvelopeChanged();
    //VERIFICARE

    /**
     * @brief enabledToggle, the envelope enabled was toggled
     * @param toggle, true if envelope is enabled
     */
    void enableEnvelope(bool enable) {m_envelopeParams->setEnableEnvelope(enable);}
    //OK

private:
    DataEnvelopeParameters * m_envelopeParams;
    DataEnvelopeUI *m_envelopeUI;
    DataEnvelopeDelegate *m_envelopeDelegate;

    GenericTimeData *m_genericTimeData;
    qint64 m_length;
    qreal * m_envelope;
    qreal m_SR;
    void init(QWidget *widget);
    void recalculateEnvelope();
    void connectingSignals();
};

#endif // DATAENVELOPE_H
