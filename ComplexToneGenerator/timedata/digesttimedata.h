#ifndef DIGESTTIMEDATA_H
#define DIGESTTIMEDATA_H


#include <timedata/generictimedata.h>
#include "plotwidget/timeplotwidgetparams.h"
#include <CTG_constants.h>

/**
  * This class inherits from GenericTimeData and handles a list of time data in order to generate a new time data calculated as a sum of all the curves into the list.
  * In the future may be more options of sum (ie average, median point by point etc.)
  */
class DigestTimeData : public GenericTimeData
{
    Q_OBJECT
public:
    DigestTimeData(QList<GenericTimeData*> * curveList);
    DigestTimeData(QList<GenericTimeData*> * curveList, TimePlotParams * timePlotParams);//double maxDuration, double SRGen);
    virtual ~DigestTimeData();

    bool inhibitRecalc() {return m_inhibitRecalc;}
    bool setInhibitRecalc(bool inhibit) {
        bool retval=m_inhibitRecalc;
        m_inhibitRecalc=inhibit;
        return retval;
    }
    
signals:
public slots:
 //   virtual void setMaxDuration(double maxDuration);//Set the max duration of this data set,  in this case i don't want a dataUpdate call
 //   virtual void setSampleRate(double SR);
    void updateData();
    void setTimeDataList(QList<GenericTimeData*> *m_curveList);
protected:

private:
    bool m_inhibitRecalc;
    virtual void recalc();
    QList<GenericTimeData*> * m_curveList;
};

#endif // DIGESTTIMEDATA_H
