#ifndef SINUSDATA_H
#define SINUSDATA_H

#include <generictimedata.h>
#include <math.h>

class SinusData : public GenericTimeData
{
    Q_OBJECT
public:
    SinusData();
    SinusData(double t0, double duration, double SRGen);
    SinusData(double frequency, double amplitude, double t0, double duration, double SRGen);
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
    double m_amplitude;
    double m_frequency;
    double m_initPhase;
    double * m_sinus;
    QWidget * m_widgetControl;
    void initControl();

    struct {
      ScaledSliderWidget * sliderFrequency;
      ScaledSliderWidget * sliderAmplitude;
      ScaledSliderWidget * sliderInitPhase;
    } m_dataControl;
};

#endif // SINUSDATA_H
