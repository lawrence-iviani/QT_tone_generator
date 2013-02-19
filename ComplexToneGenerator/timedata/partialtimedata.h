#ifndef PARTIALTIMEDATA_H
#define PARTIALTIMEDATA_H

#include "timedata/generictimedata.h"
#include "timedata/partialtimedataui.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "timedata/partialtimedataparams.h"

static qreal const PARTIALTIMEDATA_DEFAULT_DURATION=5.0;
static qreal const PARTIALTIMEDATA_DEFAULT_T0=0.0;

class PartialTimeDataUI;

//class PartialTimeDataDelegate : public DataUiHandlerDelegate
//{
//public:
//    PartialTimeDataDelegate(PartialTimeDataParams * params, PartialTimeDataUI * ui , QObject *parent = 0) :
//        DataUiHandlerDelegate((DataUiHandlerProperty *)params,(DataUiHandlerUI*)ui,"PTDDocument","Envelope_PTD",parent)
//    {}
//};


/**
  * The aim of this class is  provide basic function to those time data series that provide partial data and not full length data
  * The GenreicTimeData class provide the data for all the length of the buffer, also this class is designed to provide data for a subset.
  * IE, there's 30 secs  but if  you want to implement just 5 sec. Here you can find all those functionality in order to handle duration, start time
  */
class PartialTimeData : public GenericTimeData
{
    Q_OBJECT

public:

    explicit PartialTimeData(QObject * parent=0);
    explicit PartialTimeData(TimePlotParams * timePlotParams, QObject * parent=0);
    
signals:

public slots:
    /**
      * Set the max duration of this signal and update for data, redefine in the inerithed class if you want change beaviour. (ie don't call an update)
      * you have to change the singal length. Tipically you should override this method.
      */
    virtual void maxDurationHasChanged(qreal maxDuration);

protected:

    /**
      * Return the lowest index available to modify the internal curve, is needed in inerithed class in order to insert correct recalculated values
      * Reimplemented from GenericTimeData.
      */
    virtual const quint64 lowestSampleIndexForModification();

    /**
      * Return the lowest index available to modify the internal curve, is needed in inerithed class in order to insert correct recalculated values
      * Reimplemented from GenericTimeData.
      */
    virtual const quint64 highestSampleIndexForModification();

    virtual void connectSignals();
private:
    void init(TimePlotParams *timePlotParams);
    void privateSetDuration(qreal duration);

private slots:
  //  void maxDurationChange(qreal maxduration);
};

#endif // PARTIALTIMEDATA_H
