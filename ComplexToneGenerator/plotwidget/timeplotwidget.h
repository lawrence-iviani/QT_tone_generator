#ifndef TIMEPLOTWIGET_H
#define TIMEPLOTWIGET_H

#include "plotwidget/plotwidget.h"
#include "timedata/digesttimedata.h"
#include "plotwidget/timeplotwidgetparams.h"
#include  <qwt_plot_marker.h>
#include  <qwt_symbol.h>
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
    virtual QWidget * getControlWidget() {return m_allControl;}
    qreal sampleRate() {return m_params.sampleRate();}
    qreal duration() {return m_params.duration();}
    qreal minTime() {return m_params.minTime();}
    DigestTimeData * getDigestCurve() {return m_digestCurve;} //return the digest curve
    virtual void setRubberBandPosition(qreal position);
    TimePlotParams * getTimePlotParams() {return &m_params;}

    const  QDomDocument* getTimePlotParametersDomDocument();
signals:

    /**
     * @brief duartionChanged This signal is emitted whenever the duration change for some reason
     * @param duration The new duration
     */
    void duartionChanged(qreal duration);

    /**
     * @brief sampleRateChanged This signal is emitted whenever the duration change for some reason
     * @param SR The new sample rate
     */
    void sampleRateChanged(qreal SR);

public slots:
    void setSampleRate(qreal SR);
    void setDuration(qreal duration);
    void ZMP_statusChanged();

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

    /**
     * @brief updateUI update the UI to the actual stored parameters (sample rate and duration).
     */
    void updateUI();

    /**
     * @brief showAllCurves show all curves stored in the widget
     */
    void showAllCurves();

    /**
     * @brief hideAllCurves stored in the widget
     */
    void hideAllCurves();

protected:
    DigestTimeData * m_digestCurve;

private:
    void createControlWidget();//Create the the base control
    void initBaseControlWidget();

    TimePlotParams m_params;
    struct {
      QFrame * baseControlWidget;
      ZMP_Handler * m_zmp;//Handle zoom, panel etc
      struct {
          QGroupBox * groupBox;
          QRadioButton * PICKER;
          QRadioButton * PAN;
          QRadioButton * ZOOM;
      } ZMP_RadioButton;//Zoom Move Pan Pick enable radio button
      ScaledSliderWidget * sliderSR;//Allows to change the sample rate of the whole generated signal
      ScaledSliderWidget * sliderDuration;//Allows to change the max duration of ALL the signals. The reference is ALWAYS t0
      QPushButton * showAllCurves;
      QPushButton * hideAllCurves;
    } m_baseControl;
    QFrame * m_allControl;//The widget with all control (base and eventually extended
};

#endif // TIMEPLOTWIGET_H
