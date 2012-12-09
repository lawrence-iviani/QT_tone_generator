#ifndef PARTIALSINUSDATA_H
#define PARTIALSINUSDATA_H

#include <timedata/partialtimedata.h>
#include <CustomCurves/sinusdata.h>
#include <CustomCurves/sinusdataui.h>
#include <CTG_constants.h>
#include <math.h>

class SinusDataUI;
class SinusDataParams;
class PartialSinusData;

class PartialSinusData :  public PartialTimeData
{
    Q_OBJECT
    Q_PROPERTY(qreal amplitude READ amplitude WRITE setAmplitude)
    Q_PROPERTY(qreal frequency READ frequency WRITE setFrequency)
    Q_PROPERTY(qreal initphase READ initPhase WRITE setInitPhase)
public:

    PartialSinusData(qreal duration, qreal SRGen, QWidget *widget=0);
    PartialSinusData(qreal duration, qreal SRGen, qreal amplitude, qreal frequency, qreal initPhase , QWidget *widget=0);
    virtual ~PartialSinusData();

    void setAmplitudeFrequencyAndPhase(qreal amplitude,qreal frequency,qreal initPhase)
            {m_sinusDataParams->setAmplitudeFrequencyAndPhase(amplitude, frequency, initPhase); }
    qreal amplitude() {return m_sinusDataParams->amplitude();}
    qreal frequency() {return m_sinusDataParams->frequency();}
    qreal initPhase() {return m_sinusDataParams->initPhase();}
signals:

public slots:
    void setAmplitude(qreal amplitude) {m_sinusDataParams->setAmplitude(amplitude);}
    void setFrequency(qreal frequency) {m_sinusDataParams->setFrequency(frequency);}
    void setInitPhase(qreal initphase) { m_sinusDataParams->setInitPhase(initphase);}
protected:
    virtual void recalc();

 private:
    SinusDataParams *m_sinusDataParams;
    SinusDataUI *m_sinusDataUI;
};


#endif // PARTIALSINUSDATA_H
