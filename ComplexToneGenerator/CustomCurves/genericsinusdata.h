#ifndef GENERICSINUSDATA_H
#define GENERICSINUSDATA_H

#include <errormessage.h>
#include "timedata/generictimedata.h"
#include "plotwidget/timeplotwidgetparams.h"
#include <math.h>

static qreal const SINUSDATA_DEFAULT_INITPHASE=0;
static qreal const SINUSDATA_DEFAULT_FREQUENCY=1000;
static qreal const SINUSDATA_DEFAULT_AMPLITUDE=0.9;

class GenericSinusParams  : public GenericTimeDataParams
{
    Q_OBJECT
    Q_PROPERTY(qreal amplitude READ amplitude WRITE setAmplitude NOTIFY amplitudeChanged)
    Q_PROPERTY(qreal frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(qreal initPhase READ initPhase WRITE setInitPhase NOTIFY initPhaseChanged)

public:
    explicit GenericSinusParams(GenericTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent);
    explicit GenericSinusParams(GenericTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent, qreal amplitude, qreal frequency, qreal initPhase);

    inline qreal amplitude() {return m_amplitude;}
    inline qreal frequency() {return m_frequency;}
    inline qreal initPhase() {return m_initPhase;}//In degree

signals:
    void amplitudeChanged(qreal);
    void frequencyChanged(qreal);
    void initPhaseChanged(qreal);

public slots:
    void setAmplitude(qreal amplitude);
    void setFrequency(qreal frequency);
    void setInitPhase(qreal initPhase);//In degree

private:
    qreal m_amplitude;
    qreal m_frequency;
    qreal m_initPhase;
};

class GenericSinusUI : public GenericTimeDataUI {
    Q_OBJECT
public:
    explicit GenericSinusUI(QWidget *widget = 0);

signals:
    void amplitudeUIChanged(qreal);
    void frequencyUIChanged(qreal);
    void initPhaseUIChanged(qreal);

public slots:
    void amplitudeUIUpdate(qreal amplitude);
    void frequencyUIUpdate(qreal frequency);
    void initPhaseUIUpdate(qreal initphase);

private:
    void initControlWidget();

    struct {
      ScaledSliderWidget *sliderFrequency;
      ScaledSliderWidget *sliderAmplitude;
      ScaledSliderWidget *sliderInitPhase;
    } m_sinusDataControl;

};

class GenericSinusData : public GenericTimeData
{
    Q_OBJECT

public:
    explicit GenericSinusData(QObject * parent=0);
    explicit GenericSinusData(TimePlotParams *timePlotParams, QObject * parent=0);
    explicit GenericSinusData(TimePlotParams * timePlotParams, qreal amplitude, qreal frequency, qreal initPhase , QObject * parent=0);

signals:

public slots:

protected:
    virtual void recalc();
    virtual void connectSignals();

private:
    void init(qreal amplitude, qreal frequency, qreal initPhase,TimePlotParams * timePlotParams);

};

#endif // GENERICSINUSDATA_H


