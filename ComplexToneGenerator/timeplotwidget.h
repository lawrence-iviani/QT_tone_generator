#ifndef TIMEPLOTWIGET_H
#define TIMEPLOTWIGET_H

#include "plotwidget.h"
#include "digesttimedata.h"
#include <CTG_constants.h>

class TimePlotWidget : public PlotWidget
{
    Q_OBJECT
public:
    explicit TimePlotWidget(QWidget *parent = 0, int xScaleType=PlotWidget::Linear, int yScaleType=PlotWidget::Linear);
    ~TimePlotWidget();
    virtual QWidget * getControlWidget() {return m_allControl;}
    double sampleRate() {return m_SR;}
    double duration() {return m_duration;}
    double minTime() {return m_t0;}
    DigestTimeData * getDigestCurve() {return m_digestCurve;} //return the digest curve
signals:
    
public slots:
    void setSampleRate(double SR);
    void setDuration(double duration);
    void timeOptionPressed();
    void zoomPanButtonPressed();
    virtual void dataUpdated() { qDebug() << "TimePlotWidget::dataUpdated() CALLED"; m_digestCurve->updateData(); ;this->replot();} //
   // void curveListHasChanged();
protected:
    DigestTimeData * m_digestCurve;

private:
    void createControlWidget();//Create the the base control
    void initBaseControlWidget();

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
