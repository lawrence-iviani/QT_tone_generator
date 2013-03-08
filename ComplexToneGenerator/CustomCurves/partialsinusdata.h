#ifndef PARTIALSINUSDATA_H
#define PARTIALSINUSDATA_H

#include <errormessage.h>
#include "timedata/partialtimedata.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "sinusdata.h"

class PartialSinusParams  : public PartialTimeDataParams, public  SinusParams
{
    Q_OBJECT
    Q_PROPERTY(qreal amplitude READ amplitude WRITE setAmplitude NOTIFY amplitudeChanged)
    Q_PROPERTY(qreal frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(qreal initPhase READ initPhase WRITE setInitPhase NOTIFY initPhaseChanged)

public:
    explicit PartialSinusParams(QObject * parent) :
        PartialTimeDataParams(parent),
        SinusParams() {}
    explicit PartialSinusParams(PartialTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent) :
        PartialTimeDataParams(baseProperty,params,parent),
        SinusParams() {}
    explicit PartialSinusParams(PartialTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent, qreal amplitude, qreal frequency, qreal initPhase) :
        PartialTimeDataParams(baseProperty,params,parent),
        SinusParams(amplitude,  frequency,  initPhase) {}

    inline qreal amplitude() {return m_sinus.amplitude();}
    inline qreal frequency() {return m_sinus.frequency();}
    inline qreal initPhase() {return m_sinus.initPhase();}//In degree

signals:
    void amplitudeChanged(qreal);
    void frequencyChanged(qreal);
    void initPhaseChanged(qreal);

public slots:
    void setAmplitude(qreal amplitude) { if (m_sinus.setAmplitude(amplitude)) emit amplitudeChanged(amplitude);}
    void setFrequency(qreal frequency) { if (m_sinus.setFrequency(frequency)) emit frequencyChanged(frequency);}
    void setInitPhase(qreal initPhase) { if (m_sinus.setInitPhase(initPhase)) emit initPhaseChanged(initPhase);}

private:
    SinusParams m_sinus;
};

class PartialSinusUI : public PartialTimeDataUI {
    Q_OBJECT
public:
    explicit PartialSinusUI(QWidget *widget = 0) :
        PartialTimeDataUI(widget),
        m_widget(this)
    {
        connect((QObject*)&m_widget,SIGNAL(amplitudeUIChanged(qreal)),this,SIGNAL(amplitudeUIChanged(qreal)));
        connect((QObject*)&m_widget,SIGNAL(frequencyUIChanged(qreal)),this,SIGNAL(frequencyUIChanged(qreal)));
        connect((QObject*)&m_widget,SIGNAL(initPhaseUIChanged(qreal)),this,SIGNAL(initPhaseUIChanged(qreal)));
        this->addWidget(&m_widget, "Partial tone controls");
    }

signals:
    void amplitudeUIChanged(qreal);
    void frequencyUIChanged(qreal);
    void initPhaseUIChanged(qreal);

public slots:
    void amplitudeUIUpdate(qreal amplitude) {m_widget.amplitudeUIUpdate(amplitude);}
    void frequencyUIUpdate(qreal frequency) {m_widget.frequencyUIUpdate(frequency);}
    void initPhaseUIUpdate(qreal initphase) {m_widget.initPhaseUIUpdate(initphase);}

private:
    SinusUI m_widget;
};

class PartialSinusData : public PartialTimeData
{
    Q_OBJECT

public:
    explicit PartialSinusData(QObject * parent=0) : PartialTimeData (parent)
    {
        init(SINUSDATA_DEFAULT_AMPLITUDE,SINUSDATA_DEFAULT_FREQUENCY,SINUSDATA_DEFAULT_INITPHASE);
    }
    explicit PartialSinusData(TimePlotParams *timePlotParams, QObject * parent=0) : PartialTimeData(timePlotParams,parent)
    {
        init(SINUSDATA_DEFAULT_AMPLITUDE,SINUSDATA_DEFAULT_FREQUENCY,SINUSDATA_DEFAULT_INITPHASE);
    }
    explicit PartialSinusData(TimePlotParams * timePlotParams, qreal amplitude, qreal frequency, qreal initPhase , QObject * parent=0)  : PartialTimeData(timePlotParams,parent)
    {
        init(amplitude,frequency,initPhase);
    }

protected:
    virtual void recalc();
    virtual void connectSignals();

private:
    void init(qreal amplitude, qreal frequency, qreal initPhase);

};


#endif // PARTIALSINUSDATA_H
