#ifndef REPEATEDSQUAREDATA_H
#define REPEATEDSQUAREDATA_H

#include <errormessage.h>
#include "timedata/repeatedtimedata.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "squaredata.h"

class RepeatedSquareParams  : public RepeatedTimeDataParams, public  SquareParams
{
    Q_OBJECT
    Q_PROPERTY(qreal amplitude READ amplitude WRITE setAmplitude NOTIFY amplitudeChanged)
    Q_PROPERTY(qreal frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(qreal meanValue READ meanValue WRITE setMeanValue NOTIFY meanValueChanged)

public:
    explicit RepeatedSquareParams(QObject * parent) :
        RepeatedTimeDataParams(parent),
        SquareParams() {}
    explicit RepeatedSquareParams(RepeatedTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent) :
        RepeatedTimeDataParams(baseProperty,params,parent),
        SquareParams() {}
    explicit RepeatedSquareParams(RepeatedTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent, qreal amplitude, qreal frequency, qreal meanValue) :
        RepeatedTimeDataParams(baseProperty,params,parent),
        SquareParams(amplitude,  frequency, meanValue) {}

    inline qreal amplitude() {return m_square.amplitude();}
    inline qreal frequency() {return m_square.frequency();}
    inline qreal meanValue() {return m_square.meanValue();}

signals:
    void amplitudeChanged(qreal);
    void frequencyChanged(qreal);
    void meanValueChanged(qreal);

public slots:
    void setAmplitude(qreal amplitude) { if (m_square.setAmplitude(amplitude)) emit amplitudeChanged(amplitude);}
    void setFrequency(qreal frequency) { if (m_square.setFrequency(frequency)) emit frequencyChanged(frequency);}
    void setMeanValue(qreal meanValue) { if (m_square.setMeanValue(meanValue)) emit meanValueChanged(meanValue);}

private:
    SquareParams m_square;
};

class RepeatedSquareUI : public RepeatedTimeDataUI {
    Q_OBJECT
public:
    explicit RepeatedSquareUI(QWidget *widget = 0) :
        RepeatedTimeDataUI(widget),
        m_widget(this)
    {
        connect((QObject*)&m_widget,SIGNAL(amplitudeUIChanged(qreal)),this,SIGNAL(amplitudeUIChanged(qreal)));
        connect((QObject*)&m_widget,SIGNAL(frequencyUIChanged(qreal)),this,SIGNAL(frequencyUIChanged(qreal)));
        connect((QObject*)&m_widget,SIGNAL(meanValueUIChanged(qreal)),this,SIGNAL(meanValueUIChanged(qreal)));
        this->addWidget(&m_widget, "Repeated tone controls");
    }

signals:
    void amplitudeUIChanged(qreal);
    void frequencyUIChanged(qreal);
    void meanValueUIChanged(qreal);

public slots:
    void amplitudeUIUpdate(qreal amplitude) {m_widget.amplitudeUIUpdate(amplitude);}
    void frequencyUIUpdate(qreal frequency) {m_widget.frequencyUIUpdate(frequency);}
    void meanValueUIUpdate(qreal meanValue) {m_widget.meanValueUIUpdate(meanValue);}

private:
    SquareUI m_widget;
};

class RepeatedSquareData : public RepeatedTimeData
{
    Q_OBJECT

public:
    explicit RepeatedSquareData(QObject * parent=0) : RepeatedTimeData (parent)
    {
        init(SQUAREDATA_DEFAULT_AMPLITUDE,SQUAREDATA_DEFAULT_FREQUENCY,SQUAREDATA_DEFAULT_MEAN);
    }
    explicit RepeatedSquareData(TimePlotParams *timePlotParams, QObject * parent=0) : RepeatedTimeData(timePlotParams,parent)
    {
        init(SQUAREDATA_DEFAULT_AMPLITUDE,SQUAREDATA_DEFAULT_FREQUENCY,SQUAREDATA_DEFAULT_MEAN);
    }
    explicit RepeatedSquareData(TimePlotParams * timePlotParams, qreal amplitude, qreal frequency, qreal meanValue, QObject * parent=0)  : RepeatedTimeData(timePlotParams,parent)
    {
        init(amplitude,frequency,meanValue);
    }

protected:
    virtual void recalc();
    virtual void connectSignals();

private:
    void init(qreal amplitude, qreal frequency, qreal meanValue);

};

#endif // REPEATEDSQUAREDATA_H
