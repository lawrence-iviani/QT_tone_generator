#ifndef REPEATEDSINUSDATA_H
#define REPEATEDSINUSDATA_H

#include <timedata/repeatedtimedata.h>
#include <CustomCurves/sinusdata.h>
#include <CustomCurves/sinusdataui.h>
#include <math.h>

class SinusDataUI;
class SinusDataParams;
class RepeatedTimeData;

class RepeatedSinusData :  public RepeatedTimeData
{
    Q_OBJECT
public:

    RepeatedSinusData(qreal duration, qreal SRGen, QWidget *widget=0);
    RepeatedSinusData(qreal duration, qreal SRGen, qreal amplitude, qreal frequency, qreal initPhase , QWidget *widget=0);
    virtual ~RepeatedSinusData();

    void setAmplitudeFrequencyAndPhase(qreal amplitude,qreal frequency,qreal initPhase)
            {m_sinusDataParams->setAmplitudeFrequencyAndPhase(amplitude, frequency, initPhase); }
    qreal amplitude() {return m_sinusDataParams->amplitude();}
    qreal frequency() {return m_sinusDataParams->frequency();}
    qreal initPhase() {return m_sinusDataParams->initPhase();}
signals:

public slots:

protected:
    virtual void recalc();

 private:
    SinusDataParams *m_sinusDataParams;
    SinusDataUI *m_sinusDataUI;
};
#endif // REPEATEDSINUSDATA_H
