#ifndef TIMEPLOTWIGET_H
#define TIMEPLOTWIGET_H

#include "plotwidget/plotwidget.h"
#include "timedata/digesttimedata.h"
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
    qreal sampleRate() {return m_SR;}
    qreal duration() {return m_duration;}
    qreal minTime() {return m_t0;}
    DigestTimeData * getDigestCurve() {return m_digestCurve;} //return the digest curve
    virtual void setRubberBandPosition(qreal position);
signals:
    
public slots:
    void setSampleRate(qreal SR);
    void setDuration(qreal duration);
    void ZMP_statusChanged();

    /**
      * this function override base class function and before replot update the data in the digest curve.
      */
    virtual void updatePlot() { qDebug() << "TimePlotWidget::dataUpdated() CALLED";
                                m_digestCurve->updateData();
                                this->replot();}
   // void curveListHasChanged();
protected:
    DigestTimeData * m_digestCurve;

private:
    void createControlWidget();//Create the the base control
    void initBaseControlWidget();

    qreal m_SR;
    qreal m_duration;
    qreal m_t0;
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
    } m_baseControl;
    QFrame * m_allControl;//The widget with all control (base and eventually extended
};

#endif // TIMEPLOTWIGET_H
