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
    
private slots:
    void timeOptionPressed();
    void zoomPanButtonPressed();

private:
    void createControlWidget();//Create the the base contro
    void initBaseControlWidget();

    struct {
      QWidget * baseControlWidget;
      ZMP_Handler * m_zmp;//Handle zoom, panel etc
      QPushButton * toggleButtonOptionEnable;
      QPushButton * toggleButtonZMP;//Zoom Move Pan enable button
    } m_baseControl;
    QWidget * m_allControl;//The widget with all control (base and eventually extended

};

#endif // TIMEPLOTWIGET_H
