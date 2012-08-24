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
    virtual void setMaxDuration(double maxDuration);//Set the max duration of this data set,  in this case i don't want a dataUpdate call
    virtual void setSampleRate(double SR);
    void setTimeDataList(QList<GenericTimeData*> *m_curveList);
protected:

private:
    virtual void recalc();
    QList<GenericTimeData*> * m_curveList;
    
};

#endif // DIGESTTIMEDATA_H
