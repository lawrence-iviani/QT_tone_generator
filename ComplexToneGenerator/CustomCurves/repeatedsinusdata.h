#ifndef REPEATEDSINUSDATA_H
#define REPEATEDSINUSDATA_H

#include <timedata/repeatedtimedata.h>
#include <CustomCurves/sinusdata.h>
#include <CustomCurves/sinusdataui.h>
#include <math.h>

class SinusDataUI;
class SinusData;
class RepeatedTimeData;

class RepeatedSinusData :  public RepeatedTimeData
{
    Q_OBJECT
public:

    RepeatedSinusData(qreal duration, qreal SRGen, QWidget *widget=0);
    RepeatedSinusData(qreal duration, qreal SRGen, qreal amplitude, qreal frequency, qreal initPhase , QWidget *widget=0);
    virtual ~RepeatedSinusData();

    void setAmplitudeFrequencyAndPhase(qreal amplitude,qreal frequency,qreal initPhase)
            {m_sinusData->setAmplitudeFrequencyAndPhase(amplitude, frequency, initPhase); }
    qreal amplitude() {return m_sinusData->amplitude();}
    qreal frequency() {return m_sinusData->frequency();}
    qreal initPhase() {return m_sinusData->initPhase();}
signals:

public slots:

protected:
    virtual void recalc();

 private:
    SinusData *m_sinusData;
    SinusDataUI *m_sinusDataUI;
};
#endif // REPEATEDSINUSDATA_H
