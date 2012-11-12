#ifndef GENERICSINUSDATA_H
#define GENERICSINUSDATA_H

#include <timedata/generictimedata.h>
#include <CustomCurves/sinusdata.h>
#include <CustomCurves/sinusdataui.h>
#include <CTG_constants.h>
#include <math.h>

class SinusDataUI;
class SinusData;
class GenericTimeData;

class GenericSinusData :  public GenericTimeData
{
    Q_OBJECT
public:

    GenericSinusData(qreal duration, qreal SRGen, QWidget *widget=0);
    GenericSinusData(qreal duration, qreal SRGen, qreal amplitude, qreal frequency, qreal initPhase , QWidget *widget=0);
    virtual ~GenericSinusData();

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

#endif // GENERICSINUSDATA_H


