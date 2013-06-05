#ifndef GENERICPINKNOISEDATA_H
#define GENERICPINKNOISEDATA_H

#include <errormessage.h>
#include "timedata/generictimedata.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "pinknoisedata.h"
#include "pinknoisegenerator.h"

class GenericPinkNoiseParams  : public GenericTimeDataParams, public  PinkNoiseParams
{
    Q_OBJECT
    Q_PROPERTY(qreal average READ average WRITE setAverage NOTIFY averageChanged)
    Q_PROPERTY(qreal gain READ gain WRITE setGain NOTIFY gainChanged)

public:
    explicit GenericPinkNoiseParams(QObject * parent) :
        GenericTimeDataParams(parent),
        PinkNoiseParams() {}
    explicit GenericPinkNoiseParams(GenericTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent) :
        GenericTimeDataParams(baseProperty,params,parent),
        PinkNoiseParams() {}
    explicit GenericPinkNoiseParams(GenericTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent, qreal average, qreal gain) :
        GenericTimeDataParams(baseProperty,params,parent),
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

class GenericPinkNoiseUI : public GenericTimeDataUI {
    Q_OBJECT
public:
    explicit GenericPinkNoiseUI(QWidget *widget = 0) :
        GenericTimeDataUI(widget),
        m_widget(this)
    {
        connect((QObject*)&m_widget,SIGNAL(averageUIChanged(qreal)),this,SIGNAL(averageUIChanged(qreal)));
        connect((QObject*)&m_widget,SIGNAL(gainUIChanged(qreal)),this,SIGNAL(gainUIChanged(qreal)));

        this->addWidget(&m_widget, "Generic Pink noise controls");
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

class GenericPinkNoiseData : public GenericTimeData
{
    Q_OBJECT

public:
    explicit GenericPinkNoiseData(QObject * parent=0) : GenericTimeData (parent)
    {
        init(PINKNOISE_DEFAULT_AVERAGE,PINKNOISE_DEFAULT_GAIN);
    }
    explicit GenericPinkNoiseData(TimePlotParams *timePlotParams, QObject * parent=0) : GenericTimeData(timePlotParams,parent)
    {
        init(PINKNOISE_DEFAULT_AVERAGE,PINKNOISE_DEFAULT_GAIN);
    }
    explicit GenericPinkNoiseData(TimePlotParams * timePlotParams, qreal average, qreal gain, QObject * parent=0)  : GenericTimeData(timePlotParams,parent)
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
#endif // GENERICPINKNOISEDATA_H
