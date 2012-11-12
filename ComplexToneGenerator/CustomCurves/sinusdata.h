#ifndef SINUSDATA_H
#define SINUSDATA_H

#include <QObject>
#include <CustomCurves/sinusdataui.h>
#include <math.h>

class SinusDataUI;

class SinusData : public QObject
{
    Q_OBJECT
public:

    SinusData(QObject *object);
    SinusData(qreal amplitude, qreal frequency, qreal initPhase , QObject *object);
    virtual ~SinusData();
    static qreal deg2rad(qreal deg) {return deg*M_PI/180.0;}
    qreal amplitude() {return m_amplitude;}
    qreal frequency() {return m_frequency;}
    qreal initPhase() {return m_initPhase;}//In degree
    CustomCurveUI * getControlUI() {return ( CustomCurveUI *)m_sinusDataUI;}

signals:
    void dataUpdated();
public slots:
    void setAmplitudeFrequencyAndPhase(qreal amplitude,qreal frequency,qreal initPhase);
    void setAmplitude(qreal amplitude);
    void setFrequency(qreal frequency);
    void setInitPhase(qreal initPhase);//In degree

protected:

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

#endif // SINUSDATA_H


