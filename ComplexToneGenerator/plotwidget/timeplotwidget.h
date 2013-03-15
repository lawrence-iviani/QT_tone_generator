#ifndef TIMEPLOTWIGET_H
#define TIMEPLOTWIGET_H


#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <CTG_constants.h>
#include <QRadioButton>
#include <QGroupBox>
#include <QList>
#include "timedata/digesttimedata.h"
#include "plotwidget/plotwidget.h"
#include "plotwidget/freqplotwidget.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "plotwidget/timeplotwidgetui.h"


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
    inline DataUiHandlerProperty* getDataParameters() {return m_timePlotDelegate->getProperty();}
    inline QDomDocument exportXML() {return getDelegate()->getDomDocument();}
    friend class TimePlotParams;

    /**
      * Add a time data series to the class, return the index position in the internal list or -1 if the addition failed.
      */
    int addTimeData(GenericTimeData * gtd);

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

    /**
     * @brief setFreqWidget Set the a frequency plot to this class, every update in curves and also well be managed also by this widget.
     * @param plotWidget
     */
    void setFreqWidget(FreqPlotWidget *freqPlotWidget);

    inline bool isPlotUpdateEnabled() {return m_enablePlotUpdate;}

signals:
    void curveListChanged();

public slots: 

    /**
     * @brief setEnableUpdate enable/disble of update the widget, this propagate to all curves
     * @param enable
     * @return the previous value
     */
     bool setEnablePlot(bool enable);

    /**
     * This method is called when never one of the curve stored in this class, or extended class,is modified.
     * This method is called when an updateAttribute signal is emitted and it's not necessary to recalculate nothihg but only replot (ie changing the color of a curve).
     * Extended class should re-implent the curves correctly in order to obtain the correct results.
     * All the stuff of init, object handling etc. are delegated to the inheriting class.
     */
     void updatePlot() {
         if (m_enablePlotUpdate) {
             PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ REPLOT "));
             replot();
             if (m_freqPlot) m_freqPlot->dataUpdated();
         }
     }

    /**
     * @brief updateAndRecalc Override base method and recall a digest update.
     */
    void recalcAndUpdatePlot() {
        if (isPlotUpdateEnabled()) {
            PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ Digest Update "));
            m_digestCurve->updateData();
            updatePlot();
        }
    }

    /**
     * @brief importXML Import a DOM document containing the appropriate params
     * @param doc
     * @param err
     * @return true on success, false on fail. On fail the err field is filled
     */
    bool importXML(const QDomDocument& doc, ErrorMessage *err=NULL) {return m_timePlotDelegate->setClassByDomData(doc,true,err);}

    /**
     * @brief importXML Import a DOM node, with relative sun mode, containing the appropriate params
     * @param node
     * @param err
     * @return true on success, false on fail. On fail the err field is filled
     */
    bool importXML(const QDomNode& node, ErrorMessage *err=NULL)  {return m_timePlotDelegate->setClassByDomData(node,true,err);}


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
    QList<GenericTimeData*> m_curveList;

    /**
     * @brief connectSignals WHen inheriting from this class this method must be called after init to mantains connection, otherwise will be lost.
     * (This is an issue that must be fixed in the design).
     */
     void connectSignals();

    inline DataUiHandlerDelegate* getDelegate() {return dynamic_cast<DataUiHandlerDelegate*>(m_timePlotDelegate);}

protected slots:
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


private:
    FreqPlotWidget* m_freqPlot;
    bool m_enablePlotUpdate;
    void init();
    DataUiHandlerDelegate * m_timePlotDelegate;

};

#endif // TIMEPLOTWIGET_H
