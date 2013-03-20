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
#include <qwt_scale_widget.h>
#include <math.h>
#include <qwt_interval.h>
#include <qwt_scale_engine.h>
#include <QPainter>
#include "timedata/generictimedata.h"
#include "zmp_handler.h"
#include "plotwidget/scrollrubberband.h"


class QwtScaleWidget;

/**
  * This class is an extension of the QwtPlot class. This class provide the following feature:
  * - handle XY scales in terms of limits and type (log and linear).
  * - a set of time data curve, these can be elaborated by extension of this class (ie in time domain and freq. domain).
  */
class PlotWidget : public QwtPlot
{
    Q_OBJECT
    Q_ENUMS( Scale )
public:
    enum Scale {Linear=0, Logarithmic=1};
    explicit PlotWidget(QWidget *widget = 0, int xScaleType=PlotWidget::Linear, int yScaleType=PlotWidget::Linear);

    void setAxisName(QString xName,QString yName);
    void setBothAxisScale(qreal xmin, qreal xmax,qreal ymin, qreal ymax);
    void setBothAxisScale(int xScaleType, qreal xmin, qreal xmax,int yScaleType,qreal ymin, qreal ymax);
    void setXScaleType(int xScaleType);
    void setYScaleType(int yScaleType);
    void setDimension(int pointDimension);
    void setPlotTitle(const QString& title);


    int xScaleType() {return m_xScaleType;}
    int yScaleType() {return m_yScaleType;}
    int dimension()  {return m_dimension;}


public slots:

protected:

    int m_xScaleType;
    int m_yScaleType;

    ScrollRubberBand * m_scrollRubberBand;//Object to scroll a vertical or horizontal line in order to display where the signal is analyzed.
private:

    void plotSetup();
    int m_dimension;

};

class PlotWidgetBackground: public QwtPlotItem
{
public:
    PlotWidgetBackground(QColor color) : //=QColor(70,150,255)) :
        m_color(color)
    {
        setZ(0.0);
    }

    virtual int rtti() const
    {
        return QwtPlotItem::Rtti_PlotUserItem;
    }

    virtual void draw(QPainter *painter,
        const QwtScaleMap &, const QwtScaleMap &yMap,
        const QRectF &rect) const
    {
        Q_UNUSED(yMap);
        QRectF r = rect;
        QColor c=m_color;
        //this->plot()->axisMaxMinor(QwtPlot::yLeft)
//        for ( int i = 100; i > -100; i -= 1 )
//        {
//            r.setBottom(yMap.transform(i - 1));
//            r.setTop(yMap.transform(i));
//            painter->fillRect(r, c);
//            c = c.dark(101);
//        }
        int ymin=(int) rect.y();
        int ymax=(int) rect.y()+rect.height();
        int step=rect.height()/70;

        for ( int i = ymax; i > ymin; i -= step )
        {
            r.setBottom(i-step);
            r.setTop(i);
            painter->fillRect(r, c);
            c = c.dark(101);
        }
    }
    QColor color() {return m_color;}
    void setColor(QColor color) {m_color=color;}

private:
    QColor m_color;
};

#endif // PLOTWIDGET_H
