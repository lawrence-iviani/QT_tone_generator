#include "generictimedata.h"

GenericTimeData::GenericTimeData(QObject *parent, QWidget *widget) :
    QObject(parent),
    m_data(NULL),
    m_name(QString("no name")),
    m_MaxDuration(TIMEDATA_DEFAULT_MAX_TIME),
    m_Min_t0(TIMEDATA_DEFAULT_MIN_TIME),
    m_SR(TIMEDATA_DEFAULT_SR),
    m_t(NULL),
    m_s(NULL),
    m_sample(0),
    m_curveEnabled(true),
    m_enableEnvelope(false)
{
    init(widget);
}

GenericTimeData::GenericTimeData(qreal maxDuration, qreal SRGen, QObject *parent, QWidget *widget) :
    QObject(parent),
    m_data(NULL),
    m_name(QString("no name")),
    m_MaxDuration(maxDuration),
    m_Min_t0(TIMEDATA_DEFAULT_MIN_TIME),
    m_SR(SRGen),
    m_t(NULL),
    m_s(NULL),
    m_sample(0),
    m_curveEnabled(true),
    m_enableEnvelope(false)
{   
    init(widget);
}

void GenericTimeData::init(QWidget * widget) {
    m_curve=new QwtPlotCurve(m_name);
    m_curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve->setPaintAttribute(QwtPlotCurve::ClipPolygons);
    m_timeDataUI=new TimeDataControlUI(widget);
    m_genericTimeDataUI=new GenericTimeDataUI(this,widget);
    m_envelope=new DataEnvelope(m_SR,this);
    this->createData();
    this->connectSignal();
}


GenericTimeData::~GenericTimeData() {
    this->deleteAllData();
    if (m_curve!=NULL) {
        free(m_curve);
    }
    free(m_data);
}

void GenericTimeData::connectSignal() {
    connect(this,SIGNAL(dataUpdated()),m_timeDataUI,SLOT(updateUI()));
    connect(this,SIGNAL(nameChanged()),m_timeDataUI,SLOT(updateUI()));
    connect(this,SIGNAL(curveAttributeUpdated()),m_timeDataUI,SLOT(updateUI()));
    connect(m_envelope,SIGNAL(enableToggled(bool)),this,SLOT(setEnableEnvelope(bool)));
    connect(m_envelope,SIGNAL(envelopeChanged()),this,SLOT(updateData()));

}

void GenericTimeData::updateData() {
    recalc();
    emit dataUpdated();
}

void GenericTimeData::createData() {
    this->resetAllData();
    recalc();
    this->createDataCurve();
    emit dataUpdated();
}

bool GenericTimeData::insertSignalValue(qint64 index, qreal value) {
    bool retval=false;
    const qreal * envelopData;
    qint64 lowestIndex=lowestSampleIndexForModification();
    if (m_enableEnvelope) {
        Q_ASSERT(m_envelope!=NULL);
        envelopData=m_envelope->getEnvelope();
    }
    if ((lowestIndex<=index) && (index<=highestSampleIndexForModification())  ) {
        m_s[index]=(m_enableEnvelope ? value*envelopData[index-lowestIndex] : value);
        retval=true;
    }
    return retval;
}

void GenericTimeData::resetAllData() {
    this->deleteAllData();
    Q_ASSERT(m_t==NULL);
    Q_ASSERT(m_s==NULL);

    m_sample=floor(m_MaxDuration*m_SR);//number of sample
    qreal SL=1/m_SR; //Sample length

    m_t=new qreal[m_sample];
    m_s=new qreal[m_sample];

    for (int n=0; n < m_sample ; n++) {
        m_t[n]=this->minStartTime()+n*SL;
        m_s[n]=0;
    }

    if (m_enableEnvelope) m_envelope->setLength(this->highestSampleIndexForModification()-this->lowestSampleIndexForModification());
}

void GenericTimeData::createDataCurve() {
    Q_ASSERT(m_curve!=NULL);
    m_data=new QwtCPointerData(m_t,m_s,m_sample);
    m_curve->setData(m_data);//m_data is freed in the setData, see QWT documentation regarding QwtPlotCurve class beahvior
}

void GenericTimeData::deleteAllData() {
    if (m_t!=NULL) {
        delete[] m_t;
        m_t=NULL;
    }
    if (m_s!=NULL) {
        delete[] m_s;
        m_s=NULL;
    }
}

void GenericTimeData::setMaxDurationAndUpdate(qreal  maxDuration, bool updateData) {
    if (maxDuration < 0) {
        m_MaxDuration=0;
    } else {
        m_MaxDuration=maxDuration;
    }
    // m_Max_t1=m_MaxDuration+m_Min_t0;
    if (updateData) this->createData();//Data need to be recalculated, not only updated
}

void GenericTimeData::setMaxDuration(qreal  maxDuration) {
    this->setMaxDurationAndUpdate(maxDuration,true);
}

void GenericTimeData::setSampleRate(qreal SR) {
    this->setSampleRateAndUpdate(SR,true);
}

void GenericTimeData::setSampleRateAndUpdate(qreal SR, bool updateData) {
    m_SR=SR;
    m_envelope->setSampleRate(SR);
    if (updateData) this->createData();//Data need to be recalculated, not only updated
}

void GenericTimeData::setTimeData(qreal *t, qint64 len){
    Q_ASSERT(len==m_sample);
    if (m_t!=NULL) {
        delete[] m_t;
        m_t=NULL;
    }
    m_t=t;
}

void GenericTimeData::setSignalData(qreal *s, qint64 len){
    Q_ASSERT(len==m_sample);
    if (m_s!=NULL) {
        delete[] m_s;
        m_s=NULL;
    }
    m_s=s;
}

void GenericTimeData::setName(QString name) {
    if (m_name!=name) {
        m_name=name;
        m_curve->setTitle(m_name);
        emit(nameChanged());
    }
}

void GenericTimeData::setEnableCurve(bool enable) {
    this->m_curveEnabled=enable;
    this->setShowCurve(enable);
}

void GenericTimeData::setShowCurve(bool enable) {
    if (enable) {
        m_curve->show();
    } else {
        m_curve->hide();
    }
    emit(curveAttributeUpdated());
}

void GenericTimeData::setEnableEnvelope(bool enable) {
    if (m_enableEnvelope!=enable) {
        m_enableEnvelope=enable;
        if (enable) {
//            qDebug() << "GenericTimeData::setEnableEnvelope highind=" <<this->highestSampleIndexForModification()
//                         << " lowind=" << this->lowestSampleIndexForModification()
//                         << " len=" <<this->highestSampleIndexForModification()-this->lowestSampleIndexForModification();
            m_envelope->setLength(this->highestSampleIndexForModification()-this->lowestSampleIndexForModification());
        }
        updateData();
    }
}

void GenericTimeData::setColor(QColor color) {
    if (ComboBoxWidgetColor::isSupportedColorName(color)) {
        QPen  p=m_curve->pen();
        p.setColor(color);
        m_curve->setPen(p);
    }
    emit(curveAttributeUpdated());
}



