#ifndef TIMEPLOTWIGET_H
#define TIMEPLOTWIGET_H

#include "plotwidget.h"

class TimePlotWidget : public PlotWidget
{
    Q_OBJECT
public:
    explicit TimePlotWidget(QWidget *parent = 0, int xScaleType=PlotWidget::Linear, int yScaleType=PlotWidget::Linear);
    virtual QWidget * getControlWidget() {return m_allControl;}
signals:
    
public slots:
    void setSampleRate(double SR);
    void timeOptionPressed();
    void zoomPanButtonPressed();

private:
    void createControlWidget();//Create the the base contro
    void initBaseControlWidget();
    static int const BASE_SR=48000;

    double m_SR;
    struct {
      QWidget * baseControlWidget;
      ZMP_Handler * m_zmp;//Handle zoom, panel etc
      QPushButton * toggleButtonOptionEnable;
      QPushButton * toggleButtonZMP;//Zoom Move Pan enable button
      ScaledSliderWidget * slider_SR;//Allows to change the sample rate of the whole generated signal
    } m_baseControl;
    QWidget * m_allControl;//The widget with all control (base and eventually extended

};

#endif // TIMEPLOTWIGET_H
