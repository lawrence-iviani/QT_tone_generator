#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_audioPlayer(new AudioPlayer),
    m_plotTime(new TimePlotWidget),
    m_plotFreq(new FreqPlotWidget),
    m_indexGenerator(1),
    m_toolBoxFixedItem(0),
    ui(new Ui::MainWindow),
    m_TreeWidgetshowXML(0)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(ui->globalGridLayout);
    setupUI();
    initAudio();
    connectSignals();
}

void MainWindow::initAudio() {
    m_digestCurveStream=new InternalStreamDevice(AudioUtils::getStandardFormat(AudioUtils::DAT));
    m_digestCurveStream->setAudioData((qreal*) m_plotTime->getDigestCurve()->getSignalData(),m_plotTime->getDigestCurve()->sampleNumber());
    m_audioPlayer->setStream(m_digestCurveStream);
}

void MainWindow::connectSignals() {
    //connect command buttons
    connect(s_button.addCurve,SIGNAL(clicked()),SLOT(newCurve()));
    connect(s_button.removeAllCurves,SIGNAL(clicked()),SLOT(removeAllCurvesWithDialog()));
    connect(s_button.removeCurve,SIGNAL(clicked()),SLOT(removeCurve()));
    connect(s_button.exportDigest,SIGNAL(clicked()),SLOT(exportDigestCurve()));
    connect(s_button.exportXML ,SIGNAL(clicked()),SLOT(exportXML()));
    connect(s_button.importXML,SIGNAL(clicked()),SLOT(importXML()));
    connect(s_button.showXML ,SIGNAL(clicked()),SLOT(showXML()));

    //connect digest curve to handle update in the plots
    connect(m_plotTime->getDigestCurve(),SIGNAL(dataUpdated()),this,SLOT(digestCurveChanged()));

    //Connect position slider
    connect(m_audioPlayer,SIGNAL(streamTimePositionChanged(qreal)) ,this,SLOT(streamPositionUpdate(qreal)));
}

void MainWindow::setupUI() {
    setupSplitters();
    s_widgetUI.buttonsFrame=createButtonsFrame();
    s_widgetUI.toolboxOptionFrame=setupOptionsFrame();
    setupPlots();

    //Adding split layout
    s_widgetUI.plotSplitter->addWidget(m_plotTime);
    s_widgetUI.plotSplitter->addWidget(m_plotFreq);
    s_widgetUI.commandSplitter->addWidget(s_widgetUI.toolboxOptionFrame);
    s_widgetUI.commandSplitter->addWidget(s_widgetUI.buttonsFrame);

    //Layout all the windows
    ui->centralwidget->layout()->addWidget(s_widgetUI.globalSplitter);

}

void MainWindow::setupSplitters() {

    //Setting  plot area
    s_widgetUI.plotSplitter= new QSplitter(Qt::Vertical);
    s_widgetUI.plotSplitter->setFrameStyle(QFrame::QFrame::Raised);
    s_widgetUI.plotSplitter->setFrameShape(QFrame::QFrame::Panel);
    s_widgetUI.plotSplitter->setLineWidth(1);
    s_widgetUI.plotSplitter->setMidLineWidth(1);
    s_widgetUI.plotSplitter->setStretchFactor(0, 3);
    s_widgetUI.plotSplitter->setStretchFactor(1, 1);

    //Setting  command area
    s_widgetUI.commandSplitter= new QSplitter(Qt::Vertical);
    s_widgetUI.commandSplitter->setFrameStyle(QFrame::QFrame::Raised);
    s_widgetUI.commandSplitter->setFrameShape(QFrame::QFrame::Panel);
    s_widgetUI.commandSplitter->setLineWidth(1);
    s_widgetUI.commandSplitter->setMidLineWidth(1);
    s_widgetUI.commandSplitter->setStretchFactor(0, 1);
    s_widgetUI.commandSplitter->setStretchFactor(1, 3);

    //Setting  total area
    s_widgetUI.globalSplitter= new QSplitter(Qt::Horizontal);
    s_widgetUI.globalSplitter->addWidget(s_widgetUI.plotSplitter);
    s_widgetUI.globalSplitter->addWidget(s_widgetUI.commandSplitter);
    s_widgetUI.globalSplitter->setStretchFactor(0, 3);
    s_widgetUI.globalSplitter->setStretchFactor(1, 1);
}

QFrame *MainWindow::createButtonsFrame()  {
    QVBoxLayout *_l=new QVBoxLayout();

    //Init buttons
    s_button.addCurve=new QPushButton("Add curve");
    s_button.removeCurve=new QPushButton("Remove curve");
    s_button.removeAllCurves=new QPushButton("Remove all curves");
    s_button.exportDigest=new QPushButton("Export digest");
    s_button.exportXML=new QPushButton("Export XML");
    s_button.importXML=new QPushButton("Import XML");
    s_button.showXML=new QPushButton("Show XML");

    //Adding XML button
    QHBoxLayout * lh=new QHBoxLayout();
    lh->addWidget(s_button.exportXML,1,Qt::AlignLeft);
    lh->addWidget(s_button.importXML,1,Qt::AlignLeft);
    lh->addWidget(s_button.showXML,1,Qt::AlignLeft);
    QWidget *_buttonWidget=new QWidget(this);
    _buttonWidget->setLayout((QLayout*)lh);

    //Layout the button panel
    _l->addWidget(s_button.addCurve);
    _l->addWidget(s_button.removeAllCurves);
    _l->addWidget(s_button.removeCurve);
    _l->addWidget(s_button.exportDigest);
    _l->addWidget(_buttonWidget);

    QFrame *_w=new QFrame();
    _w->setFrameStyle(QFrame::QFrame::Raised);
    _w->setFrameShape(QFrame::QFrame::Panel);
    _w->setLineWidth(1);
    _w->setMidLineWidth(1);

    _w->setLayout((QLayout*)_l);
    return _w;
}

QFrame *MainWindow::setupOptionsFrame() {
    s_widgetUI.toolboxOption=new QToolBox();

    QVBoxLayout *_l=new QVBoxLayout();
    //Adding widget to the toolbox option
    s_widgetUI.toolboxOption->insertItem(m_toolBoxFixedItem++,m_plotTime->getControlWidget(),"Time Option");
    s_widgetUI.toolboxOption->insertItem(m_toolBoxFixedItem++,m_plotFreq->getControlWidget(),"Freq Option");

    //Setting audio player control and digest curve stream
    s_widgetUI.toolboxOption->insertItem(m_toolBoxFixedItem++, m_audioPlayer->getAudioControlWidget(),"Audio Player");

    QFrame *_w=new QFrame();
    _w->setFrameStyle(QFrame::QFrame::Raised);
    _w->setFrameShape(QFrame::QFrame::Panel);
    _w->setLineWidth(1);
    _w->setMidLineWidth(1);

    _l->addWidget(s_widgetUI.toolboxOption);
    _w->setLayout((QLayout*)_l);
    return _w;
}

void MainWindow::setupPlots() {
    m_plotTime->setBothAxisScale(TIMEDATA_DEFAULT_MIN_TIME,TIMEDATA_DEFAULT_MAX_TIME,-1.0,1.0);
    m_plotFreq->setBothAxisScale(PlotWidget::Logarithmic,20.0,20000.0,PlotWidget::Linear, -40.0,0.0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//SLOTS----------
void MainWindow::newCurve() {

    SelectCurveWindowHelper * selectCurveHelper=SelectCurveWindowDialog::getDialogCurveHelper();
    this->setupCurves(selectCurveHelper);
    SelectCurveWindowDialog * selectDialog=new SelectCurveWindowDialog(selectCurveHelper,this);
    selectDialog->exec();
    GenericTimeData * s=this->decodeSelectedCurve(selectCurveHelper);
    if (s==NULL) return;

    QString name= "Curve_";
    name.append(QString::number(m_indexGenerator++));
    s->setColor(Qt::white);
    s->setName(name);
    connect(s,SIGNAL(nameChanged()),this,SLOT(updateCurvesName()));

    //adding data to the plot time
    m_plotTime->addTimeData(s);

    //adding controls to plot
    QWidget *_widget=(QWidget*)s->getControlWidget();
    s_widgetUI.toolboxOption->addItem(_widget,s->name());

    delete selectCurveHelper;
    delete selectDialog;
}

void MainWindow::setupCurves(SelectCurveWindowHelper * selectCurveHelper) {
    //TODO, this should be decoded with a file xml or similar

    S_DataCurve t;

    t.name="Limited duration Tone generator";
    t.description=" This curve generate a pure tone with a limited in Duration";
    selectCurveHelper->addData(t);

    t.name="Tone generator";
    t.description=" This curve generate a pure tone for the whole duration";
    selectCurveHelper->addData(t);

    t.name="Repeated duration Tone generator";
    t.description=" This curve generate a pure tone with a limited in Duration";
    selectCurveHelper->addData(t);

    t.name="Limited Const Data";
    t.description=" This curve generate a constant limited in time";
    selectCurveHelper->addData(t);

    t.name="Const Data";
    t.description=" This curve generate a constant for the whole duration";
    selectCurveHelper->addData(t);
}

GenericTimeData*  MainWindow::decodeSelectedCurve(SelectCurveWindowHelper * selectCurveHelper) {
    GenericTimeData * retval=NULL;

    QString curveName=selectCurveHelper->getSelectedDataCurve().name;
    if (curveName==NULL) return NULL;
    qDebug() << "MainWindow::decodeSelectedCurve curve selected is " << curveName;


    if (QString::compare(curveName,"Limited duration Tone generator")==0 ) {
        PartialSinusData * s=new PartialSinusData(m_plotTime->getTimePlotParams() , m_plotTime);//s_widgetUI.toolboxOption);
        s->setStartTime(1.414);
       // s->setDuration(5.1);
        s->setAmplitudeFrequencyAndPhase(0.5,125,90);
        retval=(GenericTimeData*) s;
        return retval;
    }

    if (QString::compare(curveName,"Repeated duration Tone generator")==0 ) {
        RepeatedSinusData * s=new RepeatedSinusData(m_plotTime->getTimePlotParams() , m_plotTime);
        s->setStartTime(0.2);
        s->setDuration(0.5);
        s->setBlankTime(0.25);
        s->setAmplitudeFrequencyAndPhase(0.250,500,45);
        retval=(GenericTimeData*) s;
        return retval;
    }

    if (QString::compare(curveName,"Tone generator")==0 ) {
        GenericSinusData * s=new GenericSinusData(m_plotTime->getTimePlotParams() , m_plotTime);
        s->setAmplitudeFrequencyAndPhase(0.125,250,0);
        retval=(GenericTimeData*) s;
        return retval;
    }
    if (QString::compare(curveName,"Limited Const Data")==0 ) {
        PartialConstantTimeData * s=new PartialConstantTimeData(m_plotTime->getTimePlotParams());
        s->setAmplitude(0.3);
        s->setDuration(5.1);
        retval=(GenericTimeData*) s;
        return retval;
    }
    if (QString::compare(curveName,"Const Data")==0 ) {
        ConstantTimeData * s=new ConstantTimeData(m_plotTime->getTimePlotParams());
        s->setAmplitude(-0.15);
        retval=(GenericTimeData*) s;
        return retval;
    }

    return retval;
}

void MainWindow::timeDataUpdated() {
    m_plotTime->replot();
}

void MainWindow::removeCurve(){
     QStringList  sl=m_plotTime->getTimeDataStringList();
     SelectRemoveCurveWindowDialog * removeDialog=new SelectRemoveCurveWindowDialog(&sl,this);//(sl,this);
     removeDialog->exec();
     foreach (int i, removeDialog->getRemoveCurvesIndex()) {
            s_widgetUI.toolboxOption->removeItem(i+m_toolBoxFixedItem);
            if (!m_plotTime->removeTimeData(i)) {
                qWarning() << "MainWindow::removeCurve: can't remove GenericTimeData@index=" <<i;
            } else {
                qDebug() << "MainWindow::removeCurve: removed GenericTimeData@index=" <<i;
            }
     }
     delete removeDialog;
}

void MainWindow::removeAllCurvesWithDialog() {
    if( QMessageBox::question(this,"Confirm remove curves","Do you want to remove all the curves?",QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton) == QMessageBox::No ) return;
    removeAllCurves();
}

void MainWindow::removeAllCurves() {
    //const QList<GenericTimeData*> & getTimeDataList()
    //const QList<GenericTimeData*> _gtdList= m_plotTime->getTimeDataList();
    while (m_plotTime->removeTimeData(0))  {
        s_widgetUI.toolboxOption->removeItem(m_toolBoxFixedItem);
  //      if (!m_plotTime->removeTimeData(0))
  //          qWarning() << "MainWindow::removeAllCurves can't remove GenericTimeData " << " @ index=0,  null pointer?";
    }
}

void MainWindow::digestCurveChanged() {
    m_digestCurveStream->setAudioData((qreal*)m_plotTime->getDigestCurve()->getSignalData(),m_plotTime->getDigestCurve()->sampleNumber());
}

void MainWindow::streamPositionUpdate(qreal position) {
    //TODO, ACTUALLY IF THE SR OF THE PROJECT CHANGES STREAM REMAIN AT DEFAULT SR
    Q_ASSERT(m_plotTime->sampleRate()==m_digestCurveStream->getAudioFormat().sampleRate());

    //qDebug() << "MainWindow::streamPositionUpdate: position=" <<position <<  "/"<< m_audioPlayer->actualStreamTotalTime();
    m_plotTime->setRubberBandPosition( position );
}

void MainWindow::updateCurvesName() {
    GenericTimeData * s;
    int i=0;
    s=m_plotTime->getTimeData(i);
    while(s) {
        s_widgetUI.toolboxOption->setItemText(m_toolBoxFixedItem+i, s->name());
        s=m_plotTime->getTimeData(++i);
    }
}

void MainWindow::exportDigestCurve() {
    ExportAudioFileDialog dialog;
    int format;
    int channels=1;
    int sampleRate=(int) m_plotTime->sampleRate();

    dialog.exec();
    QString filename=dialog.getSelectedFileName();
    char* outfilename=filename.toLocal8Bit().data();// see http://qt-project.org/faq/answer/how_can_i_convert_a_qstring_to_char_and_vice_versa
    if (!filename.isEmpty()) {
        switch (dialog.getSelecedType()) {
            case ExportAudioFileDialog::WAVE:
                format=SF_FORMAT_WAV | SF_FORMAT_FLOAT;
                break;
            case ExportAudioFileDialog::FLAC:
                format=SF_FORMAT_FLAC | SF_FORMAT_PCM_24;
                break;
            default:
                return;
        }
    }

    if (!SndfileHandle::formatCheck(format, channels, sampleRate) ) {
        QMessageBox::warning(this,"Can't save","The selected format is not supported. File not saved");
        return;
     }

    SndfileHandle outfile(outfilename, SFM_WRITE, format, channels, sampleRate);
    if (not outfile) {
        QString msg="Can't create file: ";
        msg.append(outfilename);
        QMessageBox::warning(this,"Can't save",msg);
        //A warning dialog here
        return;
    }

    const qreal * s=m_plotTime->getDigestCurve()->getSignalData();

    //Manca Min time!!!
    qreal length=m_plotTime->duration();

    outfile.write(s, length*sampleRate);

    QString msg="Saved file ";
    msg.append(outfilename);
    QMessageBox::information(this,"Save ok!",msg);
}

QDomDocument MainWindow::createDomDocument() {
    //Set up the main document
    QDomDocument _doc(PROJECT_DOCTYPE);
    QDomElement _rootNode = _doc.createElement(PROJECTROOT_TAG);
    _doc.appendChild(_rootNode);
    _rootNode.setAttribute(DOMHELPER_VERSION_ATTRIBUTE,DOMHELPER_VERSION);

    //Generate the document to handle the main window data
    DomHelper _docMainWindow=DomHelper(this);
    _docMainWindow.initDomDocument(WINDOW_TAG);

    //Append the window details to the document
    _rootNode.appendChild(_docMainWindow.getDomDocument()->firstChild());

    //Append the Time plot parameters to the document (SR and maxDuration for now 20121217)
    _rootNode.appendChild(m_plotTime->getTimePlotParametersDomDocument()->firstChild());

    //Debug
    qDebug() << "QDomDocument MainWindow::createDomDocument time plot params section\n" << m_plotTime->getTimePlotParametersDomDocument()->toDocument().toString();

    //Append all the data curves
    foreach(GenericTimeData* _gtd, m_plotTime->getTimeDataList()) {
        const QDomDocument * _d=_gtd->getDomDocument();
        if (_d==NULL) {
            qDebug() << "MainWindow::exportXML DOM data point to NULL, " << _gtd->name() <<" can't save document";
            continue;
        }
        qDebug() << "MainWindow::appendDomDocument  processing node " << _d->nodeName();
        if (!_d->isNull()) {
            _rootNode.appendChild(_d->firstChild());
        } else {
            qDebug() << "MainWindow::exportXML "<< _gtd->name() << " DOM has first node " << _d->nodeName() ;
        }
    }
    return _doc;
}

bool MainWindow::exportXML() {
    QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save File"),
                               ".",
                               tr("XML file (*.xml *.XML)"));
    QDomDocument _doc=createDomDocument();
    bool retval= DomHelper::save(fileName, _doc);
    return retval;
}

void MainWindow::showXML() {
    if (m_TreeWidgetshowXML!=NULL) {
        m_TreeWidgetshowXML->hide();
        delete m_TreeWidgetshowXML;
    }

    m_TreeWidgetshowXML=new QTreeWidget();
    m_TreeWidgetshowXML->setGeometry(300,200,300,400);
    Qt::WindowFlags flags = m_TreeWidgetshowXML->windowFlags();
    m_TreeWidgetshowXML->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

    if (m_TreeWidgetshowXML->isHidden() ) {
        m_TreeWidgetshowXML->setWindowTitle( QString("XML dump of project") );
        m_TreeWidgetshowXML->show();
        m_TreeWidgetshowXML->expandAll();
    }
    QDomDocument _doc=createDomDocument();
    DomHelper::parseDOMToQTreeWidget(&_doc,m_TreeWidgetshowXML);
    m_TreeWidgetshowXML->expandAll();
}

bool MainWindow::importXML() {
    QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open File"),
                               ".",
                               tr("XML file (*.xml *.XML)"));
    QDomDocument _doc;
    if (!DomHelper::load(fileName,&_doc)) {
        QMessageBox::warning(0,"MainWindow::importXML","Can't load XML file.");
        return false;
    }

    if (!_doc.isDocument() || _doc.firstChild().isNull()  ) {
        QMessageBox::warning(0,"MainWindow::importXML","DATA NOT SET! Document is not a valid DomDocument");
        return false;
    }

    QString _firstchild= _doc.firstChild().nodeName();
    if (QString::compare(_firstchild,PROJECTROOT_TAG)!=0)  {
        QMessageBox::warning(0, "MainWindow::importXML",QString("DATA NOT SET! Document  contains as first child  |%1| instead  of |%2|.").arg(_firstchild).arg(PROJECTROOT_TAG));
        return false;
    }
    QDomNode _root=_doc.firstChild();
    //clear all curves
    if( QMessageBox::question(this,"Confirm remove curves","Do you want to remove all the curves?",QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton) == QMessageBox::No ) return true;
    //Store old curve data, in order to go back if something goes wrong and then removes all curves.
    QDomDocument prevProject=createDomDocument();
    removeAllCurves();

    //Disable update
    m_plotTime->setEnableUpdate(false);

    //set project properties
    QDomNodeList _nodeListProject=_root.toElement().elementsByTagName(PROJECTPARAMETERS_TAG);
    qDebug() << "MainWindow::importXML()  _nodeListProject.length=" << _nodeListProject.length();
    Q_ASSERT(_nodeListProject.length()==1);
    QDomNode _nodeProjParam=_nodeListProject.at(0);
    m_plotTime->getTimePlotParams()->setClassByDomData(_nodeProjParam);
    m_plotTime->updateUI();//need to manual recal, all the update are disabled!

    QDomNodeList _nodeListCurves=_root.toElement().elementsByTagName(GENERICTIMEDATA_TAG);
    qDebug() << "MainWindow::importXML() find "  <<  _nodeListCurves.length() << " curves";
    for (unsigned int n=0; n < _nodeListCurves.length(); n++) {
        QDomNode _node=_nodeListCurves.at(n);
        qDebug() << "MainWindow::importXML() curve "<< n  << " tag="<<  _node.nodeName();

        //get class type
        QDomNodeList _nodeObjTypeList=_node.toElement().elementsByTagName(DOMHELPER_OBJECTTYPE_TAG);
      //  Q_ASSERT(_nodeObjTypeList.length()==1);
        QDomNode _nodeObjTypeName=_nodeObjTypeList.at(0);
        QString _objTypeName=DomHelper::getNodeValue(_nodeObjTypeName);
        qDebug() << "MainWindow::importXML() _nodeObjTypeList.length "<< _nodeObjTypeList.length()
                 << " tag="<<  _nodeObjTypeName.nodeName()
                 << " objname=" << _objTypeName;
        //allocate curve
        GenericTimeData* _curve=NULL;
        if (QString::compare(_objTypeName,"PartialSinusData")==0 ) {
            qDebug() << "MainWindow::importXML() init  PartialSinusData with SR=" << m_plotTime->getTimePlotParams()->sampleRate() << " duration=" << m_plotTime->getTimePlotParams()->duration();
            PartialSinusData * s=new PartialSinusData(m_plotTime->getTimePlotParams() , m_plotTime);//s_widgetUI.toolboxOption);
            _curve=(GenericTimeData*) s;
        }
        if (QString::compare(_objTypeName,"RepeatedSinusData")==0 ) {
            qDebug() << "MainWindow::importXML() init  RepeatedSinusData with SR=" << m_plotTime->getTimePlotParams()->sampleRate() << " duration=" << m_plotTime->getTimePlotParams()->duration();
            RepeatedSinusData * s=new RepeatedSinusData(m_plotTime->getTimePlotParams() , m_plotTime);
            _curve=(GenericTimeData*) s;
        }
        if (QString::compare(_objTypeName,"GenericSinusData")==0 ) {
            qDebug() << "MainWindow::importXML() init  GenericSinusData with SR=" << m_plotTime->getTimePlotParams()->sampleRate() << " duration=" << m_plotTime->getTimePlotParams()->duration();
            GenericSinusData * s=new GenericSinusData(m_plotTime->getTimePlotParams() , m_plotTime);
            _curve=(GenericTimeData*) s;
        }
        //set class with DomObject
        if (_curve!=NULL) {
            _curve->inihbitUpdate();
            if (!_curve->importXML(_node)) {
                QMessageBox::warning(0, "MainWindow::importXML",QString("DATA NOT SET! SetClassByDomData failed."));
                return false;
            }
            m_plotTime->addTimeData(_curve);
            //adding controls to plot
            QWidget *_widget=(QWidget*)_curve->getControlWidget();
            s_widgetUI.toolboxOption->addItem(_widget,_curve->name());
        } else {
            qWarning() << "MainWindow::importXML() invalid curve "<< n;
            QMessageBox::warning(0,"MainWindow::importXML", QString("Invalid curve %1").arg(n));
            return false;
        }
    }

    //enable again and force recalc
     m_plotTime->setEnableUpdate(true);
     m_plotTime->forceRecreateAll();//TODO: all the data curve are created during init.. this should not be needed.
     m_plotTime->updatePlot();



    return  true;

}
