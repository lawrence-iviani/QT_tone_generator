#include "digesttimedata.h"

DigestTimeData::DigestTimeData(QList<GenericTimeData *> *curveList) :
    GenericTimeData()
{
    m_curveList=curveList;
    this->updateData();
}

DigestTimeData::DigestTimeData(QList<GenericTimeData*> *curveList,qreal duration, qreal SRGen) :
    GenericTimeData(duration,SRGen)
{
    m_curveList=curveList;
    this->updateData();
}

DigestTimeData::~DigestTimeData() {}

//Due to the fact the digest list is updated when all the curves are updated, this means that
// for example, SR o duration change this will updated twice
void DigestTimeData::setMaxDuration(qreal maxDuration) {
    this->setMaxDurationAndUpdate(maxDuration,false);
}

void DigestTimeData::setSampleRate(qreal SR) {
    this->setSampleRateAndUpdate(SR,false);
}

void DigestTimeData::setTimeDataList(QList<GenericTimeData *> *curveList) {
    m_curveList=curveList;
    this->updateData();
}

void DigestTimeData::updateData() {
    //Need to force a reset, because the digest list is updated when all the curves are updated, this means that
    // for example, SR o duration change this will updated twice. In this case force a reset before go on.
    DigestTimeData::createData();
}

void DigestTimeData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz")  << " - DigestTimeData::recalc() ---------------- " << this->name();

    Q_ASSERT(m_curveList!=NULL);
    const qreal * digestData=this->getSignalData();
    GenericTimeData * gtd;

    if ( m_curveList->size()>0) {
        for (int m=0; m < m_curveList->size();m++) {
            gtd=m_curveList->at(m);
            Q_ASSERT(gtd!=NULL);
            if (m_curveList->at(m)->isEnabled()) {
                const qreal * gtdData=gtd->getSignalData();
                //qDebug() << "gtd->sampleNumber()=" << gtd->sampleNumber() << "=this->sampleNumber()" << this->sampleNumber();
                Q_ASSERT(gtd->sampleNumber()==this->sampleNumber());
                for (int n=0; n < gtd->sampleNumber(); n++) {
                    Q_ASSERT(this->insertSignalValue(n,digestData[n]+gtdData[n]));
                }
            }
        }
    }
}
