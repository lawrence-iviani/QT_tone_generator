#ifndef DATAENVELOPEUI_H
#define DATAENVELOPEUI_H

#include <QFrame>
#include <QComboBox>
#include <QCheckBox>
#include <qmath.h>
#include "scaledsliderwidget.h"
#include "envelope/dataenvelopeparameters.h"
#include "CTG_constants.h"
#include "DataUiHandler.h"

class DataEnvelopeParameters;

static const qreal  DATAENVELOPEUI_TIME_STEP=0.0001;

class DataEnvelopeUI : public DataUiHandlerUI
{
    Q_OBJECT
    
public:
    explicit DataEnvelopeUI(QWidget *widget = 0);
    virtual ~DataEnvelopeUI();

//    /**
//     * @brief isEnabledEnvelopeUI, return if is enabled the envloper.
//     * @return true, envelope enabled, false envelope disabled.
//     */
//    bool isEnabledEnvelopeUI();


signals:
    void holdLevelUIChanged(qreal);
    void sustainLevelUIChanged(qreal);
    void attackDurationUIChanged(qreal);
    void holdDurationUIChanged(qreal);
    void decayDurationUIChanged(qreal);
    void sustainDurationUIChanged(qreal);
    void releaseDurationUIChanged(qreal);
    void enableUIChanged(bool);
    void envelopeTypeUIChanged(QString);

public slots:
    /**
      * Set the hold amplitude value
      */
    void holdLevelUIUpdate(qreal holdAmplitude);

    /**
     * @brief setSustainAmplitude , Set the sustain amplitude value
     * @param sustainAmplitude amplitude sustain to set.
     */
    void sustainLevelUIUpdate(qreal sustainAmplitude);

    void attackDurationUIUpdate(qreal attackTime);
    void holdDurationUIUpdate(qreal holdTime);
    void decayDurationUIUpdate(qreal decayTime);
    void sustainDurationUIUpdate(qreal sustainTime);
    void releaseDurationUIUpdate(qreal releaseTime);

    /**
     * @brief envelopeTypeUIUpdate
     * @param type
     */
    void envelopeTypeUIUpdate(QString type);

    /**
     * @brief setEnableEnvelopeUI enable/disable the UI checkbox related to envelope and emit
     * @param true enable check
     */
    void enableUIUpdate(bool enable);

    /**
     * @brief setParamsTime
     * @param params
     */
    void setParamsTime(DataEnvelopeParameters * params);

private:
    QWidget * m_widgetEnable;
    struct {
       // QLabel * enableLabel;
        QCheckBox * enableCB;
    } m_structEnable;

    //Type
    QWidget* m_widgetType;
    struct {
        QLabel * typeLabel;
        QComboBox* envelopeType;
    } m_structType;

    //Amplitude slider
    QWidget * m_widgetAmplitude;
    struct {
        ScaledSliderWidget * hold;
        ScaledSliderWidget * sustain;
    } m_structAmplitude;

    //Time slider
    QWidget * m_widgetTime;
    struct {
        ScaledSliderWidget * attack;
        ScaledSliderWidget * hold;
        ScaledSliderWidget * decay;
        ScaledSliderWidget * sustain;
        ScaledSliderWidget * release;
    }  m_structTime;

    void initEnableAndTypeWidgets();
    void initEnvelopeWidget();
    void initAmplitudeWidget();
    void initTimeWidget();
    void setSliderValue(ScaledSliderWidget * slider, qreal val);

private slots:
    /**
      * Set a generic time widget, updtaing all the other and the DataEnvelopeParameters, is set in percentile (between 0 and 1)
      */
    void setTimeSlider(ScaledSliderWidget * slider, qreal valPercent);

    void enableCheckBoxToggled();
};

#endif // DATAENVELOPEUI_H
