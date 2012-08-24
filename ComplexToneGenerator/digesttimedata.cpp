#include "digesttimedata.h"

DigestTimeData::DigestTimeData(QList<GenericTimeData *> *curveList) :
    GenericTimeData()
{
    m_curveList=curveList;
    //connect(this,SIGNAL(maxDurationUpdate(double)),this,SLOT(maxDurationChanged(double)));
    this->updateData();
}

DigestTimeData::DigestTimeData(QList<GenericTimeData*> *curveList,double duration, double SRGen) :
    GenericTimeData(duration,SRGen)
{
    m_curveList=curveList;
    //connect(this,SIGNAL(maxDurationUpdate(double)),this,SLOT(maxDurationChanged(double)));
    this->updateData();
}

DigestTimeData::~DigestTimeData() {}

void DigestTimeData::setMaxDuration(double maxDuration) {
    this->setMaxDurationAndUpdate(maxDuration,false);
}

void DigestTimeData::setSampleRate(double SR) {
    this->setSampleRateAndUpdate(SR,false);
}

void DigestTimeData::setTimeDataList(QList<GenericTimeData *> *curveList) {
    m_curveList=curveList;
    this->updateData();
}

void DigestTimeData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz")  << " - DigestTimeData::recalc() ---------------- " << this->name();

    Q_ASSERT(m_curveList!=NULL);
    double * digestData=this->getSignalData();
    GenericTimeData * gtd;
    double * gtdData;

    int n_dw=(this->minStartTime())*this->sampleRateGeneration();
    qDebug() << " m_min_t0=" << this->minStartTime() << " n_dw=" << n_dw << " NSample=" << this->sampleNumber();
    Q_ASSERT( n_dw >=0);
    Q_ASSERT(n_dw <=this->sampleNumber());

    int n_up=(this->minStartTime()+this->maxDuration())*this->sampleRateGeneration();
    qDebug() << "m_max_Duration=" << this->maxDuration()  << " n_up=" << n_up << " NSample=" << this->sampleNumber();
    Q_ASSERT( n_up>=0 );
    Q_ASSERT(n_up<=this->sampleNumber());

    if ( m_curveList->size()>0) {
        for (int m=0; m < m_curveList->size();m++) {
            gtd=m_curveList->at(m);
            Q_ASSERT(gtd!=NULL);
            if (m_curveList->at(m)->isEnabled()) {
                gtdData=gtd->getSignalData();
                Q_ASSERT(gtd->sampleNumber()==this->sampleNumber());
                for (int n=0; n < gtd->sampleNumber(); n++) {
                    digestData[n]+=gtdData[n];
                }
            }
        }
    }
}
