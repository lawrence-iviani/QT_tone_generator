#include "generictimedata.h"

GenericTimeData::GenericTimeData(QObject *parent) :
    QObject(parent),
    m_enableRecalc(false),
    m_data(NULL),
    m_t(NULL),
    m_s(NULL),
    m_sample(0)
{
    m_timeDataDelegate=new DataUiHandlerDelegate(
                dynamic_cast<DataUiHandlerProperty*>(new GenericTimeDataParams(parent)),
                dynamic_cast<DataUiHandlerUI*>(new GenericTimeDataUI()),
                parent);
    init();
}

GenericTimeData::GenericTimeData(TimePlotParams * timePlotParams, QObject *parent) :
    QObject(parent),
    m_enableRecalc(false),
    m_data(NULL),
    m_t(NULL),
    m_s(NULL),
    m_sample(0)
{
    m_timeDataDelegate=new DataUiHandlerDelegate(
                dynamic_cast<DataUiHandlerProperty*>(new GenericTimeDataParams(parent)),
                dynamic_cast<DataUiHandlerUI*>(new GenericTimeDataUI()),
                parent);
    init(timePlotParams);
}

void GenericTimeData::init(TimePlotParams *timePlotParams) {
    GenericTimeDataParams *_gtdp=dynamic_cast<GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);

    //set up the curve
    m_curve=new QwtPlotCurve(_gtdp->name());
    m_curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve->setPaintAttribute(QwtPlotCurve::ClipPolygons);
    //Set property for this curve
    setTimePlotParams(timePlotParams);
    _gtdp->setShowCurve(true);
    _gtdp->setEnableCurve(true);
    //set envelope
    m_envelope=new DataEnvelope(_gtdp->sampleRate(),this);
    //set recalc enabled
    m_enableRecalc=true;
    this->createData();
    //connect any signal
    this->connectSignals();

}

void GenericTimeData::setTimePlotParams(TimePlotParams * timePlotParams) {
    GenericTimeDataParams *_gtdp=dynamic_cast<GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);
    if (timePlotParams) {
        _gtdp->setMaxDuration(timePlotParams->maxDuration());
        _gtdp->setStartTime(timePlotParams->minTime());
        _gtdp->setSampleRate(timePlotParams->sampleRate());
    } else {
        _gtdp->setMaxDuration(TIMEDATA_DEFAULT_PROJECT_TIME);
        _gtdp->setStartTime(TIMEDATA_DEFAULT_MIN_TIME);
        _gtdp->setSampleRate(TIMEDATA_DEFAULT_SR);
    }
    this->createData();
}

GenericTimeData::~GenericTimeData() {
    this->deleteAllData();
    if (m_curve!=NULL) {
        free(m_curve);
        m_curve=NULL;
    }
    free(m_data);
    m_data=NULL;
    if (m_timeDataDelegate) delete m_timeDataDelegate;
}

void GenericTimeData::connectSignals() {
    GenericTimeDataParams *_gtdp=dynamic_cast< GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);
    //connect sample rate & max duration
    Q_ASSERT(connect(_gtdp,SIGNAL(maxDurationChanged(qreal)),this,SLOT(maxDurationHasChanged(qreal))));
    Q_ASSERT(connect(_gtdp,SIGNAL(sampleRateChanged(qreal)),this,SLOT(sampleRateHasChanged(qreal))));

    //emit check/unckeck show and enable, color, name
    Q_ASSERT(connect(_gtdp,SIGNAL(showCurveChanged(bool)),this,SLOT(curveHasChanged())));
    Q_ASSERT(connect(_gtdp,SIGNAL(enableCurveChanged(bool)),this,SLOT(updateData())));
    Q_ASSERT(connect(_gtdp,SIGNAL(colorChanged(QColor)),this,SLOT(curveHasChanged())));
    Q_ASSERT(connect(_gtdp,SIGNAL(nameChanged(QString)),this,SLOT(curveHasChanged())));


    //emit when the name change
    Q_ASSERT(connect(_gtdp,SIGNAL(nameChanged(QString)),this,SIGNAL(nameChanged(QString))));

}

void GenericTimeData::updateData() {
    if (m_enableRecalc) {
        recalc();
        emit dataChanged();
    }
}

void GenericTimeData::createData() {
    if (m_enableRecalc) {
        this->resetAllData();
        recalc();
        this->createDataCurve();
        emit dataChanged();
    }
}

bool GenericTimeData::insertSignalValue(quint64 index, qreal value) {
    bool retval=false;
    const qreal * envelopData;
    quint64 lowestIndex=lowestSampleIndexForModification();
    bool _enableEnv=m_envelope->getEnvelopeParameters()->isEnabledEnvelope();
    if (_enableEnv) {
        Q_ASSERT(m_envelope!=NULL);
        envelopData=m_envelope->getEnvelope();
    }
    if ((lowestIndex<=index) && (index<=highestSampleIndexForModification())  ) {
        m_s[index]=(_enableEnv ? value*envelopData[index-lowestIndex] : value);
        retval=true;
    }
    return retval;
}

void GenericTimeData::resetAllData() {
    this->deleteAllData();
    Q_ASSERT(m_t==NULL);
    Q_ASSERT(m_s==NULL);

    GenericTimeDataParams *_gtdp=dynamic_cast< GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);
    m_sample=qFloor(_gtdp->maxDuration()*_gtdp->sampleRate());//number of sample
    qreal SL=1/_gtdp->sampleRate(); //Sample length

    m_t=new qreal[m_sample];
    m_s=new qreal[m_sample];

    for (quint64 n=0; n < m_sample ; n++) {
        m_t[n]=_gtdp->startTime()+n*SL;
        m_s[n]=0;
    }

    if (m_envelope->getEnvelopeParameters()->isEnabledEnvelope())
        m_envelope->setLength(this->highestSampleIndexForModification()-this->lowestSampleIndexForModification());
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

void GenericTimeData::setTimeData(qreal *t, quint64 len){
    Q_ASSERT(len==m_sample);
    if (m_t!=NULL) {
        delete[] m_t;
        m_t=NULL;
    }
    m_t=t;
}

void GenericTimeData::setSignalData(qreal *s, quint64 len){
    Q_ASSERT(len==m_sample);
    if (m_s!=NULL) {
        delete[] m_s;
        m_s=NULL;
    }
    m_s=s;
}

void GenericTimeData::inihbitUpdate() {
    this->setEnableRecalc(false);
    // this->blockSignals(true);
}

void GenericTimeData::enableUpdate() {
    this->setEnableRecalc(true);
    //this->blockSignals(false);
}

void GenericTimeData::maxDurationHasChanged(qreal maxDuration) {
    Q_UNUSED(maxDuration);
    createData();
    //    resetAllData();
    //    recalc();
    //    createDataCurve();
}

void GenericTimeData::sampleRateHasChanged(qreal SR) {
    Q_UNUSED(SR);
    createData();
    //    resetAllData();
    //    recalc();
    //    createDataCurve();
}

void GenericTimeData::curveHasChanged() {
    GenericTimeDataParams *_gtdp=dynamic_cast< GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);

    //setting color
    QPen  p=m_curve->pen();
    p.setColor(_gtdp->color());
    m_curve->setPen(p);

    //Set name
    m_curve->setTitle(_gtdp->name());

    //Show/hide
    if (_gtdp->isShowEnabled()) {
        m_curve->show();
    } else {
        m_curve->hide();
    }

    emit (curveAttributeChanged());
}


//void GenericTimeData::setEnableCurve(bool enable) {
//    this->m_curveEnabled=enable;
//    this->setShowCurve(enable);
//}

//void GenericTimeData::setShowCurve(bool enable) {
//    if (enable) {
//        m_curve->show();
//    } else {
//        m_curve->hide();
//    }
//    if (m_enableRecalc) emit(curveAttributeUpdated());
//}

//void GenericTimeData::regenerateDomDocument()
//{
//    if (m_enableRecalc) {
//        //Generate the DomDocument of this class
//        //qDebug() << "GenericTimeData::regenerateDomDocument  with tag |"<<GENERICTIMEDATA_TAG<<"|";
//        initDomDocument(PROJECT_CURVETYPE,GENERICTIMEDATA_TAG);// initDomDocument(GENERICTIMEDATA_TAG);

//        //Get DOM document of this object QPROPERTY
//        QDomDocument _doc;
//        this->selfObjectData(&_doc,GENERICTIMEDATAPARAMETERS_TAG);
//        Q_ASSERT(!_doc.isNull());
//        Q_ASSERT(_doc.isDocument());
//        if (!_doc.firstChild().isNull() && appendDomDocument(_doc)) {
//         //   qDebug() << "GenericTimeData::regenerateDomDocument append self properties was fine";
//        } else {
//            qWarning() << "GenericTimeData::regenerateDomDocument append self properties WAS  NOT FINE!!!!!";
//        }

//        //Getting the envelop, it will be appended to
//         const QDomDocument* _d=m_envelope->getEnvelopeParametersDomDocument();
//         Q_ASSERT(!_d->isNull());
//         Q_ASSERT(_d->isDocument());

//         //This is a fix, it should not work this ways but for some reason it's necessary regenerate the Dom
//         if (_d->firstChild().isNull()) {
//            qWarning() << "GenericTimeData::regenerateDomDocument  testing ENVELOPE DomDocument @"<< _d << "first node null, FORCE REGENERATE!!!!! " << _d->nodeName();
//            m_envelope->forceRegenerateDomDocument();
//            _d=m_envelope->getEnvelopeParametersDomDocument();
//      //      qDebug() << "GenericTimeData::regenerateDomDocument  testing ENVELOPE now  DomDocument @"<< _d << " first node is " << _d->nodeName();
//         }
//         Q_ASSERT(!_d->isNull());
//         Q_ASSERT(_d->isDocument());
//         Q_ASSERT(!_d->firstChild().isNull());
//         //qDebug() << "GenericTimeData::regenerateDomDocument  start parsing " << _d->nodeName() ;
//         //qDebug() << "GenericTimeData::regenerateDomDocument  first child node is " << _d->firstChild().nodeName();

//         //Append the envelope to the document
//         if (!_d->firstChild().isNull() && appendDomDocument(_d)) {
//      //       qDebug() << "GenericTimeData::regenerateDomDocument append envelope was fine";
//         } else {
//             qWarning() << "GenericTimeData::regenerateDomDocument append envelope WAS NOT FINE!!!!!";
//         }
//    } else qDebug() << "GenericTimeData::regenerateDomDocument don't need regenarate, flag regenerate set to FALSE";
// }

//bool GenericTimeData::isImportableByDomData(const QDomDocument & doc) {
//    const QDomDocument * pDoc=&doc;
//    return this->isImportableByDomData(pDoc);
//}

//bool GenericTimeData::isImportableByDomData(const QDomDocument * doc) {
//    if (doc->isNull() ) {
//        qWarning() << "GenericTimeData::isImportableByDomData document is null";
//        QMessageBox::warning(0, "GenericTimeData::isImportableByDomData","Document is null");
//        return false;
//    }
//    if (!doc->isDocument()) {
//        qWarning() << "GenericTimeData::isImportableByDomData document is not a document, is " << doc->nodeType();
//        QMessageBox::warning(0, "GenericTimeData::isImportableByDomData",(new QString("Document is not a document is %1"))->arg(doc->nodeType()));
//        return false;
//    }
//    QDomNode node = doc->firstChild();
//    return this->isImportableByDomData(node);
//}

//bool GenericTimeData::isImportableByDomData(QDomNode& node) {

//    //Test if the object is compatible
//    QDomNodeList _nodeListObjType=node.toElement().elementsByTagName(DOMHELPER_OBJECTTYPE_TAG);
//    Q_ASSERT(_nodeListObjType.length()==1);
//    QString _objTypeString=this->getNodeValue(_nodeListObjType.at(0));
//    if (!QString::compare(_objTypeString,this->metaObject()->className())==0) {
//        qWarning() << "GenericTimeData::importXML " << QString("Object not importable! Incompatible object |%1| and |%2|.").arg(_objTypeString).arg(this->metaObject()->className());
//        QMessageBox::warning(0, "GenericTimeData::importXML",QString("Object not importable! Incompatible object |%1| and |%2|.").arg(_objTypeString).arg(this->metaObject()->className()));
//        return false;
//    }

//    //Test if the curve is importable, duration
//    QDomNodeList _nodeListDuration=node.toElement().elementsByTagName("maxduration");
//    Q_ASSERT(_nodeListDuration.length()==1);
//    QString _durationString=this->getNodeValue(_nodeListDuration.at(0));
//    if (!isValidMaxDuration(_durationString.toDouble())) {
//        qWarning() <<  "GenericTimeData::importXML " << QString("Object not importable! Document  contains invalid maxduration  |%1|, project has been set for |%2|.").arg(_durationString).arg(m_MaxDuration);
//        QMessageBox::warning(0, "GenericTimeData::importXML",QString("Object not importable! Document  contains invalid maxduration  |%1|, project has been set for |%2|.").arg(_durationString).arg(m_MaxDuration));
//        return false;
//    }

//    //And Sample Rate
//    QDomNodeList _nodeListSR=node.toElement().elementsByTagName("samplerate");
//    Q_ASSERT(_nodeListSR.length()==1);
//    QString _SRstring=this->getNodeValue(_nodeListSR.at(0));
//    if (!isValidSampleRate(_SRstring.toDouble())) {
//        qWarning() << "GenericTimeData::importXML "<< QString("Object not importable! Document  contains invalid samplerate  |%1|, project has been set for |%2|.").arg(_SRstring).arg(m_SR);
//        QMessageBox::warning(0, "GenericTimeData::importXML",QString("Object not importable! Document  contains invalid samplerate  |%1|, project has been set for |%2|.").arg(_SRstring).arg(m_SR));
//        return false;
//    }
//    return true;
//}

//bool GenericTimeData::importXML() {
//    CTG_app * _app=(CTG_app*) qApp;
//    QString _fileName = QFileDialog::getOpenFileName(NULL, tr("Open CTG curve"),
//                               _app->curveSavePath(),
//                               tr("CTG curve file (*.CCF *.ccf)"));
//    if (_fileName=="") return true;
//    //saving path
//    QFileInfo _fi(_fileName);
//    QString _path=_fi.absolutePath();
//    QString _name=_fi.baseName();
//    if (_path!="")
//        _app->setCurveSavePath(_path);
//    if (_name!="")
//        m_fileName=QString("%1.ccf").arg(_name);

//    return this->importXML(_fileName);
//}

//bool GenericTimeData::importXML(QString fileName) {
//    QDomDocument _d;
//    if (!DomHelper::load(fileName,&_d)) {
//        QMessageBox::warning(0,"GenericTimeData::importXML","Can't load XML file.");
//        return false;
//    }
//    return this->importXML(&_d);
//}

//bool GenericTimeData::importXML(const QDomDocument *doc) {
//    if (doc==NULL) {
//        qWarning() << "GenericTimeData::importXML DATA NOT SET! Document is NULL";
//        QMessageBox::warning(0, "GenericTimeData::importXML","DATA NOT SET! Document is NULL");
//        return false;
//    }
//    if (!doc->isDocument() ) {
//        qWarning() << "GenericTimeData::importXML DATA NOT SET! Document is not a valid DomDocument";
//        QMessageBox::warning(0,"GenericTimeData::importXML","DATA NOT SET! Document is not a valid DomDocument");
//        return false;
//    }
//    QDomNode _firstChild=doc->firstChild();
//    return importXML(&_firstChild);
//}

//bool GenericTimeData::importXML(const QDomNode &node) {
//    return importXML(&node);
//}

//bool GenericTimeData::importXML(const QDomNode *node) {

//    if (node==NULL || node->isNull()) {
//        qWarning() << "GenericTimeData::importXML DATA NOT SET! Trying to import a NULL node";
//        QMessageBox::warning(0,"GenericTimeData::importXML","DATA NOT SET! Trying to import a NULL node");
//        return false;
//    }

//    QString _firstchild=node->nodeName();
//    if (QString::compare(_firstchild,GENERICTIMEDATA_TAG)!=0)  {
//        qWarning() << "GenericTimeData::importXML" << QString("DATA NOT SET! Document  contains as first child  |%1| instead  of |%2|.").arg(_firstchild).arg(GENERICTIMEDATA_TAG);
//        QMessageBox::warning(0, "GenericTimeData::importXML",QString("DATA NOT SET! Document  contains as first child  |%1| instead  of |%2|.").arg(_firstchild).arg(GENERICTIMEDATA_TAG));
//        return false;
//    }

//    qDebug() << "GenericTimeData::importXML importing node " << node->nodeName();

//    //Disable signal propagation, ui update
//    bool _prevValueRecalc=this->setEnableRecalc(false);
//    bool _prevValueSignals=this->blockSignals(true);
//    bool _prevValueUpdateUI=m_genericTimeDataUI->setEnableUpdateUI(false);

//    //Importing data curve
//    QDomNodeList _nodeList=node->toElement().elementsByTagName(GENERICTIMEDATAPARAMETERS_TAG);
//  //  qDebug() << "GenericTimeData::importXML getting element "<< GENERICTIMEDATAPARAMETERS_TAG <<" nodeList.length=" << _nodeList.length();
//    Q_ASSERT(_nodeList.length()==1);
//    QDomNode _node=_nodeList.at(0);

//    //test if it's importable
//    if (!isImportableByDomData(_node))
//        return false;

//    //Setting data curve
//    if (!this->setClassByDomData(_node)) {
//        QMessageBox::warning(0, "GenericTimeData::importXML",QString("DATA NOT SET! SetClassByDomData failed."));
//        return false;
//    }
//    //20130104
//    //In this phase, due to optimization, the data are not updated, this cmd force to set the correct length before going to set envelope.
//    //otherwise the envelope will be  not set correctly in class that inerhits from repeatedTD and partialTD.
//    this->resetAllData();

//    //Importing envelope data
//    _nodeList=node->toElement().elementsByTagName(ENEVELOPEPARAMETERS_TAG);
//   // qDebug() << "GenericTimeData::importXML getting element "<<  ENEVELOPEPARAMETERS_TAG <<" nodeList.length=" << _nodeList.length();
//    Q_ASSERT(_nodeList.length()==1);
//    _node=_nodeList.at(0);
//    if (!m_envelope->setEnvelopeParams(_node)) {
//        QMessageBox::warning(0, "GenericTimeData::importXML",QString("ENVELOPE DATA NOT SET! setEnvelopeParams failed."));
//        return false;
//    }

//    //Enable in the inverse order.
//    m_genericTimeDataUI->setEnableUpdateUI(_prevValueUpdateUI);
//    this->blockSignals(_prevValueSignals);
//    this->setEnableRecalc(_prevValueRecalc);
//    this->createData();

//    //20130114
//    //Rimosso in quanto non sembra servire..
//    //this->regenerateDomDocument();

//    return  true;
//}

//void GenericTimeData::exportXML() {
//    CTG_app * _app=(CTG_app*) qApp;

//    QString _fileName="";
//    if (m_fileName=="")
//        _fileName=QString("%1/%2_SR%3_%4s.ccf")
//                .arg(_app->curveSavePath())
//                .arg(m_name)
//                .arg(m_SR)
//                .arg(m_MaxDuration);
//    else
//        _fileName=m_fileName;

//     _fileName = QFileDialog::getSaveFileName(NULL, tr("Save CTG curve"),
//                               _fileName,
//                               tr("CTG curve file (*.CCF *.ccf)"));
//    this->exportXML(_fileName);
//    //saving path
//    QFileInfo _fi(_fileName);
//    QString _path=_fi.absolutePath();
//    if (_path!="")
//        _app->setCurveSavePath(_path);
//}

//void GenericTimeData::exportXML(QString fileName) {
//    this->regenerateDomDocument();
//    DomHelper::save(fileName,this->getDomDocument());
//}

//void GenericTimeData::copy() {
//    CTG_app * _app=(CTG_app*) qApp;
//    const QDomDocument*  _d=this->getDomDocument();
//    _app->setClipboard(*_d);
//}

//bool GenericTimeData::paste() {
//    if( QMessageBox::question(NULL,"Confirm paste curve",
//                              QString("Do you want to overwrte curve %1 ?").arg(name()),
//                              QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton) == QMessageBox::No ) return true;

//    QString _name=name();
//    QColor _color=color();
//    CTG_app * _app=(CTG_app*) qApp;

//    QDomDocument& _d=_app->clipboard();
//    if (! importXML(&_d) ) {
//        return false;
//    }
//    inihbitUpdate();
//    setName(_name);
//    enableUpdate();
//    setColor(_color);

//    return true;
//}
