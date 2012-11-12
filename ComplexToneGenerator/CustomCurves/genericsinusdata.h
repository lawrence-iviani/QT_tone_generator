#ifndef GENERICSINUSDATA_H
#define GENERICSINUSDATA_H

#include <timedata/generictimedata.h>
#include <CustomCurves/sinusdata.h>
#include <CustomCurves/sinusdataui.h>
#include <CTG_constants.h>
#include <math.h>

class SinusDataUI;
class SinusDataParams;
class GenericTimeData;

class GenericSinusData :  public GenericTimeData
{
    Q_OBJECT
public:

    GenericSinusData(qreal duration, qreal SRGen, QWidget *widget=0);
    GenericSinusData(qreal duration, qreal SRGen, qreal amplitude, qreal frequency, qreal initPhase , QWidget *widget=0);
    virtual ~GenericSinusData();

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

#endif // GENERICSINUSDATA_H


