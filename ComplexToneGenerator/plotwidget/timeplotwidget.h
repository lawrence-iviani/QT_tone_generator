#ifndef TIMEPLOTWIGET_H
#define TIMEPLOTWIGET_H

#include "timedata/digesttimedata.h"
#include "plotwidget/plotwidget.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "plotwidget/timeplotwidgetui.h"
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <CTG_constants.h>
#include <QRadioButton>
#include <QGroupBox>

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
signals:

public slots: 

    /**
     * @brief setEnableUpdate enable/disble of update the widget, this propagate to all curves
     * @param enable
     * @return the previous value
     */
    virtual bool setEnablePlot(bool enable);

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
     * @brief updateAndRecalc Override base method and recall a digest update.
     */
    virtual void recalcAndUpdatePlot() {
        if (isPlotUpdateEnabled()) {
            PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ Digest Update "));
            m_digestCurve->updateData();
            updatePlot();
        }
    }

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

    /**
     * @brief connectSignals WHen inheriting from this class this method must be called after init to mantains connection, otherwise will be lost.
     * (This is an issue that must be fixed in the design).
     */
    virtual void connectSignals();

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
    void init();
    DataUiHandlerDelegate * m_timePlotDelegate;

};

#endif // TIMEPLOTWIGET_H
