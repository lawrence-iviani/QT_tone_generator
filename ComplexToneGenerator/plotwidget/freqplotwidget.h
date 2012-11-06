#ifndef FREQPLOTWIDGET_H
#define FREQPLOTWIDGET_H

#include "plotwidget.h"
#include <CTG_constants.h>

class FreqPlotWidget : public PlotWidget
{
    Q_OBJECT
public:
    explicit FreqPlotWidget(QWidget *widget = 0, int xScaleType=PlotWidget::Linear, int yScaleType=PlotWidget::Linear);
    virtual QWidget * getControlWidget() {return m_allControl;}
signals:
    
public slots:
    
private slots:
    void freqOptionPressed();

private:
    void createControlWidget();//Create the the base contro
    void initBaseControlWidget();

    struct {
      QFrame * baseControlWidget;
      QPushButton * toggleButtonOptionEnable;
    } m_baseControl;
    QFrame * m_allControl;//The widget with all control (base and eventually extended

};

#endif // FREQPLOTWIDGET_H
