#ifndef TIMEPLOTWIGET_H
#define TIMEPLOTWIGET_H

#include "timedata/digesttimedata.h"
#include "plotwidget/plotwidget.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "plotwidget/timeplotwidgetui.h"
#include  <qwt_plot_marker.h>
#include  <qwt_symbol.h>
#include <CTG_constants.h>
#include <QRadioButton>
#include <QGroupBox>

//class TimePlotWidgetDelegate : public DataUiHandlerDelegate
//{
//public:
//    TimePlotWidgetDelegate(TimePlotParams * params, TimePlotWidgetUI * ui , QObject *parent = 0) :
//        DataUiHandlerDelegate((DataUiHandlerProperty*)params,(DataUiHandlerUI*)ui,"TimePlotWidgetDocument","TimePlotWidgetRoot",parent)
//    {}
//    virtual ~TimePlotWidgetDelegate() {}

//};

/**
  * This class specializes the PlotWidget with a data digest class a sum of all the init timedata in the prject.
  * Aleo provides:
  * Options for zoom&pan option.
  * Controls for the max duration of the time data and the SR of the project.
  * Play the digest curve.
  */
class TimePlotWidget : public PlotWidget
{
    Q_OBJECT
public:
    explicit TimePlotWidget(QWidget *widget = 0, int xScaleType=PlotWidget::Linear, int yScaleType=PlotWidget::Linear);
    ~TimePlotWidget();

    DigestTimeData * getDigestCurve() {return m_digestCurve;} //return the digest curve
    virtual void setRubberBandPosition(qreal position);
    inline QWidget * getControlWidget() {return dynamic_cast<QWidget*>(m_timePlotDelegate->getUI()); }// Give back a QWidget that can be used to control this class.
    inline DataUiHandlerProperty* getDataParameters() {return dynamic_cast<DataUiHandlerProperty*>(m_timePlotDelegate->getProperty());}
    inline DataUiHandlerDelegate* getDelegate() {return dynamic_cast<DataUiHandlerDelegate*>(m_timePlotDelegate);}

signals:

public slots:

    /**
     * @brief setAllCurvesSampleRate Set to all curves the same sample rate
     * @param SR
     */
    void setAllCurvesSampleRate(qreal samplerate);

    /**
     * @brief setAllCurvesDuration Set to all curves the same duration
     * @param duration
     */
    void setAllCurvesMaxDuration(qreal maxduration);
    

    /**
      * this function override base class function and before replot update the data in the digest curve.
      */
    virtual void updatePlot() {
        if (m_enableUpdate) {
            qDebug() << "TimePlotWidget::dataUpdated() CALLED";
            m_digestCurve->updateData();
            this->replot();
        }
    }

    /**
     * @brief setEnableUpdate enable/disble of update the widget, this is propagate to all curves
     * @param enable
     * @return the previous value
     */
    virtual bool setEnableUpdate(bool enable);

    /**
     * @brief forceRecalcAll force to recalc all the data curve and the digest curve. All the curves are recalculated by calling recalc
     */
    virtual void forceRecreateAll();

    /**
     * @brief forceUpdateAll force to update all the data curve and the digest curve. All the curves are recalculated by calling update
     */
    virtual void forceUpdateAll() ;

//    /**
//     * @brief updateUI update the UI to the actual stored parameters (sample rate and duration).
//     */
//    void updateUI();

    /**
     * @brief showAllCurves show/hide all curves stored in this widget
     * @param show
     */
    void showAllCurves(bool show);
    /**
     * @brief showAllCurves show
     */
    void showAllCurves() {showAllCurves(true);}

    /**
     * @brief showAllCurves hide all curves stored in this widget
     */
    void hideAllCurves() {showAllCurves(false);}

    /**
     * @brief enableAllCurves enable/disable all curves stored in this widget
     */
    void enableAllCurves(bool enable);


protected:
    DigestTimeData * m_digestCurve;

private:
    void init();
    void connectSignals();

    DataUiHandlerDelegate * m_timePlotDelegate;

};

#endif // TIMEPLOTWIGET_H
