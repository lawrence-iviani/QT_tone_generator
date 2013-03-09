#ifndef GENERICSQUAREDATA_H
#define GENERICSQUAREDATA_H

#include <errormessage.h>
#include "timedata/generictimedata.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "squaredata.h"

class GenericSquareParams  : public GenericTimeDataParams, public  SquareParams
{
    Q_OBJECT
    Q_PROPERTY(qreal amplitude READ amplitude WRITE setAmplitude NOTIFY amplitudeChanged)
    Q_PROPERTY(qreal frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(qreal meanValue READ meanValue WRITE setMeanValue NOTIFY meanValueChanged)

public:
    explicit GenericSquareParams(QObject * parent) :
        GenericTimeDataParams(parent),
        SquareParams() {}
    explicit GenericSquareParams(GenericTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent) :
        GenericTimeDataParams(baseProperty,params,parent),
        SquareParams() {}
    explicit GenericSquareParams(GenericTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent, qreal amplitude, qreal frequency, qreal meanValue) :
        GenericTimeDataParams(baseProperty,params,parent),
        SquareParams(amplitude,  frequency,meanValue) {}

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

class GenericSquareUI : public GenericTimeDataUI {
    Q_OBJECT
public:
    explicit GenericSquareUI(QWidget *widget = 0) :
        GenericTimeDataUI(widget),
        m_widget(this)
    {
        connect((QObject*)&m_widget,SIGNAL(amplitudeUIChanged(qreal)),this,SIGNAL(amplitudeUIChanged(qreal)));
        connect((QObject*)&m_widget,SIGNAL(frequencyUIChanged(qreal)),this,SIGNAL(frequencyUIChanged(qreal)));
        connect((QObject*)&m_widget,SIGNAL(meanValueUIChanged(qreal)),this,SIGNAL(meanValueUIChanged(qreal)));
        this->addWidget(&m_widget, "Square wave controls");
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

class GenericSquareData : public GenericTimeData
{
    Q_OBJECT

public:
    explicit GenericSquareData(QObject * parent=0) : GenericTimeData (parent)
    {
        init(SQUAREDATA_DEFAULT_AMPLITUDE,SQUAREDATA_DEFAULT_FREQUENCY,SQUAREDATA_DEFAULT_MEAN);
    }
    explicit GenericSquareData(TimePlotParams *timePlotParams, QObject * parent=0) : GenericTimeData(timePlotParams,parent)
    {
        init(SQUAREDATA_DEFAULT_AMPLITUDE,SQUAREDATA_DEFAULT_FREQUENCY,SQUAREDATA_DEFAULT_MEAN);
    }
    explicit GenericSquareData(TimePlotParams * timePlotParams, qreal amplitude, qreal frequency, qreal meanValue, QObject * parent=0)  : GenericTimeData(timePlotParams,parent)
    {
        init(amplitude,frequency, meanValue);
    }

protected:
    virtual void recalc();
    virtual void connectSignals();

private:
    void init(qreal amplitude, qreal frequency, qreal meanValue);

};

#endif // GENERICSQUAREDATA_H


