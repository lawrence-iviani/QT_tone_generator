#ifndef SINUSDATA_H
#define SINUSDATA_H

#include <generictimedata.h>
#include <CTG_constants.h>
#include <math.h>

class SinusData : public GenericTimeData
{
    Q_OBJECT
public:
    SinusData();
    SinusData(double duration, double SRGen);
    SinusData(double duration,double SRGen, double amplitude, double frequency, double initPhase );
    virtual ~SinusData();
    static double deg2rad(double deg) {return deg*M_PI/180.0;}
    double amplitude() {return m_amplitude;}
    double frequency() {return m_frequency;}
    double initPhase() {return m_initPhase;}//In degree
    double startTime() {return m_t0;}
    double duration() {return m_duration;}

signals:
    
public slots:
    void setAmplitudeFrequencyAndPhase(double amplitude,double frequency,double initPhase);
    void setAmplitude(double amplitude);
    void setFrequency(double frequency);
    void setInitPhase(double initPhase);//In degree
    void setDuration(double duration);//Set the duration of this signal, it will be clipped by the base class if the min or max time values are outside the limits of the base class
    void setStartTime(double t0);//Set the min start time of this signal, it will be clipped by the base class if the min or max time values are outside the limits of the base class
    //void minStartTimeChanged(double t0);//Call this slot if some lower time limit is changed in the base class.
    virtual void setMaxDuration(double maxDuration);//

protected:
    virtual void recalc();

 private:
    static double const SINUSDATA_DEFAULT_INITPHASE=0;
    static double const SINUSDATA_DEFAULT_FREQUENCY=10;
    static double const SINUSDATA_DEFAULT_AMPLITUDE=0.9;
    static double const SINUSDATA_DEFAULT_DURATION=1.5;
    static double const SINUSDATA_DEFAULT_INITTIME=2.1;

    double m_amplitude;
    double m_frequency;
    double m_initPhase;
    double m_t0;
    double m_duration;
    double * m_sinus;
    QFrame * m_widgetControl;
    void initControl();

    struct {
      ScaledSliderWidget * sliderFrequency;
      ScaledSliderWidget * sliderAmplitude;
      ScaledSliderWidget * sliderInitPhase;
      ScaledSliderWidget * sliderDuration;
      ScaledSliderWidget * slider_t0;
    } m_dataControl;
};

#endif // SINUSDATA_H
