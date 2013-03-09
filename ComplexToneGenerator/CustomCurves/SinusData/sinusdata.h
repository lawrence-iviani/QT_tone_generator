#ifndef SINUSDATA_H
#define SINUSDATA_H

#include <QObject>
#include <QWidget>
#include "scaledsliderwidget.h"
#include "CTG_constants.h"
#include <math.h>

static qreal const SINUSDATA_DEFAULT_INITPHASE=0;
static qreal const SINUSDATA_DEFAULT_FREQUENCY=1000;
static qreal const SINUSDATA_DEFAULT_AMPLITUDE=0.9;

#define M_SIN(amplitude,frequency,phase,t) amplitude*sin(2*M_PI*frequency*t+phase)

class SinusParams {
public:
    explicit SinusParams();
    explicit SinusParams(qreal amplitude, qreal frequency, qreal initPhase);


    inline qreal amplitude() {return m_amplitude;}
    inline qreal frequency() {return m_frequency;}
    inline qreal initPhase() {return m_initPhase;}//In degree

    inline bool setAmplitude(qreal amplitude) {
        if (m_amplitude!=amplitude) {
            m_amplitude=amplitude;
            return true;
        }
        return false;
    }
    inline bool setFrequency(qreal frequency) {
        if (m_frequency!=frequency) {
            m_frequency=frequency;
            return true;
        }
        return false;
    }
    inline bool setInitPhase(qreal initPhase){
        if (m_initPhase!=initPhase) {
            m_initPhase=initPhase;
            return true;
        }
        return false;
    }

    /**
     * @brief calcSinus calculate sinus for value n at samplerate
     * @return
     */
    qreal calcSinus(quint64 n,qreal SR);

    /**
     * @brief calcSinus
     * @param sinus
     * @param n_dw
     * @param n_up
     * @param SR
     */
    void calcSinus(qreal *sinus,quint64 n_dw,quint64 n_up,qreal SR);

private:
    qreal m_amplitude;
    qreal m_frequency;
    qreal m_initPhase;
};

class SinusUI : public QWidget {
    Q_OBJECT
public:
    explicit SinusUI(QWidget *widget = 0);
    virtual ~SinusUI() {}
signals:
    void amplitudeUIChanged(qreal);
    void frequencyUIChanged(qreal);
    void initPhaseUIChanged(qreal);

public slots:
    void amplitudeUIUpdate(qreal amplitude);
    void frequencyUIUpdate(qreal frequency);
    void initPhaseUIUpdate(qreal initphase);

private:
    void initControlWidget();

    struct {
      ScaledSliderWidget *sliderFrequency;
      ScaledSliderWidget *sliderAmplitude;
      ScaledSliderWidget *sliderInitPhase;
    } m_sinusDataControl;

};

#endif // SINUSDATA_H
