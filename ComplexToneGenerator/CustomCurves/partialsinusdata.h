#ifndef PARTIALSINUSDATA_H
#define PARTIALSINUSDATA_H

#include <timedata/partialtimedata.h>
#include <CustomCurves/sinusdataui.h>
#include <CTG_constants.h>
#include <math.h>

class SinusDataUI;

class PartialSinusData : public PartialTimeData
{
    Q_OBJECT
public:
    PartialSinusData(qreal duration, qreal SRGen, QWidget *widget=0);
    PartialSinusData(qreal duration, qreal SRGen, qreal amplitude, qreal frequency, qreal initPhase , QWidget *widget=0);
    virtual ~PartialSinusData();
    static qreal deg2rad(qreal deg) {return deg*M_PI/180.0;}
    qreal amplitude() {return m_amplitude;}
    qreal frequency() {return m_frequency;}
    qreal initPhase() {return m_initPhase;}//In degree

signals:

public slots:
    void setAmplitudeFrequencyAndPhase(qreal amplitude,qreal frequency,qreal initPhase);
    void setAmplitude(qreal amplitude);
    void setFrequency(qreal frequency);
    void setInitPhase(qreal initPhase);//In degree

protected:
    virtual void recalc();

 private:
    static qreal const SINUSDATA_DEFAULT_INITPHASE=0;
    static qreal const SINUSDATA_DEFAULT_FREQUENCY=10;
    static qreal const SINUSDATA_DEFAULT_AMPLITUDE=0.9;

    qreal m_amplitude;
    qreal m_frequency;
    qreal m_initPhase;
    qreal * m_sinus;
    SinusDataUI *m_sinusDataUI;
};

#endif // PARTIALSINUSDATA_H
