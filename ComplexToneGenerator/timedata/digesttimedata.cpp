#include "digesttimedata.h"

DigestTimeData::DigestTimeData(QList<GenericTimeData *> *curveList) :
    GenericTimeData()
{
    m_curveList=curveList;
    setName(QString("Digest"));
    //Force to disconnect the envelope data. Saving on performance
    disconnect(this->getEnvelopeData(),SIGNAL(envelopeChanged()),this,SLOT(updateData()));
    //Avoid update of UI (there's not UI for this class)
    m_genericTimeDataUI->setEnableUpdateUI(false);
    this->updateData();
}

DigestTimeData::DigestTimeData(QList<GenericTimeData*> * curveList, TimePlotParams * timePlotParams ) :
    GenericTimeData(timePlotParams)
{
    m_curveList=curveList;
    setName(QString("Digest"));
    //Force to disconnect the envelope data. Saving on performance
    disconnect(this->getEnvelopeData(),SIGNAL(envelopeChanged()),this,SLOT(updateData()));
    //Avoid update of UI (there's not UI for this class)
    m_genericTimeDataUI->setEnableUpdateUI(false);
    this->updateData();
}

DigestTimeData::~DigestTimeData() {}

void DigestTimeData::setTimeDataList(QList<GenericTimeData *> *curveList) {
    m_curveList=curveList;
    this->updateData();
}

void DigestTimeData::updateData() {
    //Need to force a reset, because the digest list is updated when all the curves are updated, this means that
    // for example, SR o duration change this will updated twice. In this case force a reset before go on.
    if (m_enableRecalc) DigestTimeData::createData();
}

void DigestTimeData::recalc() {
    if (!m_enableRecalc) {
        qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz")  << " - DigestTimeData::recalc() was  DISABLED!! " << this->name();
        return;
    }
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz")  << " - DigestTimeData::recalc() is enabled---------------- " << this->name();
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz")  << " - DigestTimeData::recalc() SR=" << this->sampleRate()
            << " nsamples="<< this->sampleNumber();


    Q_ASSERT(m_curveList!=NULL);
    const qreal * digestData=this->getSignalData();
    GenericTimeData * gtd;

    if ( m_curveList->size()>0) {
        for (int m=0; m < m_curveList->size();m++) {
            gtd=m_curveList->at(m);
            Q_ASSERT(gtd!=NULL);
            if (m_curveList->at(m)->isEnabled()) {
                const qreal * gtdData=gtd->getSignalData();
                qDebug() << "("<< m <<") gtd->sampleNumber()=" << gtd->sampleNumber() << "@ "<< gtd->sampleRate() <<"Hz  this->sampleNumber()" << this->sampleNumber()<< "@ "<< this->sampleRate() <<"Hz";
                Q_ASSERT(gtd->sampleNumber()==this->sampleNumber());
                for (int n=0; n < gtd->sampleNumber(); n++) {
                    Q_ASSERT(this->insertSignalValue(n,digestData[n]+gtdData[n]));
                }
            }
        }
    }
}
