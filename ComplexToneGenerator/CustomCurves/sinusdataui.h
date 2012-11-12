#ifndef SINUSDATAUI_H
#define SINUSDATAUI_H

#include <CustomCurves/customcurveui.h>
#include <scaledsliderwidget.h>
#include <CustomCurves/sinusdata.h>
#include <CTG_constants.h>

class SinusDataParams;

class SinusDataUI : public CustomCurveUI
{
    Q_OBJECT
public:
    explicit SinusDataUI(SinusDataParams *sd, QWidget *widget = 0);
signals:

public slots:
    /**
      * Force the UI to be update re-reading information from the referenced GenericTimeData class.
      * This slot doesn't propagate the signal/slot related to the UI, is intended to be called just to refresh the UI if something changes in the data class
      */
    /**
     * @brief updateControlUI Force the UI to be update re-reading information from the referenced GenericTimeData class.
     * This slot doesn't propagate the signal/slot related to the UI, is intended to be called just to refresh the UI if something changes in the data class
     */
    virtual void updateControlUI();

private:
    void initControl();
    void connectingControlSignal();
    void setSliderValue(ScaledSliderWidget * slider, qreal val);

    struct {
      ScaledSliderWidget *sliderFrequency;
      ScaledSliderWidget *sliderAmplitude;
      ScaledSliderWidget *sliderInitPhase;
    } m_sinusDataControl;
    SinusDataParams *m_sinusData;
};



#endif // SINUSDATAUI_H
