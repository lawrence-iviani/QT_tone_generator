#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <CTG_constants.h>
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
    explicit PlotWidget(QWidget *parent = 0, int xScaleType=PlotWidget::Linear, int yScaleType=PlotWidget::Linear);
    void setAxisName(QString xName,QString yName);
    void setBothAxisScale(double xmin, double xmax,double ymin, double ymax);
    void setBothAxisScale(int xScaleType, double xmin, double xmax,int yScaleType,double ymin, double ymax);
    void setXScaleType(int xScaleType);
    void setYScaleType(int yScaleType);
    void setDimension(int pointDimension);
    int addTimeData(GenericTimeData * gtd);
    virtual QWidget * getControlWidget() {return NULL;}// Give back a QWidget that can be used to control this class. The class that inherits implement its controls widget
    bool removeTimeData(int index);//True, curve removed, false curve not found
    GenericTimeData * getTimeData(int index);//return the curve at index
    int xScaleType() {return m_xScaleType;}
    int yScaleType() {return m_yScaleType;}
    int dimension()  {return m_dimension;}



signals:
    void curveListChanged();
public slots:
  /**
    * his method grant to be called when never one of the curve stored in this class, or extended class,is modified.
    * Extended class handle the curves correctly in order to obtain the correct results.
    * All the stuff of init, object handling etc. are delegated to the inheriting class.
    */
    virtual void updatePlot() { qDebug() << "PlotWidget::dataUpdated() CALLED";this->replot();} //
protected:
    QList<GenericTimeData*> m_curveList;

    int m_xScaleType;
    int m_yScaleType;
private:
    void plotSetup();
    int m_dimension;

protected:

};

#endif // PLOTWIDGET_H
