#ifndef TIMEPLOTWIGET_H
#define TIMEPLOTWIGET_H

#include "plotwidget.h"

class TimePlotWidget : public PlotWidget
{
    Q_OBJECT
public:
    explicit TimePlotWidget(QWidget *parent = 0, int xScaleType=PlotWidget::Linear, int yScaleType=PlotWidget::Linear);
    virtual QWidget * getControlWidget() {return m_allControl;}
    double sampleRate() {return m_SR;}

signals:
    
public slots:
    void setSampleRate(double SR);
    void setDuration(double duration);
    void timeOptionPressed();
    void zoomPanButtonPressed();

private:
    void createControlWidget();//Create the the base contro
    void initBaseControlWidget();
    static double const BASE_SR=48000;//The base sample rate used as default
    static double const BASE_T0=0.0;//Start time or lower time limit. For now is always 0, tommorow may change.
    static double const BASE_DURATION=7.0;//The base duration used as default

    double m_SR;
    double m_duration;
    double m_t0;
    struct {
      QWidget * baseControlWidget;
      ZMP_Handler * m_zmp;//Handle zoom, panel etc
      QPushButton * toggleButtonOptionEnable;
      QPushButton * toggleButtonZMP;//Zoom Move Pan enable button
      ScaledSliderWidget * sliderSR;//Allows to change the sample rate of the whole generated signal
      ScaledSliderWidget * sliderDuration;//Allows to change the max duration of ALL the signals. The reference is ALWAYS t0
    } m_baseControl;
    QWidget * m_allControl;//The widget with all control (base and eventually extended

};

#endif // TIMEPLOTWIGET_H
