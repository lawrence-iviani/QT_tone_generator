#ifndef PARTIALTIMEDATA_H
#define PARTIALTIMEDATA_H

#include <generictimedata.h>

/**
  * The aim of this class is  provide basic function to those time data series that provide partial data and not full length data
  * The GenreicTimeData class provide the data for all the length of the buffer, also this class is designed to provide data for a subset.
  * IE, there's 30 secs of but you want to implement just 5 sec. Here you can find all those functionality in order to handle duration, start time
  */
class PartialTimeData : public GenericTimeData
{
    Q_OBJECT
public:
    PartialTimeData();
    PartialTimeData(double maxDuration, double SRGen);
    
    double startTime() {return m_t0;}
    double duration() {return m_duration;}
signals:
    
public slots:
    void setDuration(double duration);//Set the duration of this signal, it will be clipped by the base class if the min or max time values are outside the limits of the base class
    void setStartTime(double t0);//Set the min start time of this signal, it will be clipped by the base class if the min or max time values are outside the limits of the base class
    //void minStartTimeChanged(double t0);//Call this slot if some lower time limit is changed in the base class.
    virtual void setMaxDuration(double maxDuration);

protected:

    /**
      * This struct contains controls useful to be integrated in the inerhit class.
      * This means that this struct should be inserted in the widget that will be associated to the method
      * of an instance of GenericTimeData::setControlWidget(QWidget *). In this case will possible add the control implemented in this struct
      */
    struct {
      ScaledSliderWidget * widgetDuration;
      ScaledSliderWidget * widget_t0;
    } m_partialDataControl;

private:
    static double const TIMEDATA_DEFAULT_DURATION=5.0;
    static double const TIMEDATA_DEFAULT_INITTIME=0.0;

    double m_t0;
    double m_duration;


    void initControl();
};

#endif // PARTIALTIMEDATA_H
