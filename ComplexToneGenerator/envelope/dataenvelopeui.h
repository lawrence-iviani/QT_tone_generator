#ifndef DATAENVELOPEUI_H
#define DATAENVELOPEUI_H

#include <QFrame>
#include <QCheckBox>
#include <qmath.h>
#include "scaledsliderwidget.h"
#include "envelope/dataenvelopeparameters.h"
#include <CustomCurves/customcurveui.h>
#include "CTG_constants.h"

namespace Ui {
class DataEnvelopeUI;
}

static const qreal  DATAENVELOPEUI_TIME_STEP=0.0001;

class DataEnvelopeUI : public CustomCurveUI
{
    Q_OBJECT
    
public:
    explicit DataEnvelopeUI(QWidget *widget = 0);
    explicit DataEnvelopeUI(DataEnvelopeParameters *params ,  QWidget *widget = 0);
    ~DataEnvelopeUI();

    /**
     * @brief isEnabledEnvelopeUI, return if is enabled the envloper.
     * @return true, envelope enabled, false envelope disabled.
     */
    bool isEnabledEnvelopeUI();


signals:

public slots:
    /**
      * Set the hold amplitude value
      */
    void setHoldAmplitude(qreal holdAmplitude);

    /**
     * @brief setSustainAmplitude , Set the sustain amplitude value
     * @param sustainAmplitude amplitude sustain to set.
     */
    void setSustainAmplitude(qreal sustainAmplitude);

    void setAttackTime(qreal attackTime);
    void setHoldTime(qreal holdTime);
    void setDecayTime(qreal decayTime);
    void setSustainTime(qreal sustainTime);
    void setReleaseTime(qreal releaseTime);

    /**
     * @brief setEnableEnvelopeUI enable/disable the UI checkbox related to envelope and emit
     * @param true enable check
     */
    void setEnableEnvelopeUI(bool enable);

    /**
      * Refresh the ui with the actual value in the DataEnvelopeParameters class
      */
    virtual void updateControlUI();

    void updateUI();


private:
    Ui::DataEnvelopeUI *ui;

    //Amplitude slider
    struct {
        ScaledSliderWidget * hold;
        ScaledSliderWidget * sustain;
    } m_structAmplitude;

    //Time slider
    struct {
        ScaledSliderWidget * attack;
        ScaledSliderWidget * hold;
        ScaledSliderWidget * decay;
        ScaledSliderWidget * sustain;
        ScaledSliderWidget * release;
    }  m_structTime;

    DataEnvelopeParameters *  m_parameters;
    void initEnvelopeWidget();
    void initAmplitudeWidget();
    void initTimeWidget();
    void setSliderValue(ScaledSliderWidget * slider, qreal val);

private slots:
    /**
      * Set a generic time widget, updtaing all the other and the DataEnvelopeParameters, is set in percentile (between 0 and 1)
      */
    void setTimeSlider(ScaledSliderWidget * slider, qreal valPercent);
};

#endif // DATAENVELOPEUI_H
