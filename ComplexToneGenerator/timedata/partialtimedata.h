#ifndef PARTIALTIMEDATA_H
#define PARTIALTIMEDATA_H

#include <timedata/generictimedata.h>
#include <timedata/partialtimedataui.h>
#include <CustomCurves/customcurveui.h>

class PartialTimeDataUI;

/**
  * The aim of this class is  provide basic function to those time data series that provide partial data and not full length data
  * The GenreicTimeData class provide the data for all the length of the buffer, also this class is designed to provide data for a subset.
  * IE, there's 30 secs  but if  you want to implement just 5 sec. Here you can find all those functionality in order to handle duration, start time
  */
class PartialTimeData : public GenericTimeData
{
    Q_OBJECT
public:
    PartialTimeData(QObject *parent=0,QWidget * widget=0);
    PartialTimeData(qreal maxDuration, qreal SRGen, QObject *parent=0,QWidget *widget=0);
    
    qreal startTime() {return m_t0;}
    qreal duration() {return m_duration;}
signals:
    
public slots:
    void setDuration(qreal duration);//Set the duration of this signal, it will be clipped by the base class if the min or max time values are outside the limits of the base class
    void setStartTime(qreal t0);//Set the min start time of this signal, it will be clipped by the base class if the min or max time values are outside the limits of the base class
    //void minStartTimeChanged(qreal t0);//Call this slot if some lower time limit is changed in the base class.
    virtual void setMaxDuration(qreal maxDuration);

protected:

    /**
      * Return the lowest index available to modify the internal curve, is needed in inerithed class in order to insert correct recalculated values
      * Reimplemented from GenericTimeData.
      */
    qint64 lowestSampleIndexForModification()  {
        qint64 retval=(this->startTime()-this->minStartTime())*this->sampleRate();
        Q_ASSERT(retval >=0);
        Q_ASSERT(retval <=this->sampleNumber());
        return retval;
    }

    /**
      * Return the lowest index available to modify the internal curve, is needed in inerithed class in order to insert correct recalculated values
      * Reimplemented from GenericTimeData.
      */
    qint64 highestSampleIndexForModification() {
        qint64 retval=(this->startTime()+this->duration())*this->sampleRate();
        Q_ASSERT(retval >=0);
        Q_ASSERT(retval <=this->sampleNumber());
        return retval;
    }

private:
    void connectSignal();
    static qreal const TIMEDATA_DEFAULT_DURATION=5.0;
    static qreal const TIMEDATA_DEFAULT_INITTIME=0.0;

    PartialTimeDataUI *m_partialTimeDataUI;
    qreal m_t0;
    qreal m_duration;
};

#endif // PARTIALTIMEDATA_H
