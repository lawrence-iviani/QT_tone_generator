#ifndef FREQPLOTWIDGET_H
#define FREQPLOTWIDGET_H

#include "plotwidget.h"

class FreqPlotWidget : public PlotWidget
{
    Q_OBJECT
public:
    explicit FreqPlotWidget(QWidget *parent = 0, int xScaleType=PlotWidget::Linear, int yScaleType=PlotWidget::Linear);
    virtual QWidget * getControlWidget() {return m_allControl;}
signals:
    
public slots:
    
private slots:
    void freqOptionPressed();

private:
    void createControlWidget();//Create the the base contro
    void initBaseControlWidget();

    struct {
      QWidget * baseControlWidget;
      QPushButton * toggleButtonOptionEnable;
    } m_baseControl;
    QWidget * m_allControl;//The widget with all control (base and eventually extended

};

#endif // FREQPLOTWIDGET_H
