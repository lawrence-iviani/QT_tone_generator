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
        QMessageBox::warning(NULL,QString("Error import curve"), QString("Error importing paarameters").arg(_err.message()));
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



//        GenericTimeDataParams *_gtdp=dynamic_cast<GenericTimeDataParams*> (getDataParameters());
//        Q_ASSERT(_gtdp!=NULL);
//        CTG_app * _app=(CTG_app*) qApp;

//        //preserve name and color

        if( QMessageBox::question(NULL,"Confirm import curve",
                                  QString("Do you want to overwrite curve %1 ?").arg(_name),
                                  QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton) == QMessageBox::No )
            return ;

        if (!importDomDocument(_d,&_err) ) {
            QMessageBox::warning(NULL,QString("Error import curve"), QString("Error importing paarameters").arg(_err.message()));
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
    QDomNode _rootNode=doc.firstChild();
    ErrorMessage _localErr;
    if (doc.isNull() || _rootNode.isNull() ) {
        QString msg("Null data document");
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,msg);
        return false;
    }
    //Verify doctype
    if (doc.doctype().name()!=TIMEDATACURVE_DOCTYPE) {
        QString msg=QString("Invalid doc type, expected -%1- found -%2-").arg(doc.doctype().toText().data()).arg(TIMEDATACURVE_DOCTYPE);
        if (err){
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        }  else ErrorMessage::WARNING(Q_FUNC_INFO,_localErr);
        return false;
    }

 //   qDebug() << Q_FUNC_INFO<<  "importing doc=" << doc.toString();
    return importDomNode(_rootNode,err);
}

bool GenericTimeData::importDomNode(const QDomNode& rootNode, ErrorMessage* err) {
    GenericTimeDataParams *_gtdp=dynamic_cast< GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);
//////------------- 1 retrieving data node for TIME DATA and ENVELOPE rootNode
//////------------- 2 verifing importing before importing
//////------------- 3 Importing
    //1a------------- RETRIEVING TIMEDATAPARAMS NODE
    QDomNodeList _nodeListTimeDataParams;
    ErrorMessage _localErr;
    if(!DomHelperUtility::nodeListByTagName(_nodeListTimeDataParams,
                                        rootNode,
                                        TIMEDATACURVEPARAMETERS_TAG,
                                        TIMEDATACURVE_PARAMSVERSION,
                                        &_localErr)) {
        if (err) {
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,_localErr);
        return false;
    }
    if (_nodeListTimeDataParams.length()==0) {
        QString msg="Error parsing time data, no valid parameters node";
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,msg);
        return false;
    }
    if (_nodeListTimeDataParams.length()>1) {
        QString msg="Error parsing time data, too many parameters nodes";
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,msg);
        return false;
    }
    QDomNode _timeDataParamsNode=_nodeListTimeDataParams.at(0);

    //1b------------- RETRIEVING ENVELOPE NODE
    QDomNodeList _nodeListEnvelope;
    if (!DomHelperUtility::nodeListByTagName(_nodeListEnvelope,
                                        rootNode,
                                        ENVELOPE_TAG,
                                        ENVELOPE_PARAMSVERSION,
                                        &_localErr) ) {
        if (err) {
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,_localErr);
        return false;
    }
    if (_nodeListEnvelope.length()==0) {
        QString msg="Error parsing envelope data, no valid envelope node";
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,msg);
        return false;
    }
    if (_nodeListEnvelope.length()>1) {
        QString msg="Error parsing envelope data, too many envelope nodes";
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,msg);
        return false;
    }
    QDomNode _envelopeNode=_nodeListEnvelope.at(0);

    ///2------------- VERIFYING
    //Ask to delegate if time data is importable
    if (!m_timeDataDelegate->isImportableByDomData(_timeDataParamsNode,&_localErr) ) {
        if (err) {
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,_localErr);
        return false;
    }

    if (!areValidTimeDataSettings(rootNode,&_localErr) ) {
        if (err) {
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,_localErr);
        return false;
    }
    //same for envelope
    if (!m_envelope->getDelegate()->isImportableByDomData(_envelopeNode,&_localErr) ) {
        if (err) {
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,_localErr);
        return false;
    }

    //3------------- SETTING PARAMS
    this->inihbitUpdate();
    if (!getDelegate()->setClassByDomData(_timeDataParamsNode,true,&_localErr) ) {
        if (err) {
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,_localErr);
        this->enableUpdate();
        return false;
    }

    //Before setting envelope sets SR an sample (they are not recalculated due to the inihbitUpdate)
    m_envelope->setSampleRateAndSampleNumber(_gtdp->sampleRate(),
                                             this->highestSampleIndexForModification()-this->lowestSampleIndexForModification());
    if (!m_envelope->getDelegate()->setClassByDomData(_envelopeNode,true,&_localErr) ) {
        if (err) {
            err->appendMessage(_localErr);
        } else ErrorMessage::WARNING(Q_FUNC_INFO,_localErr);
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
   // qDebug() << Q_FUNC_INFO<<  "compose doc=" << _d.toString();
    return _d;
}

bool GenericTimeData::areValidTimeDataSettings(const QDomNode& node, ErrorMessage* errMessage) {
    GenericTimeDataParams *_gtdp=dynamic_cast< GenericTimeDataParams*> (getDataParameters());
    Q_ASSERT(_gtdp!=NULL);

    QDomNodeList _nodeListObjType=node.toElement().elementsByTagName(DOMHELPER_OBJECTTYPE_TAG);
    if (_nodeListObjType.length()==0) {
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(QString("Data object not importable! no TAG |%1| found.").
                                   arg(DOMHELPER_OBJECTTYPE_TAG));
        }
        return false;
    }

    //Lookin for the correct node, many can be in there
    QDomNode _nodeObject;
    bool _foundNode=false;
    QString _objParams=getDelegate()->getProperty()->metaObject()->className();
    for (uint i=0 ; i < _nodeListObjType.length() ; i++ ) {
        QDomNode _n=_nodeListObjType.at(i);
        QString _objTypeString=DomHelperUtility::getNodeValue(_n);
        if (QString::compare(_objTypeString,_objParams)==0) {
            if (_foundNode) {
                if (errMessage) {
                    errMessage->setMethod(Q_FUNC_INFO);
                    errMessage->setMessage(QString("Too many data object with TAG |%1| and value |%2|.").
                                           arg(DOMHELPER_OBJECTTYPE_TAG).
                                           arg(DomHelperUtility::getNodeValue(_nodeObject)).
                                           arg(_objParams));
                }
                return false;
            }
            _foundNode=true;
            _nodeObject=_n;
        }
    }

    //Test if i found something
    if (!_foundNode || _nodeObject.isNull()) {
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(QString("Object not importable! Can't found a valid object descriptor."));
        }
        return false;
    }

    //Test if the curve is importable, duration
    QDomNodeList _nodeListDuration=node.toElement().elementsByTagName("maxDuration");
    Q_ASSERT(_nodeListDuration.length()==1);
    QString _durationString=DomHelperUtility::getNodeValue(_nodeListDuration.at(0));
    if (_gtdp->maxDuration()<_durationString.toDouble()) {
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(QString("Document  contains invalid maxDuration  |%1|, project has been set for |%2|.").
                                           arg(_durationString).
                                           arg(_gtdp->maxDuration()));
        }
        return false;
    }

    //And Sample Rate
    QDomNodeList _nodeListSR=node.toElement().elementsByTagName("sampleRate");
    Q_ASSERT(_nodeListSR.length()==1);
    QString _SRstring=DomHelperUtility::getNodeValue(_nodeListSR.at(0));
    if (_gtdp->sampleRate()!=_SRstring.toDouble()) {
        if (errMessage) {
            errMessage->setMethod(Q_FUNC_INFO);
            errMessage->setMessage(QString("Document  contains invalid sampleRate  |%1|, project has been set for |%2|.").
                                   arg(_SRstring).
                                   arg(_gtdp->sampleRate()));
        }
        return false;
    }
    return true;
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


