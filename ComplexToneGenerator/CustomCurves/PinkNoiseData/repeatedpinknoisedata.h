#ifndef REPEATEDPINKNOISEDATA_H
#define REPEATEDPINKNOISEDATA_H

#include <errormessage.h>
#include "timedata/repeatedtimedata.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "pinknoisedata.h"
#include "pinknoisegenerator.h"

class RepeatedPinkNoiseParams  : public RepeatedTimeDataParams, public  PinkNoiseParams
{
    Q_OBJECT
    Q_PROPERTY(qreal average READ average WRITE setAverage NOTIFY averageChanged)
    Q_PROPERTY(qreal gain READ gain WRITE setGain NOTIFY gainChanged)

public:
    explicit RepeatedPinkNoiseParams(QObject * parent) :
        RepeatedTimeDataParams(parent),
        PinkNoiseParams() {}
    explicit RepeatedPinkNoiseParams(RepeatedTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent) :
        RepeatedTimeDataParams(baseProperty,params,parent),
        PinkNoiseParams() {}
    explicit RepeatedPinkNoiseParams(RepeatedTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent, qreal average, qreal gain) :
        RepeatedTimeDataParams(baseProperty,params,parent),
        PinkNoiseParams(average,gain) {}

    inline qreal average() {return m_pink.average();}
    inline qreal gain() {return m_pink.gain();}

signals:
    void averageChanged(qreal);
    void gainChanged(qreal);
public slots:
    void setAverage(qreal average) { if (m_pink.setAverage(average)) emit averageChanged(average);}
    void setGain(qreal gain) { if (m_pink.setGain(gain)) emit gainChanged(gain);}

private:
    PinkNoiseParams m_pink;
};

class RepeatedPinkNoiseUI : public RepeatedTimeDataUI {
    Q_OBJECT
public:
    explicit RepeatedPinkNoiseUI(QWidget *widget = 0) :
        RepeatedTimeDataUI(widget),
        m_widget(this)
    {
        connect((QObject*)&m_widget,SIGNAL(averageUIChanged(qreal)),this,SIGNAL(averageUIChanged(qreal)));
        connect((QObject*)&m_widget,SIGNAL(gainUIChanged(qreal)),this,SIGNAL(gainUIChanged(qreal)));

        this->addWidget(&m_widget, "Repeated Pink noise controls");
    }

signals:
    void averageUIChanged(qreal);
    void gainUIChanged(qreal);

public slots:
    void averageUIUpdate(qreal average) {m_widget.averageUIUpdate(average);}
    void gainUIUpdate(qreal gain) {m_widget.gainUIUpdate(gain);}


private:
    PinkNoiseUI m_widget;
};

class RepeatedPinkNoiseData : public RepeatedTimeData
{
    Q_OBJECT

public:
    explicit RepeatedPinkNoiseData(QObject * parent=0) : RepeatedTimeData (parent)
    {
        init(PINKNOISE_DEFAULT_AVERAGE,PINKNOISE_DEFAULT_GAIN);
    }
    explicit RepeatedPinkNoiseData(TimePlotParams *timePlotParams, QObject * parent=0) : RepeatedTimeData(timePlotParams,parent)
    {
        init(PINKNOISE_DEFAULT_AVERAGE,PINKNOISE_DEFAULT_GAIN);
    }
    explicit RepeatedPinkNoiseData(TimePlotParams * timePlotParams, qreal average, qreal gain, QObject * parent=0)  : RepeatedTimeData(timePlotParams,parent)
    {
        init(average,gain);
    }

protected:
    virtual void recalc();
    virtual void connectSignals();

private:
    void init(qreal average,qreal gain);
    PinkNoiseGenerator m_generator;
};
#endif // REPEATEDPINKNOISEDATA_H
