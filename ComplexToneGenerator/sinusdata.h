#ifndef SINUSDATA_H
#define SINUSDATA_H

#include <partialtimedata.h>
#include <CTG_constants.h>
#include <math.h>

class SinusData : public PartialTimeData
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

signals:
    
public slots:
    void setAmplitudeFrequencyAndPhase(double amplitude,double frequency,double initPhase);
    void setAmplitude(double amplitude);
    void setFrequency(double frequency);
    void setInitPhase(double initPhase);//In degree

protected:
    virtual void recalc();

 private:
    static double const SINUSDATA_DEFAULT_INITPHASE=0;
    static double const SINUSDATA_DEFAULT_FREQUENCY=10;
    static double const SINUSDATA_DEFAULT_AMPLITUDE=0.9;

    double m_amplitude;
    double m_frequency;
    double m_initPhase;
    double * m_sinus;
    QFrame * m_widgetControl;
    void initControl();

    struct {
      ScaledSliderWidget * sliderFrequency;
      ScaledSliderWidget * sliderAmplitude;
      ScaledSliderWidget * sliderInitPhase;
    } m_dataControl;
};

#endif // SINUSDATA_H
