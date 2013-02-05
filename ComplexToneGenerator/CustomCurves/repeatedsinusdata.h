#ifndef REPEATEDSINUSDATA_H
#define REPEATEDSINUSDATA_H

#include <errormessage.h>
#include "timedata/repeatedtimedata.h"
#include "plotwidget/timeplotwidgetparams.h"
#include <math.h>

class RepeatedSinusParams  : public RepeatedTimeDataParams
{
    Q_OBJECT
    Q_PROPERTY(qreal amplitude READ amplitude WRITE setAmplitude NOTIFY amplitudeChanged)
    Q_PROPERTY(qreal frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(qreal initPhase READ initPhase WRITE setInitPhase NOTIFY initPhaseChanged)

public:

    explicit RepeatedSinusParams(RepeatedTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent);
    explicit RepeatedSinusParams(RepeatedTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent, qreal amplitude, qreal frequency, qreal initPhase);

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

class RepeatedSinusUI : public RepeatedTimeDataUI {
    Q_OBJECT
public:
    explicit RepeatedSinusUI(QWidget *widget = 0);

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

class RepeatedSinusData : public RepeatedTimeData
{
    Q_OBJECT

public:
    explicit RepeatedSinusData(QObject * parent=0);
    explicit RepeatedSinusData(TimePlotParams *timePlotParams, QObject * parent=0);
    explicit RepeatedSinusData(TimePlotParams * timePlotParams, qreal amplitude, qreal frequency, qreal initPhase , QObject * parent=0);

signals:

public slots:

protected:
    virtual void recalc();
    virtual void connectSignals();

private:
    void init(qreal amplitude,qreal frequency, qreal initPhase, TimePlotParams * timePlotParams);
};

#endif // REPEATEDSINUSDATA_H
