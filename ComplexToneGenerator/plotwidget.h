#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_series_data.h>
#include <qwt_global.h>
#include <math.h>
#include <qwt_interval.h>
#include <qwt_scale_engine.h>
#include <QList>
#include "generictimedata.h"
#include "zmp_handler.h"


class PlotWidget : public QwtPlot
{
    Q_OBJECT
    Q_ENUMS( Scale )
public:
    enum Scale {Linear=0, Logarithmic=1};
  //  explicit PlotWidget(QWidget *parent = 0);
    explicit PlotWidget(QWidget *parent = 0, int xScaleType=PlotWidget::Linear, int yScaleType=PlotWidget::Linear);
    void setAxisName(QString xName,QString yName);
    void setBothAxisScale(double xmin, double xmax,double ymin, double ymax);
    void setBothAxisScale(int xScaleType, double xmin, double xmax,int yScaleType,double ymin, double ymax);
    void setXScaleType(int xScaleType);
    void setYScaleType(int yScaleType);
    void setDimension(int pointDimension);
    int addTimeData(GenericTimeData * gtd);
    QWidget * getControlWidget() {return m_allControl; }// Give back a QWidget that can be used to control this class.
    bool removeTimeData(int index);//True, curve removed, false curve not found
    GenericTimeData * getTimeData(int index);//return the curve at index
    int xScaleType() {return m_xScaleType;}
    int yScaleType() {return m_yScaleType;}
    int dimension()  {return m_dimension;}

signals:

public slots:

private slots:
    void timeOptionPressed();
    void zoomPanButtonPressed();

private:
    void createControlWidget();//Create the the base contro
    void initBaseControlWidget();
    void plotSetup();
    int m_dimension;
    static const int DEFAULT_PLOT_DIMENSION=11;
    QList<GenericTimeData*> m_curveList;
    struct {
      QWidget * baseControlWidget;
      ZMP_Handler * m_zmp;//Handle zoom, panel etc
      QPushButton * toggleButtonOptionEnable;
      QPushButton * toggleButtonZMP;//Zoom Move Pan enable button
    } m_baseControl;
    QWidget * m_allControl;//The widget with all control (base and eventually extended (NOT YET IMPLEMENTED)

protected:
    int m_xScaleType;
    int m_yScaleType;
};

#endif // PLOTWIDGET_H
