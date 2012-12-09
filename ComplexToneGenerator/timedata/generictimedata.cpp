#include "generictimedata.h"

GenericTimeData::GenericTimeData(QWidget *widget) :
    QObject((QObject*) widget),
    DomHelper(this),
    m_enableRecalc(true),
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

GenericTimeData::GenericTimeData(qreal maxDuration, qreal SRGen, QWidget *widget) :
    QObject((QObject*)widget),
    DomHelper(this),
    m_enableRecalc(true),
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
    this->regenerateDomDocument();
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
    connect(m_genericTimeDataUI,SIGNAL(controlUIUpdated()),this,SLOT(regenerateDomDocument()));
}



void GenericTimeData::updateData() {
    if (m_enableRecalc) {
        recalc();
        emit dataUpdated();
    }
}

void GenericTimeData::createData() {
    if (m_enableRecalc) {
        this->resetAllData();
        recalc();
        this->createDataCurve();
        emit dataUpdated();
    }
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

    m_sample=qFloor(m_MaxDuration*m_SR);//number of sample
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

void GenericTimeData::setMaxDuration(qreal  maxDuration) {
    if (maxDuration!=m_MaxDuration) {
        if (maxDuration < 0) {
            m_MaxDuration=0;
        } else {
            m_MaxDuration=maxDuration;
        }
        resetAllData();
        emit(maxDurationChanged(maxDuration));
        recalc();
        createDataCurve();
    }

}

void GenericTimeData::setSampleRate(qreal SR) {
    if ( (SR!=m_SR) && (SR >0) ) {
        m_SR=SR;
        m_envelope->setSampleRate(SR);
        resetAllData();
        emit (sampleRateChanged(SR));
        recalc();
        createDataCurve();
    }
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
    if (m_enableRecalc) emit(curveAttributeUpdated());
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
    if (m_enableRecalc) emit(curveAttributeUpdated());
}

void GenericTimeData::regenerateDomDocument()
{
    if (m_enableRecalc) {
        //Generate the DomDocument of this class
        qDebug() << "GenericTimeData::regenerateDomDocument  with tag |"<<GENERICTIMEDATA_TAG<<"|";
      //  generateDomDocument(GENERICTIMEDATA_TAG);
        initDomDocument(GENERICTIMEDATA_TAG);

        //Get DOM document of this object QPROPERTY
        QDomDocument _doc;
        this->selfObjectData(&_doc,GENERICTIMEDATAPARAMETERS_TAG);
        Q_ASSERT(!_doc.isNull());
        Q_ASSERT(_doc.isDocument());
        if (!_doc.firstChild().isNull() && appendDomDocument(_doc)) {
            qDebug() << "GenericTimeData::regenerateDomDocument append self properties was fine";
        } else {
            qDebug() << "GenericTimeData::regenerateDomDocument append self properties WAS  NOT FINE!!!!!";
        }

        //Getting the envelop, it will be appended to
         const QDomDocument* _d=m_envelope->getEnvelopeParametersDomDocument();
         Q_ASSERT(!_d->isNull());
         Q_ASSERT(_d->isDocument());

         //This is a fix, it should not work this ways but for some reason it's necessary regenerate the Dom
         if (_d->firstChild().isNull()) {
            qDebug() << "GenericTimeData::regenerateDomDocument  testing ENVELOPE DomDocument @"<< _d << "first node null, FORCE REGENERATE!!!!! " << _d->nodeName();
            m_envelope->forceRegenerateDomDocument();
            _d=m_envelope->getEnvelopeParametersDomDocument();
            qDebug() << "GenericTimeData::regenerateDomDocument  testing ENVELOPE now  DomDocument @"<< _d << " first node is " << _d->nodeName();
         }
         Q_ASSERT(!_d->isNull());
         Q_ASSERT(_d->isDocument());
         Q_ASSERT(!_d->firstChild().isNull());
         qDebug() << "GenericTimeData::regenerateDomDocument  start parsing " << _d->nodeName() ;
         qDebug() << "GenericTimeData::regenerateDomDocument  first child node is " << _d->firstChild().nodeName();

         //Append the envelope to the document
         if (!_d->firstChild().isNull() && appendDomDocument(_d)) {
             qDebug() << "GenericTimeData::regenerateDomDocument append envelope was fine";
         } else {
             qDebug() << "GenericTimeData::regenerateDomDocument append envelope WAS NOT FINE!!!!!";
         }
    } else qDebug() << "GenericTimeData::regenerateDomDocument don't need regenarate";
 }

bool GenericTimeData::importXML() {
    QString fileName = QFileDialog::getOpenFileName(NULL, tr("Save File"),
                               ".",
                               tr("XML file (*.xml *.XML)"));
    return this->importXML(fileName);
}

bool GenericTimeData::importXML(QString fileName) {
    QDomDocument _d;
    if (!DomHelper::load(fileName,&_d)) {
        QMessageBox::warning(0,"GenericTimeData::importXML","Can't load XML file.");
        return false;
    }
    return this->importXML(&_d);
}

bool GenericTimeData::importXML(const QDomDocument *doc) {
    if (doc==NULL) {
        QMessageBox::warning(0, "GenericTimeData::importXML","DATA NOT SET! Document is NULL");
        return false;
    }

    if (!doc->isDocument() ) {
        QMessageBox::warning(0,"GenericTimeData::importXML","DATA NOT SET! Document is not a valid DomDocument");
        return false;
    }

    QString _firstchild= doc->firstChild().nodeName();
    if (QString::compare(_firstchild,GENERICTIMEDATA_TAG)!=0)  {
        QMessageBox::warning(0, "GenericTimeData::importXML",QString("DATA NOT SET! Document  contains as first child  |%1| instead  of |%2|.").arg(_firstchild).arg(GENERICTIMEDATA_TAG));
        return false;
    }

    //Disable signal propagation, ui update
    bool _prevValueRecalc=this->setEnableRecalc(false);
    bool _prevValueSignals=this->blockSignals(false);
    bool _prevValueUpdateUI=m_genericTimeDataUI->setEnableUpdateUI(false);

    //Importing data curve
    QDomNodeList _nodeList=doc->elementsByTagName(GENERICTIMEDATAPARAMETERS_TAG);
    qDebug() << "GenericTimeData::importXML getting element "<< GENERICTIMEDATAPARAMETERS_TAG <<" nodeList.length=" << _nodeList.length();
    Q_ASSERT(_nodeList.length()==1);
    QDomNode node=_nodeList.at(0);

    //Setting data curve
    if (!this->setClassByDomData(node)) {
        QMessageBox::warning(0, "GenericTimeData::importXML",QString("DATA NOT SET! SetClassByDomData failed."));
        return false;
    }

    //Importing envelope data
    _nodeList=doc->elementsByTagName(ENEVELOPEPARAMETERS_TAG);
    qDebug() << "GenericTimeData::importXML getting element "<<  ENEVELOPEPARAMETERS_TAG <<" nodeList.length=" << _nodeList.length();
    Q_ASSERT(_nodeList.length()==1);
    node=_nodeList.at(0);
    if (!m_envelope->setEnvelopeParams(node)) {
        QMessageBox::warning(0, "GenericTimeData::importXML",QString("ENVELOPE DATA NOT SET! setEnvelopeParams failed."));
        return false;
    }

    //Enable in the inverse order.
    m_genericTimeDataUI->setEnableUpdateUI(_prevValueUpdateUI);
    this->blockSignals(_prevValueSignals);
    this->setEnableRecalc(_prevValueRecalc);
    this->createData();
    this->regenerateDomDocument();

    return  true;
}

void GenericTimeData::exportXML() {
    QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save File"),
                               ".",
                               tr("XML file (*.xml *.XML)"));
    this->exportXML(fileName);
}

void GenericTimeData::exportXML(QString fileName) {
    DomHelper::save(fileName,this->getDomDocument());
}

