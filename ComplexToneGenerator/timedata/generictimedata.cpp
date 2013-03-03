#include "generictimedata.h"

GenericTimeData::GenericTimeData(QObject *parent) :
    QObject(parent),
    m_enableRecalc(false),
    m_ui(NULL),
    m_envelope(NULL),
    m_data(NULL),
    m_t(NULL),
    m_s(NULL),
    m_sample(0)
{
    m_timeDataDelegate=new DataUiHandlerDelegate(
                dynamic_cast<DataUiHandlerProperty*>(new GenericTimeDataParams(parent)),
                dynamic_cast<DataUiHandlerUI*>(new GenericTimeDataUI()),
                TIMEDATACURVE_DOCTYPE,
                TIMEDATACURVEPARAMETERS_TAG,
                TIMEDATACURVE_DOCVERSION,
                TIMEDATACURVE_SUFFIX,
                parent);
    init();
}

GenericTimeData::GenericTimeData(TimePlotParams * timePlotParams, QObject *parent) :
    QObject(parent),
    m_enableRecalc(false),
    m_ui(NULL),
    m_envelope(NULL),
    m_data(NULL),
    m_t(NULL),
    m_s(NULL),
    m_sample(0)
{
    m_timeDataDelegate=new DataUiHandlerDelegate(
                dynamic_cast<DataUiHandlerProperty*>(new GenericTimeDataParams(parent)),
                dynamic_cast<DataUiHandlerUI*>(new GenericTimeDataUI()),
                TIMEDATACURVE_DOCTYPE,
                TIMEDATACURVEPARAMETERS_TAG,
                TIMEDATACURVE_DOCVERSION,
                TIMEDATACURVE_SUFFIX,
                parent);
    init(timePlotParams);
}

void GenericTimeData::init(TimePlotParams *timePlotParams) {
    GenericTimeDataParams *_gtdp=dynamic_cast<GenericTimeDataParams*> (getDataParameters());
    GenericTimeDataUI *_gtdpUI=dynamic_cast<GenericTimeDataUI*> (m_timeDataDelegate->getUI());
    Q_ASSERT(_gtdp!=NULL);
    Q_ASSERT(_gtdpUI!=NULL);

    //set up the curve
    m_curve=new QwtPlotCurve(_gtdp->name());
    m_curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve->setPaintAttribute(QwtPlotCurve::ClipPolygons);

    //setting color & Style
    QPen _p=m_curve->pen();
    QColor _c=_gtdp->color();
    _c.setAlpha(PLOTWIDGET_DEFAULT_PLOT_CURVE_TRANSPARENCY);
    _p.setColor(_c);
    _p.setWidthF(PLOTWIDGET_DEFAULT_PLOT_CURVE_WIDTH);
    m_curve->setPen(_p);

    //Set property for this curve
    setTimePlotParams(timePlotParams);
    _gtdp->setShowCurve(true);
    _gtdp->setEnableCurve(true);

    //Prepare envelope
    initEnvelope();

    //Now reset everything so everything has the correct init.
    this->resetAllData();
    this->createDataCurve();
    //connect any signal
    this->connectSignals();

    //set recalc enabled
    m_enableRecalc=true;
    this->updateData();
}

void GenericTimeData::initEnvelope() {
    GenericTimeDataParams *_gtdp=dynamic_cast<GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);
    //Generate envelope
    m_envelope=new DataEnvelope(_gtdp->sampleRate(),
                                this);
    //Connect envelope changed.
    Q_ASSERT(connect(m_envelope,SIGNAL(envelopeChanged()),this,SLOT(updateData())));

    //I Want show the envelope and set to true?
    //getEnvelopeParameters()->setEnableEnvelope(true);
}

void GenericTimeData::initControlWidget() {
    QWidget *_wGTD=dynamic_cast<QWidget*>(m_timeDataDelegate->getUI());
    Q_ASSERT(_wGTD);
    QWidget *_wEnv=dynamic_cast<QWidget*>(m_envelope->getDelegate()->getUI());
    Q_ASSERT(_wEnv);

    QWidget *_prevWidget=NULL;
    if (m_ui) {
        _prevWidget=m_ui;
    }
    m_ui=new QWidget();
    QVBoxLayout * _l=new QVBoxLayout(m_ui);

    _l->addWidget(_wEnv,1);
    _l->addWidget(_wGTD,1);

    if (_prevWidget) delete _prevWidget;
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
    if (m_envelope) delete m_envelope;
}

void GenericTimeData::connectSignals() {
    //Init the control widget for this curve
    initControlWidget();

    GenericTimeDataParams *_gtdp=dynamic_cast< GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);
    GenericTimeDataUI *_gtdpUI=dynamic_cast<GenericTimeDataUI*> (m_timeDataDelegate->getUI());
    Q_ASSERT(_gtdpUI!=NULL);

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

    //C&P button pressed
    Q_ASSERT(connect(_gtdpUI,SIGNAL(buttonCopyPressed()),this,SLOT(copy())));
    Q_ASSERT(connect(_gtdpUI,SIGNAL(buttonPastePressed()),this,SLOT(paste())));

    //import&export button pressed
    Q_ASSERT(connect(_gtdpUI,SIGNAL(buttonExportXMLPressed()),this,SLOT(save())));
    Q_ASSERT(connect(_gtdpUI,SIGNAL(buttonImportXMLPressed()),this,SLOT(load())));

    //Prepare envelope, I need to set the correct number of sample in this stage because the sample number depends on the select curve type
    m_envelope->setSampleNumber(this->highestSampleIndexForModification()-this->lowestSampleIndexForModification());
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
    DataEnvelopeParameters*  _ep=getEnvelopeParameters();
    bool _enableEnv=_ep->isEnabledEnvelope();
    if (_enableEnv) {
        Q_ASSERT(m_envelope!=NULL);
        const qreal * envelopData=m_envelope->getEnvelope();
        Q_ASSERT(envelopData!=NULL);
        quint64 lowestIndex=lowestSampleIndexForModification();
        quint64 highestIndex=highestSampleIndexForModification();
        if ((lowestIndex<=index) && (index<=highestIndex)  ) {
            m_s[index]= value*envelopData[index-lowestIndex];
            retval=true;
        }
    } else {
        m_s[index]= value;
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

    DataEnvelopeParameters*  _ep=getEnvelopeParameters();
    if (_ep->isEnabledEnvelope())
        m_envelope->setSampleNumber(this->highestSampleIndexForModification()-this->lowestSampleIndexForModification());
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
}

void GenericTimeData::enableUpdate() {
    this->setEnableRecalc(true);
    createData();
}

void GenericTimeData::maxDurationHasChanged(qreal maxDuration) {
//    DataEnvelopeUI *_envUI=dynamic_cast<DataEnvelopeUI *> m_envelope->getControlWidget();
//    Q_ASSERT(_envUI);
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
    //setting color & Style
    QPen _p=m_curve->pen();
    QColor _c=_gtdp->color();
    _c.setAlpha(PLOTWIDGET_DEFAULT_PLOT_CURVE_TRANSPARENCY);
    _p.setColor(_c);
    _p.setWidthF(PLOTWIDGET_DEFAULT_PLOT_CURVE_WIDTH);
    m_curve->setPen(_p);

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

void GenericTimeData::copy() {
    CTG_app * _app=(CTG_app*) qApp;
    QDomDocument _d=composeDomDocument();
    _app->setClipboard(_d);
}

void GenericTimeData::paste() {
    GenericTimeDataParams *_gtdp=dynamic_cast<GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);
    CTG_app * _app=(CTG_app*) qApp;

    //preserve name and color
    QString _name=_gtdp->name();
    QColor _color=_gtdp->color();
    if( QMessageBox::question(NULL,"Confirm paste curve",
                              QString("Do you want to overwrte curve %1 ?").arg(_name),
                              QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton) == QMessageBox::No )
        return ;

    QDomDocument _d=_app->clipboard();
    ErrorMessage _err;
    if (!importDomDocument(_d,&_err) ) {
        QMessageBox::warning(NULL,QString("Error import curve"), QString("Error importing parameters\n%1").arg(_err.message()));
    }
    _gtdp->setName(_name);
    _gtdp->setColor(_color);
}

void GenericTimeData::load() {
        CTG_app * _app=(CTG_app*) qApp;
        QString _fileName = QFileDialog::getOpenFileName(NULL, tr("Open curve"),
                                   _app->curveSavePath(),
                                   tr("Curve file (*.%1 *.%2)")
                                         .arg(QString(TIMEDATACURVE_SUFFIX).toUpper())
                                         .arg(QString(TIMEDATACURVE_SUFFIX).toLower()));
        //Retrieving File Info and saving path
        QFileInfo _fi(_fileName);
        QString _path=_fi.absolutePath();
        QString _name=_fi.baseName();
        if (_fileName=="") return;
        if (_path!="")
            _app->setCurveSavePath(_path);
        if (_name!="")
            m_fileName=QString("%1.ccf").arg(_name);

        QDomDocument _d;
        ErrorMessage _err;
        if (!DomHelperUtility::load(_fileName,_d,&_err)) {
            QMessageBox::warning(NULL,QString("Error loading file"), QString("Error loading file%1\n%2")
                                 .arg(m_fileName)
                                 .arg(_err.message()));
        }
        if( QMessageBox::question(NULL,"Confirm import curve",
                                  QString("Do you want to overwrite curve %1 ?").arg(_name),
                                  QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton) == QMessageBox::No )
            return ;

        if (!importDomDocument(_d,&_err) ) {
            QMessageBox::warning(NULL,QString("Error import curve"), QString("Error importing parameters\n%1").arg(_err.message()));
        }

}

bool GenericTimeData::importXML(const QDomDocument& doc, ErrorMessage* err) {
    return importDomDocument(doc,err);
}

QDomDocument GenericTimeData::exportXML() {
    return composeDomDocument();
}

void GenericTimeData::save() {
    GenericTimeDataParams *_gtdp=dynamic_cast< GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);
    CTG_app * _app=(CTG_app*) qApp;

    QString _fileName="";
    if (m_fileName=="")
        _fileName=QString("%1/%2_SR%3_%4s.%5")
                .arg(_app->curveSavePath())
                .arg(_gtdp->name())
                .arg(_gtdp->sampleRate())
                .arg(_gtdp->maxDuration())
                .arg(TIMEDATACURVE_SUFFIX);
    else
        _fileName=m_fileName;

     _fileName = QFileDialog::getSaveFileName(NULL, tr("Save curve"),
                                              _fileName,
                                              tr("Curve file (*.%1 *.%2)")
                                                    .arg(QString(TIMEDATACURVE_SUFFIX).toUpper())
                                                    .arg(QString(TIMEDATACURVE_SUFFIX).toLower()));
     ErrorMessage _err;
     if (!DomHelperUtility::save(_fileName,composeDomDocument(),&_err)) {
         QMessageBox::warning(NULL,QString("Error saving file"), QString("Error saving file %1\n%2")
                              .arg(m_fileName)
                              .arg(_err.message()));
     }

    //saving path
    QFileInfo _fi(_fileName);
    QString _path=_fi.absolutePath();
    if (_path!="")
        _app->setCurveSavePath(_path);
}

bool GenericTimeData::importDomDocument(const QDomDocument& doc, ErrorMessage* err) {
    ErrorMessage _localErr;
    if (!GenericTimeData::testDomDocument(doc,&_localErr)) {
        if (err) {
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,_localErr.message()));
        return false;
    }
    return importDomNode(doc.firstChild(),err);
}

bool GenericTimeData::importXML(const QDomNode& node, ErrorMessage* err) {
    return importDomNode(node,err);
}

bool GenericTimeData::importDomNode(const QDomNode& rootNode, ErrorMessage* err) {
    GenericTimeDataParams *_gtdp=dynamic_cast< GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);
    ErrorMessage _localErr;
//////------------- 1 retrieving data node for TIME DATA and ENVELOPE rootNode
//////------------- 2 verifing importing before importing
//////------------- 3 Importing
    //1a------------- RETRIEVING TIMEDATAPARAMS NODE
    QDomNode _timeDataParamsNode=foundDomTimeDataParams(rootNode,&_localErr);
    if (_timeDataParamsNode.isNull()) {
        if (err) {
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,_localErr.message()));
        return false;
    }
    //1b------------- RETRIEVING ENVELOPE NODE
    QDomNode _envelopeNode=foundDomEnvelopeParams(rootNode,&_localErr);
    if (_envelopeNode.isNull()) {
        if (err) {
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,_localErr.message()));
        return false;
    }

    ///2------------- VERIFYING
    //Ask to delegate if time data is importable
    if (!m_timeDataDelegate->isImportableByDomData(_timeDataParamsNode,&_localErr) ) {
        if (err) {
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,_localErr.message()));
        return false;
    }

    //Verify if it's compatible with this class
    if (!areValidTimeDataSettings(rootNode,&_localErr) ) {
        if (err) {
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,_localErr.message()));
        return false;
    }
    //Verify if the envelope is importable
    if (!m_envelope->getDelegate()->isImportableByDomData(_envelopeNode,&_localErr) ) {
        if (err) {
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,_localErr.message()));
        return false;
    }

    //3------------- SETTING PARAMS
    this->inihbitUpdate();
    if (!getDelegate()->setClassByDomData(_timeDataParamsNode,true,&_localErr) ) {
        if (err) {
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,_localErr.message()));
        this->enableUpdate();
        return false;
    }

    //Before setting envelope sets SR an sample (they are not recalculated due to the inihbitUpdate)
    m_envelope->setSampleRateAndSampleNumber(_gtdp->sampleRate(),
                                             this->highestSampleIndexForModification()-this->lowestSampleIndexForModification());
    if (!m_envelope->getDelegate()->setClassByDomData(_envelopeNode,true,&_localErr) ) {
        if (err) {
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,_localErr.message());
        this->enableUpdate();
        return false;
    }
    this->enableUpdate();
    return true;
}

QDomDocument GenericTimeData::composeDomDocument() {
    QDomNode _n1=getDelegate()->getDomDocument().firstChild();
    Q_ASSERT(!_n1.isNull());
    QDomNode _n2=m_envelope->getDelegate()->getDomDocument().firstChild();
    Q_ASSERT(!_n2.isNull());
    QList<QDomNode*> _nodeList;
    _nodeList << &_n1;
    _nodeList << &_n2;
    QDomDocument _d=DomHelperUtility::createDocFromNodesList(_nodeList,
                                                             TIMEDATACURVE_DOCTYPE,
                                                             TIMEDATACURVE_TAG,
                                                             TIMEDATACURVE_DOCVERSION);
    QDomNode _n=_d.elementsByTagName(TIMEDATACURVE_TAG).at(0);
    _n.toElement().setAttribute(DOMHELPER_OBJECTTYPE_ATTRIBUTE,this->metaObject()->className());
    //qDebug() << Q_FUNC_INFO<<  "compose doc=" << _d.toString();
    return _d;
}

bool GenericTimeData::areValidTimeDataSettings(const QDomNode& node, ErrorMessage* errMessage) {
    GenericTimeDataParams *_gtdp=dynamic_cast< GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);

    QDomNode _attObjectType=DomHelperUtility::getAttribute(node.attributes(),DOMHELPER_OBJECTTYPE_ATTRIBUTE);
    if (_attObjectType.isNull()) {
        QString msg=QString("Object not importable! no TAG |%1| found.").
                arg(DOMHELPER_OBJECTTYPE_ATTRIBUTE);
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(msg);
        } else PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,msg,ErrorMessage::WARNINGMESSAGE));
        return false;
    }
    QString _objType=this->metaObject()->className();
    if (_attObjectType.nodeValue()!=_objType) {
        QString msg=QString("Object not importable! Found object %1 , expected object %2")
                .arg(_attObjectType.nodeValue())
                .arg(_objType);
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(msg);
        } else PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,msg,ErrorMessage::WARNINGMESSAGE));
        return false;
    }

//TODO: CONTROL VERSION MISSING!!

    //Test if the curve is importable, duration
    QDomNodeList _nodeListDuration=node.toElement().elementsByTagName("maxDuration");
    Q_ASSERT(_nodeListDuration.length()==1);
    QString _durationString=DomHelperUtility::getNodeValue(_nodeListDuration.at(0));
    if (_gtdp->maxDuration()<_durationString.toDouble()) {
        QString msg=QString("Document  contains invalid maxDuration  |%1|, project has been set for |%2|.").
                arg(_durationString).
                arg(_gtdp->maxDuration());
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(msg);
        } else PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,msg,ErrorMessage::WARNINGMESSAGE));
        return false;
    }

    //And Sample Rate
    QDomNodeList _nodeListSR=node.toElement().elementsByTagName("sampleRate");
    Q_ASSERT(_nodeListSR.length()==1);
    QString _SRstring=DomHelperUtility::getNodeValue(_nodeListSR.at(0));
    if (_gtdp->sampleRate()!=_SRstring.toDouble()) {
        QString msg=QString("Document  contains invalid sampleRate  |%1|, project has been set for |%2|.").
                arg(_SRstring).
                arg(_gtdp->sampleRate());
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(msg);
        } else PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,msg,ErrorMessage::WARNINGMESSAGE));
        return false;
    }
    return true;
}

//Static method
bool GenericTimeData::testDomDocument(const QDomDocument& doc, ErrorMessage* err) {
    QDomNode _rootNode=doc.firstChild();
    ErrorMessage _localErr;

    if (doc.isNull() || !doc.isDocument() || _rootNode.isNull()) {
        QString msg("Null data document");
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,msg));
        return false;
    }

    //Verify doctype
    if (doc.doctype().name()!=TIMEDATACURVE_DOCTYPE) {
        QString msg=QString("Invalid doc type, expected -%1- found -%2-").arg(doc.doctype().toText().data()).arg(TIMEDATACURVE_DOCTYPE);
        if (err){
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        }  else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,_localErr.message()));
        return false;
    }
    return true;
}

QDomNode GenericTimeData::foundDomTimeDataParams(const QDomNode rootNode, ErrorMessage* err) {
    QDomNodeList _nodeListTimeDataParams;
    ErrorMessage _localErr;
    QDomNode _emptyRetval;
    if(!DomHelperUtility::nodeListByTagName(_nodeListTimeDataParams,
                                        rootNode,
                                        TIMEDATACURVEPARAMETERS_TAG,
                                        TIMEDATACURVE_PARAMSVERSION,
                                        &_localErr)) {
        if (err) {
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,_localErr.message()));
        return _emptyRetval;
    }
    if (_nodeListTimeDataParams.length()==0) {
        QString msg="Error parsing time data, no valid parameters node";
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,msg));
        return _emptyRetval;
    }
    if (_nodeListTimeDataParams.length()>1) {
        QString msg="Error parsing time data, too many parameters nodes";
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,msg));
        return _emptyRetval;
    }
    return _nodeListTimeDataParams.at(0);
}

QDomNode GenericTimeData::foundDomEnvelopeParams(const QDomNode rootNode, ErrorMessage *err) {
    QDomNodeList _nodeListEnvelope;
    ErrorMessage _localErr;
    QDomNode _emptyRetval;

    if (!DomHelperUtility::nodeListByTagName(_nodeListEnvelope,
                                        rootNode,
                                        ENVELOPE_TAG,
                                        ENVELOPE_PARAMSVERSION,
                                        &_localErr) ) {
        if (err) {
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,_localErr.message()));
        return _emptyRetval;
    }
    if (_nodeListEnvelope.length()==0) {
        QString msg="Error parsing envelope data, no valid envelope node";
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,msg));
        return _emptyRetval;
    }
    if (_nodeListEnvelope.length()>1) {
        QString msg="Error parsing envelope data, too many envelope nodes";
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,msg));
        return _emptyRetval;
    }
    return _nodeListEnvelope.at(0);
}

QString GenericTimeData::getObjectType(const QDomNode &node) {
    QString _retval="";

    QDomNode _attObjectType=DomHelperUtility::getAttribute(node.attributes(),DOMHELPER_OBJECTTYPE_ATTRIBUTE);
    if (_attObjectType.isNull()) {
        QString msg=QString("Object not importable! no TAG |%1| found.").
                arg(DOMHELPER_OBJECTTYPE_ATTRIBUTE);
        PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,msg,ErrorMessage::WARNINGMESSAGE));
        return _retval;
    }
    return _attObjectType.nodeValue();
}

