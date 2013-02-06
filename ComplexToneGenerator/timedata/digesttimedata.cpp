#include "digesttimedata.h"

DigestTimeData::DigestTimeData(QList<GenericTimeData *> *curveList) :
    GenericTimeData()
{ m_curveList=curveList;init();}

DigestTimeData::DigestTimeData(QList<GenericTimeData*> * curveList, TimePlotParams * timePlotParams ) :
    GenericTimeData(timePlotParams)
{ m_curveList=curveList;init();}

void DigestTimeData::init() {
    GenericTimeDataParams * _params=dynamic_cast<GenericTimeDataParams*>(this->getDataParameters());
    Q_ASSERT(_params!=NULL);
    _params->setName("Digest");
    //Force to disconnect the envelope data. Saving on performance
    disconnect(this->getEnvelopeData(),SIGNAL(envelopeChanged()),this,SLOT(updateData()));
    this->updateData();

    //setting color & Style
    QwtPlotCurve *_curve=this->getCurve();
    QPen _p=_curve->pen();
    QColor _c=_p.color();
 //   QBrush _b=_curve->brush();
 //   _b.setStyle(Qt::SolidPattern);
    _c.setAlpha(PLOTWIDGET_DEFAULT_PLOT_CURVE_TRANSPARENCY);
    _p.setColor(_c);
    _p.setWidthF(PLOTWIDGET_DEFAULT_PLOT_CURVE_WIDTH*2);
    _curve->setPen(_p);
  //      _curve->setBrush(_b);
}

DigestTimeData::~DigestTimeData() {}

void DigestTimeData::setTimeDataList(QList<GenericTimeData *> *curveList) {
    m_curveList=curveList;
    this->updateData();
}

void DigestTimeData::recalc() {
    GenericTimeDataParams * _params=dynamic_cast<GenericTimeDataParams*>(this->getDataParameters());
    Q_ASSERT(_params!=NULL);
    if (!isEnableRecalc()) {
//        qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz")  << " - DigestTimeData::recalc() was  DISABLED!! " << this->name();
        PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,
                          " recalc DISABLED for "+_params->name()));
        return;
    }
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,
                      " recalc enbled for "+_params->name() +
                      " SR="+ _params->sampleRate()));

    Q_ASSERT(m_curveList!=NULL);
    const qreal * digestData=this->getSignalData();
    GenericTimeData * gtd;
    quint64 _sampleNumber=this->getSampleNumber();

    if ( m_curveList->size()>0) {
        for (int m=0; m < m_curveList->size();m++) {
            gtd=m_curveList->at(m);
            Q_ASSERT(gtd!=NULL);
            GenericTimeDataParams * _gtdParams=dynamic_cast<GenericTimeDataParams*>(gtd->getDataParameters());
            Q_ASSERT(_gtdParams!=NULL);
            if (_gtdParams->isCurveEnabled()) {
                const qreal * gtdData=gtd->getSignalData();
                PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"Curve(%1) n_sample=%2@ SR %3 Hz, digest curve n_sample=%4@ SR %5 Hz").
                                  arg(m).arg(gtd->getSampleNumber()).
                                  arg(dynamic_cast<GenericTimeDataParams*>(gtd->getDataParameters())->sampleRate()).
                                  arg(this->getSampleNumber()).
                                  arg(_params->sampleRate()));
                Q_ASSERT(gtd->getSampleNumber()==_sampleNumber);
                for (quint64 n=0; n < gtd->getSampleNumber(); n++) {
                    Q_ASSERT(this->insertSignalValue(n,digestData[n]+gtdData[n]));
                }
            }
        }
    }
}
