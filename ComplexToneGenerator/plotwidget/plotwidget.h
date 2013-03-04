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
#include <QList>
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

    /**
      * Add a time data series to the class, return the index position in the internal list or -1 if the addition failed.
      */
    int addTimeData(GenericTimeData * gtd);
    virtual QWidget * getControlWidget() {return NULL;}// Give back a QWidget that can be used to control this class. The class that inherits implement its controls widget

    /**
      * Remove a time data series to the class, return if the data was removed.
      */
    bool removeTimeData(int index);//True, curve removed, false curve not found

    /**
     * @brief getTimeData Get back the time data at index.
     * @param index
     * @return
     */
    GenericTimeData * getTimeData(int index);//return the curve at index

    /**
      * Get back the whole list of curve stored in this instance.
      */
    const QList<GenericTimeData*> & getTimeDataList() {return (const QList<GenericTimeData*> &)m_curveList;}//return the whole list of curve stored in this instance.

    /**
      * Get back the whole list of curvename  stored in this instance.
      * The name position index in the list is consistent with the internal curves list.
      */
    QStringList  getTimeDataStringList();

    int xScaleType() {return m_xScaleType;}
    int yScaleType() {return m_yScaleType;}
    int dimension()  {return m_dimension;}
    virtual void setRubberBandPosition(qreal position) {Q_UNUSED(position);}

    inline bool isPlotUpdateEnabled() {return m_enablePlotUpdate;}

signals:
    void curveListChanged();
public slots:

    /**
     * @brief updateAndRecalc This method can be reimplmented. Now it's the same of updatePlot.
     * The method is connect to the updateData curve signal and is called when data modification happen and any eventual internal recalculation
     * and not only replota, are needed.
     */
    virtual void recalcAndUpdatePlot() {
        updatePlot();
    }

  /**
    * This method is called when never one of the curve stored in this class, or extended class,is modified.
    * This method is called when an updateAttribute signal is emitted and it's not necessary to recalculate nothihg but only replot (ie changing the color of a curve).
    * Extended class should re-implent the curves correctly in order to obtain the correct results.
    * All the stuff of init, object handling etc. are delegated to the inheriting class.
    */
    virtual void updatePlot() {
        if (m_enablePlotUpdate) {
            PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ REPLOT "));
            replot();
        }
    }

    /**
     * @brief setEnableUpdate enable/disble of update the widget, this propagate to all curves
     * @param enable
     * @return the previous value
     */
    virtual bool setEnablePlot(bool enable);

//    /**
//     * @brief forceRecalcAll force to recalc all the data curve. All the curves are recalculated by calling recalc
//     */
//    virtual void forceRecreateAll();

//    /**
//     * @brief forceUpdateAll force to update all the data curve. All the curves are recalculated by calling update
//     */
//    virtual void forceUpdateAll() ;

//    /**
//     * @brief forceUpdateUI force to update only the UI and the DOM data to be aligned.
//     */
//    virtual void forceUpdateUI();

protected:
    QList<GenericTimeData*> m_curveList;

    int m_xScaleType;
    int m_yScaleType;

    ScrollRubberBand * m_scrollRubberBand;//Object to scroll a vertical or horizontal line in order to display where the signal is analyzed.
private:
    bool m_enablePlotUpdate;
    void connectSignal();
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
