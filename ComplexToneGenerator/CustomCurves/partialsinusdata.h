#ifndef PARTIALSINUSDATA_H
#define PARTIALSINUSDATA_H

#include <timedata/partialtimedata.h>
#include <CustomCurves/sinusdata.h>
#include <CustomCurves/sinusdataui.h>
#include <CTG_constants.h>
#include <math.h>

class SinusDataUI;
class SinusData;
class PartialSinusData;

class PartialSinusData :  public PartialTimeData
{
    Q_OBJECT
public:

    PartialSinusData(qreal duration, qreal SRGen, QWidget *widget=0);
    PartialSinusData(qreal duration, qreal SRGen, qreal amplitude, qreal frequency, qreal initPhase , QWidget *widget=0);
    virtual ~PartialSinusData();

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


#endif // PARTIALSINUSDATA_H
