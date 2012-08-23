#include "digesttimedata.h"

DigestTimeData::DigestTimeData(QList<GenericTimeData *> *curveList) :
    GenericTimeData()
{
    m_curveList=curveList;
    connect(this,SIGNAL(maxDurationUpdate(double)),this,SLOT(maxDurationChanged(double)));
    this->updateData();
}

DigestTimeData::DigestTimeData(QList<GenericTimeData*> *curveList,double duration, double SRGen) :
    GenericTimeData(duration,SRGen)
{
    m_curveList=curveList;
    connect(this,SIGNAL(maxDurationUpdate(double)),this,SLOT(maxDurationChanged(double)));
    this->updateData();
}

DigestTimeData::~DigestTimeData() {}

void DigestTimeData::maxDurationChanged(double maxDuration) {

}

void DigestTimeData::setTimeDataList(QList<GenericTimeData *> *curveList) {
    m_curveList=curveList;
    this->updateData();
}

void DigestTimeData::recalc() {
    qDebug() << "DigestTimeData::recalc() CALLED";
    Q_ASSERT(m_curveList!=NULL);
    double * digestData=this->getSignalData();
    //double * t=this->getTimeData();

    int n_dw=(this->minStartTime())*this->sampleRateGeneration();
    Q_ASSERT( n_dw >=0);
    Q_ASSERT(n_dw <=this->sampleNumber());

    int n_up=(this->minStartTime()+this->maxDuration())*this->sampleRateGeneration();
    Q_ASSERT( n_up>=0 );
    Q_ASSERT(n_up<=this->sampleNumber());

    if ( m_curveList->size()>0) {
        for (int m=0; m < m_curveList->size();m++) {
            double * tData=m_curveList->at(m)->getSignalData();
            Q_ASSERT(m_curveList->at(m)->sampleNumber()==this->sampleNumber());
            for (int n=0; n < m_curveList->at(m)->sampleNumber(); n++) {
                digestData[n]=+tData[n];
            }
        }
    }
}
