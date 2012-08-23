#ifndef DIGESTTIMEDATA_H
#define DIGESTTIMEDATA_H


#include <generictimedata.h>
#include <CTG_constants.h>

class DigestTimeData : public GenericTimeData
{
    Q_OBJECT
public:
    DigestTimeData(QList<GenericTimeData*> * curveList);
    DigestTimeData(QList<GenericTimeData*> * curveList, double maxDuration, double SRGen);
    virtual ~DigestTimeData();
    
signals:
    
public slots:
    void maxDurationChanged(double maxDuration);//Call this slot if the duration of the whole signal is changed in the base class.
    void setTimeDataList(QList<GenericTimeData*> *m_curveList);
protected:
    virtual void recalc();
private:
    QList<GenericTimeData*> * m_curveList;
    
};

#endif // DIGESTTIMEDATA_H
