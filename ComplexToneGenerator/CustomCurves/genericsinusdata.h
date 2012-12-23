#ifndef GENERICSINUSDATA_H
#define GENERICSINUSDATA_H

#include "timedata/generictimedata.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "CustomCurves/sinusdata.h"
#include "CustomCurves/sinusdataui.h"
#include "CTG_constants.h"
#include <math.h>

class SinusDataUI;
class SinusDataParams;
class GenericTimeData;

class GenericSinusData :  public GenericTimeData
{
    Q_OBJECT
    Q_PROPERTY(qreal amplitude READ amplitude WRITE setAmplitude)
    Q_PROPERTY(qreal frequency READ frequency WRITE setFrequency)
    Q_PROPERTY(qreal initphase READ initPhase WRITE setInitPhase)

public:
    GenericSinusData(QWidget *widget=0);
    GenericSinusData(TimePlotParams *timePlotParams, QWidget *widget=0);
    GenericSinusData(TimePlotParams * timePlotParams, qreal amplitude, qreal frequency, qreal initPhase , QWidget *widget=0);
    virtual ~GenericSinusData();

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

#endif // GENERICSINUSDATA_H


