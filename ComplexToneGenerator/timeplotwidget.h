#ifndef TIMEPLOTWIGET_H
#define TIMEPLOTWIGET_H

#include "plotwidget.h"
#include "digesttimedata.h"
#include  <qwt_plot_marker.h>
#include  <qwt_symbol.h>
#include <CTG_constants.h>

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
    explicit TimePlotWidget(QWidget *parent = 0, int xScaleType=PlotWidget::Linear, int yScaleType=PlotWidget::Linear);
    ~TimePlotWidget();
    virtual QWidget * getControlWidget() {return m_allControl;}
    double sampleRate() {return m_SR;}
    double duration() {return m_duration;}
    double minTime() {return m_t0;}
    DigestTimeData * getDigestCurve() {return m_digestCurve;} //return the digest curve
    virtual void setPosition(qreal position);
signals:
    
public slots:
    void setSampleRate(double SR);
    void setDuration(double duration);
    void timeOptionPressed();
    void zoomPanButtonPressed();

    /**
      * this function override base class function and before replot update the data in the digest curve.
      */
    virtual void updatePlot() { qDebug() << "TimePlotWidget::dataUpdated() CALLED"; m_digestCurve->updateData(); this->replot();}
   // void curveListHasChanged();
protected:
    DigestTimeData * m_digestCurve;

private:
    void createControlWidget();//Create the the base control
    void initBaseControlWidget();

    double m_SR;
    double m_duration;
    double m_t0;
    struct {
      QFrame * baseControlWidget;
      ZMP_Handler * m_zmp;//Handle zoom, panel etc
      QPushButton * toggleButtonOptionEnable;
      QPushButton * toggleButtonZMP;//Zoom Move Pan enable button
      ScaledSliderWidget * sliderSR;//Allows to change the sample rate of the whole generated signal
      ScaledSliderWidget * sliderDuration;//Allows to change the max duration of ALL the signals. The reference is ALWAYS t0
    } m_baseControl;
    QFrame * m_allControl;//The widget with all control (base and eventually extended
    QwtPlotMarker * m_markerPosition;
};

#endif // TIMEPLOTWIGET_H
