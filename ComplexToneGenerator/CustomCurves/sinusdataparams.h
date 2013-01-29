#ifndef SINUSDATAPARAMS_H
#define SINUSDATAPARAMS_H

#include <QObject>
#include <math.h>

static qreal const SINUSDATA_DEFAULT_INITPHASE=0;
static qreal const SINUSDATA_DEFAULT_FREQUENCY=10;
static qreal const SINUSDATA_DEFAULT_AMPLITUDE=0.9;

class SinusDataParams : public QObject
{
    Q_OBJECT
public:

    SinusDataParams(QObject *object);
    SinusDataParams(qreal amplitude, qreal frequency, qreal initPhase , QObject *object);
    virtual ~SinusDataParams();
    static qreal deg2rad(qreal deg) {return deg*M_PI/180.0;}
    qreal amplitude() {return m_amplitude;}
    qreal frequency() {return m_frequency;}
    qreal initPhase() {return m_initPhase;}//In degree

signals:
    void dataUpdated();
public slots:
    void setAmplitudeFrequencyAndPhase(qreal amplitude,qreal frequency,qreal initPhase);
    void setAmplitude(qreal amplitude);
    void setFrequency(qreal frequency);
    void setInitPhase(qreal initPhase);//In degree

protected:

private:
    qreal m_amplitude;
    qreal m_frequency;
    qreal m_initPhase;
    qreal * m_sinus;
};

#endif // SINUSDATAPARAMS_H


