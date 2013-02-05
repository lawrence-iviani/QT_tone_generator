#ifndef REPEATEDTIMEDATA_H
#define REPEATEDTIMEDATA_H

#include <errormessage.h>
#include "timedata/partialtimedata.h"
#include "timedata/repeatedtimedataui.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "timedata/repeatedtimedataparams.h"

static qreal const REPEATEDTIMEDATA_DEFAULT_BLANK=1.0;

class RepeatedTimeDataUI;

//class RepeatedTimeDataDelegate : public DataUiHandlerDelegate
//{
//public:
//    RepeatedTimeDataDelegate(RepeatedTimeDataParams * params, RepeatedTimeDataUI * ui , QObject *parent = 0) :
//        DataUiHandlerDelegate((DataUiHandlerProperty *)params,(DataUiHandlerUI*)ui,"RTDDocument","Envelope_RTD",parent)
//    {}
//};


/**
  * The aim of this class is  provide basic function to those time data series that provide partial data and not full length data
  * The GenreicTimeData class provide the data for all the length of the buffer, also this class is designed to provide data for a subset.
  * IE, there's 30 secs  but if  you want to implement just 5 sec. Here you can find all those functionality in order to handle duration, start time
  */
class RepeatedTimeData : public PartialTimeData
{
    Q_OBJECT

public:
    explicit RepeatedTimeData(QObject * parent=0);
    explicit RepeatedTimeData(TimePlotParams * timePlotParams, QObject * parent=0);


signals:
    
public slots:

protected:

    /**
     * @brief insertSignalValue This function insert a qreal value in the sample position index and return true if the insertion is succesful, if return false the index is out of range.
     *  This overloading allows to insert the one shot curve in multiple (repeated) position.
     * @param index the sample index where the value must be inserted
     * @param value the value to insert
     * @return True if the insert was succesful.
     */
    virtual bool insertSignalValue(quint64 index, qreal value);
    virtual void connectSignals();

private slots:
    /**
     * @brief updateRepetitions recalculate the number of repetitions
     */
    void updateRepetitions();
private:
    void init(TimePlotParams *timePlotParams);

    uint m_repetitions;
};

#endif // REPEATEDTIMEDATA_H
