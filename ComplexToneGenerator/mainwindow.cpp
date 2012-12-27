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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initAudio() {
    m_digestCurveStream=new InternalStreamDevice(AudioUtils::getStandardFormat(AudioUtils::DAT));
    m_digestCurveStream->setAudioData((qreal*) m_plotTime->getDigestCurve()->getSignalData(),m_plotTime->getDigestCurve()->sampleNumber());
    m_audioPlayer->setStream(m_digestCurveStream);
}

void MainWindow::connectSignals() {
    //connect command buttons
    connect(s_button.addCurve,SIGNAL(clicked()),SLOT(newCurve()));
    connect(s_button.removeCurve,SIGNAL(clicked()),SLOT(removeCurve()));
    connect(s_button.duplicateCurves,SIGNAL(clicked()),SLOT(duplicateCurves()));

#ifdef COMPLETE_FAST_SELECTION
    connect(s_button.removeAllCurves,SIGNAL(clicked()),SLOT(removeAllCurvesWithDialog()));
    connect(s_button.exportDigest,SIGNAL(clicked()),SLOT(exportDigestCurve()));
    connect(s_button.exportXML ,SIGNAL(clicked()),SLOT(exportXML()));
    connect(s_button.importXML,SIGNAL(clicked()),SLOT(importXML()));
    connect(s_button.showXML ,SIGNAL(clicked()),SLOT(showXML()));
#endif
    //connect digest curve to handle update in the plots
    connect(m_plotTime->getDigestCurve(),SIGNAL(dataUpdated()),this,SLOT(digestCurveChanged()));

    //Connect position slider
    connect(m_audioPlayer,SIGNAL(streamTimePositionChanged(qreal)) ,this,SLOT(streamPositionUpdate(qreal)));
}

void MainWindow::setupUI() {
    connectMenusAndShortcut();
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
    this->setWindowTitle("New Complex Generator project");
}

void MainWindow::connectMenusAndShortcut() {
       // connect(ui-> ,SIGNAL(triggered()),this,SLOT );
    //---------File menu
    {
        //NEW
        connect(ui->actionNew_Project,SIGNAL(triggered()),this,SLOT(newProject()));//new
        //LOAD
        connect(ui->actionLoad_Project ,SIGNAL(triggered()),this,SLOT(load()));
        //SAVE
        connect(ui->actionSave_project,SIGNAL(triggered()),this,SLOT(save()));
        //SAVE AS
        connect(ui->actionSave_as,SIGNAL(triggered()),this,SLOT(saveAs()));
        //Import curve
        connect(ui->actionImport_curve,SIGNAL(triggered()),this,SLOT( importCurve()) );
        //EXPORT AUDIO FILE
        connect(ui->actionExport_audio_file,SIGNAL(triggered()),this,SLOT(exportDigestCurve()));
    }

    //---------Curves menu
    {
        //NEW CURVE
        connect(ui->actionAdd_curve,SIGNAL(triggered()),this,SLOT(newCurve()));
        //DUPLICATE
        connect(ui->actionDuplicate_curves,SIGNAL(triggered()),this,SLOT(duplicateCurves()));
        //REMOVE ALL
        connect(ui->actionRemove_all_curves,SIGNAL(triggered()),this,SLOT(removeAllCurvesWithDialog()) );
        //REMOVE ONE
        connect(ui->actionRemove_curve ,SIGNAL(triggered()),this,SLOT(removeCurve()) );
    }

    //---------Show menu
    connect(ui->actionShow_Proj_struct,SIGNAL(triggered()),this,SLOT(showXML()));

    //---------About menu


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

    //20121224 Added menu support, so buttons controls now are intended only for fast operations. Reactvate define COMPLETE_FAST_SELECTION in compile

    //Init buttons
    s_button.addCurve=new QPushButton("Add curve");
    s_button.removeCurve=new QPushButton("Remove curve");
    s_button.duplicateCurves=new QPushButton("Duplicate curves");
#ifdef COMPLETE_FAST_SELECTION
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
#endif
    //Layout the button panel
    _l->addWidget(s_button.addCurve);
    _l->addWidget(s_button.removeCurve);
    _l->addWidget(s_button.duplicateCurves);

#ifdef COMPLETE_FAST_SELECTION
    _l->addWidget(s_button.removeAllCurves);
    _l->addWidget(s_button.exportDigest);
    _l->addWidget(_buttonWidget);
#endif

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

 //       PartialSinusData * s=new PartialSinusData(m_plotTime->getTimePlotParams() , m_plotTime);//s_widgetUI.toolboxOption);

        GenericTimeData *s=CustomCurveFactory::instance()->newCurve("PartialSinusData");//,m_plotTime);
     //   s->setParent(m_plotTime);
        s->setTimePlotParams(m_plotTime->getTimePlotParams() );
        //s->setStartTime(1.414);
       // s->setDuration(5.1);
       // s->setAmplitudeFrequencyAndPhase(0.5,125,90);
        retval=(GenericTimeData*) s;
        return retval;
    }

    if (QString::compare(curveName,"Repeated duration Tone generator")==0 ) {
     //   RepeatedSinusData * s=new RepeatedSinusData(m_plotTime->getTimePlotParams() , m_plotTime);
     //   s->setStartTime(0.2);
     //   s->setDuration(0.5);
     //   s->setBlankTime(0.25);
     //   s->setAmplitudeFrequencyAndPhase(0.250,500,45);

        GenericTimeData *s=CustomCurveFactory::instance()->newCurve("RepeatedSinusData");//,m_plotTime);
       // s->setParent(m_plotTime);
        s->setTimePlotParams(m_plotTime->getTimePlotParams() );
        retval=(GenericTimeData*) s;
        return retval;
    }

    if (QString::compare(curveName,"Tone generator")==0 ) {
        GenericTimeData *s=CustomCurveFactory::instance()->newCurve("GenericSinusData");//,m_plotTime);
       // s->setParent(m_plotTime);
        s->setTimePlotParams(m_plotTime->getTimePlotParams() );
     //   GenericSinusData * s=new GenericSinusData(m_plotTime->getTimePlotParams() , m_plotTime);
     //   s->setAmplitudeFrequencyAndPhase(0.125,250,0);
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
     SelectMultipleCurvesWindowDialog * removeDialog=new SelectMultipleCurvesWindowDialog(&sl,this);
     removeDialog->setActionDialog("remove");
     removeDialog->exec();
     bool _firstTime=true;
     foreach (int i, removeDialog->getSelectedCurvesIndex()) {
            int _index;
            _index=(_firstTime ? i : i-1);
            s_widgetUI.toolboxOption->removeItem(i+m_toolBoxFixedItem);
            if (!m_plotTime->removeTimeData(_index)) {
                qWarning() << "MainWindow::removeCurve: can't remove GenericTimeData@index=" <<_index;
            } else {
                qDebug() << "MainWindow::removeCurve: removed GenericTimeData@index=" <<_index;
            }
     }
     delete removeDialog;
}

void MainWindow::removeAllCurves() {
     while (m_plotTime->removeTimeData(0))  {
        s_widgetUI.toolboxOption->removeItem(m_toolBoxFixedItem);
     }
}

void MainWindow::newProject() {
    if( QMessageBox::question(this,"Confirm New Porject","Do you want create a new project?\nAll Data will be lost",QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton) == QMessageBox::No ) return;
    CTG_app * _app=(CTG_app*) qApp;
    _app->setProjectName("");
    _app->setAudioDigestSaveName("");
    removeAllCurves();
    this->setWindowTitle("New Complex Generator project");
}

void MainWindow::load() {
    CTG_app * _app=(CTG_app*) qApp;
    QString _fileName = QFileDialog::getOpenFileName(this, tr("Open CTG project"),
                                             _app->projectSavePath(),
                                             tr("CTG project file (*.cpf *.CPF)"));
    if (_fileName=="") return;

    if (!importXML(_fileName)) {
        QMessageBox::warning(this,"Project not loaded","Project can't be loaded");
        return;
    }

    //saving path,name & title
    QFileInfo _fi(_fileName);
    QString _path=_fi.absolutePath();
    QString _name=_fi.baseName();
    if (_path!="")
        _app->setProjectSavePath(_path);
    if (_name!="") {
        _app->setProjectName(_name);
        this->setWindowTitle(_name);
    }
}

void MainWindow::save() {
    CTG_app * _app=(CTG_app*) qApp;

    //if the project wasn't already saved...
    if (_app->projectName()=="") {
        saveAs();
        return;
    }

    QString _fileName=QString("%1/%2.cpf").arg(_app->projectSavePath()).arg(_app->projectName());
    if (!exportXML(_fileName))
        QMessageBox::warning(this,"Error saving project","Project NOT saved");
}

void MainWindow::saveAs() {
    CTG_app * _app=(CTG_app*) qApp;

    QString _fileName=QString("%1/%2.cpf").arg(_app->projectSavePath()).arg(_app->projectName());
    _fileName = QFileDialog::getSaveFileName(this, tr("Save CTG project"),
                                                    _app->projectSavePath(),
                               tr("CTG project file (*.cpf *.CPF)"));
    if(_fileName=="") return;

    if (!exportXML(_fileName)) {
        QMessageBox::warning(this,"Error saving project","Project NOT saved");
        return;
    }

    //saving path,name & title
    QFileInfo _fi(_fileName);
    QString _path=_fi.absolutePath();
    QString _name=_fi.baseName();
    if (_path!="")
        _app->setProjectSavePath(_path);
    if (_name!="") {
        _app->setProjectName(_name);
        this->setWindowTitle(_name);
    }
}

void MainWindow::importCurve() {
    CTG_app * _app=(CTG_app*) qApp;
    QString _fileName = QFileDialog::getOpenFileName(this, tr("Import CTG curve"),
                                                     _app->curveSavePath(),
                                                     tr("CTG curve file (*.CCF *.ccf)"));
    if (_fileName=="") return;

    QDomDocument _doc;
    if (!DomHelper::load(_fileName,&_doc)) {
        QMessageBox::warning(0,"Curve not loaded","File can not be loaded.");
        return;
    }

    if (!importXMLCurve(_doc)) {
        QMessageBox::warning(this,"Curve not loaded","Invalid data structure.");
        return;
    }

    m_plotTime->forceUpdateUI();


    QFileInfo _fi(_fileName);
    QString _path=_fi.absolutePath();
    if (_path!="")
        _app->setCurveSavePath(_path);

}

void MainWindow::removeAllCurvesWithDialog() {
    if( QMessageBox::question(this,"Confirm remove curves","Do you want to remove all the curves?",QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton) == QMessageBox::No ) return;
    removeAllCurves();
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

void MainWindow::duplicateCurves() {
     QStringList  sl=m_plotTime->getTimeDataStringList();
     SelectMultipleCurvesWindowDialog * duplicateDialog=new SelectMultipleCurvesWindowDialog(&sl,this);
     duplicateDialog->setActionDialog("duplicate");
     duplicateDialog->exec();
     bool _prevValueEnablePlot=m_plotTime->setEnableUpdate(false);
     foreach (int i, duplicateDialog->getSelectedCurvesIndex()) {
         GenericTimeData * gtd=m_plotTime->getTimeData(i);
         if (gtd==NULL) {
             qWarning() << "MainWindow::duplicateCurves() can't import GenericTimeData @ index "<< i << ", return a NULL pointer. Going ahead";
             continue;
         }
         importXMLCurve(gtd->getDomDocument());
         gtd->setName(QString("%1_%2").arg(gtd->name()).arg("copy"));
     }
     m_plotTime->setEnableUpdate(_prevValueEnablePlot);
     m_plotTime->forceUpdateUI();
     m_plotTime->updatePlot();
     updateCurvesName();//This need a FIX,  should be made automatically with the set function
     delete duplicateDialog;
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
    const QDomDocument * _docTimeParams=m_plotTime->getTimePlotParametersDomDocument();
    _rootNode.appendChild(_docTimeParams->firstChild());

    //Debug
//    qDebug() << "MainWindow::createDomDocument time plot params section\n" << _docTimeParams->toString();

    //Append all the data curves
    foreach(GenericTimeData* _gtd, m_plotTime->getTimeDataList()) {
        const QDomDocument * _d=_gtd->getDomDocument();
        if (_d==NULL) {
            qDebug() << "MainWindow::createDomDocument DOM data point to NULL, " << _gtd->name() <<" can't save document";
            continue;
        }
        if (!_d->isNull()) {
            qDebug() << "MainWindow::createDomDocument  " << _gtd->name() << " appending " << _d->nodeName();
           // qDebug() << "MainWindow::createDomDocument " << _d->toString();
            _rootNode.appendChild(_d->firstChild());
        } else {
            qDebug() << "MainWindow::createDomDocument "<< _gtd->name() << " has node " << _d->nodeName() << "null";
        }
    }
    return _doc;
}

bool MainWindow::exportXML(const QString& filename) {
    QDomDocument _doc=createDomDocument();
    return DomHelper::save(filename, _doc);
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

bool MainWindow::importXML(const QString& fileName) {
    QDomDocument _doc;
    if (!DomHelper::load(fileName,&_doc)) {
        QMessageBox::warning(0,"MainWindow::importXML","Can't load XML file.");
        return false;
    }
    return importXML(_doc);
}

bool MainWindow::importXML(const QDomDocument& doc) {
    if (!doc.isDocument() || doc.firstChild().isNull()  ) {
        QMessageBox::warning(0,"MainWindow::importXML","DATA NOT SET! Document is not a valid DomDocument");
        return false;
    }

    QString _firstchild= doc.firstChild().nodeName();
    if (QString::compare(_firstchild,PROJECTROOT_TAG)!=0)  {
        QMessageBox::warning(0, "MainWindow::importXML",QString("DATA NOT SET! Document  contains as first child  |%1| instead  of |%2|.").arg(_firstchild).arg(PROJECTROOT_TAG));
        return false;
    }
    //clear all curves
    if( QMessageBox::question(this,"Confirm remove curves","Do you want to remove all the curves?",QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton) == QMessageBox::No ) return true;
    //Store old curve data, in order to go back if something goes wrong and then removes all curves.
    QDomDocument prevProject=createDomDocument();//TODO, revert curve!
    removeAllCurves();

    //Disable update
    bool _prevValueEnablePlot=m_plotTime->setEnableUpdate(false);

    //set project properties
    QDomNodeList _nodeListProject=doc.elementsByTagName(PROJECTPARAMETERS_TAG);
    qDebug() << "MainWindow::importXML()  _nodeListProject.length=" << _nodeListProject.length();

    if (_nodeListProject.length()!=1){
        QMessageBox::warning(0, "MainWindow::importXML",QString("DATA NOT SET! Document  contains an invalid number (%1) of root element.").arg(_nodeListProject.length()));
        qWarning() << "MainWindow::importXML() Document  contains an invalid number ("<< _nodeListProject.length() << ") of root element";
        qDebug() << "MainWindow::importXML() " << doc.toDocument().toString();
        return false;
    }

    QDomNode _nodeProjParam=_nodeListProject.at(0);
    m_plotTime->getTimePlotParams()->setClassByDomData(_nodeProjParam);
    m_plotTime->updateUI();//need to manual recal, all the update are disabled!

    if (!importXMLCurve(doc)) {
        qWarning() << "MainWindow::importXML() DATA NOT SET! can't import the XML curve part.";
        QMessageBox::warning(0, "MainWindow::importXML",QString("DATA NOT SET! can't import the XML curve part.").arg(_nodeListProject.length()));
        return false;
    }

    //enable again and force recalc
     m_plotTime->setEnableUpdate(_prevValueEnablePlot);
     m_plotTime->forceRecreateAll();
     m_plotTime->forceUpdateUI();
     m_plotTime->updatePlot();

    return  true;
}

bool MainWindow::importXMLCurve(const QDomDocument* doc) {
    return importXMLCurve(*doc);
}

bool MainWindow::importXMLCurve(const QDomDocument& doc) {
    if (!doc.isDocument() || doc.isNull() ) {
        qWarning() << "MainWindow::importXMLCurve() import not a valid document or null document";
        return false;
    }
     if( doc.firstChild().isNull()  ) {
         qWarning() << "MainWindow::importXMLCurve() document "<< doc.nodeName() << " has a null first child";
         return false;
     }

    QDomNodeList _nodeListCurves=doc.elementsByTagName(GENERICTIMEDATA_TAG);
    qDebug() << "MainWindow::importXML() find "  <<  _nodeListCurves.length() << " curves";
    for (unsigned int n=0; n < _nodeListCurves.length(); n++) {
        QDomNode _node=_nodeListCurves.at(n);
        qDebug() << "MainWindow::importXML() curve "<< n  << " tag="<<  _node.nodeName();

        //get class type
        QDomNodeList _nodeObjTypeList=_node.toElement().elementsByTagName(DOMHELPER_OBJECTTYPE_TAG);
      //  Q_ASSERT(_nodeObjTypeList.length()==1);
        QDomNode _nodeObjTypeName=_nodeObjTypeList.at(0);
        QString _objTypeName=DomHelper::getNodeValue(_nodeObjTypeName);
        qDebug() << "MainWindow::importXMLCurve() _nodeObjTypeList.length "<< _nodeObjTypeList.length()
                 << " tag="<<  _nodeObjTypeName.nodeName()
                 << " objname=" << _objTypeName;
        //allocate curve
        GenericTimeData* _curve=NULL;
        _curve=(GenericTimeData*)  CustomCurveFactory::instance()->newCurve(_objTypeName);

        //set class with DomObject
        if (_curve!=NULL) {
            _curve->inihbitUpdate();
            _curve->setTimePlotParams(m_plotTime->getTimePlotParams() );
            if (!_curve->importXML(_node)) {
                qWarning() << "MainWindow::importXMLCurve() invalid curve  curve->importXML(node) failed"<< n;
                //QMessageBox::warning(0, "MainWindow::importXML",QString("DATA NOT SET! SetClassByDomData failed."));
                return false;
            }
            m_plotTime->addTimeData(_curve);
            //adding controls to plot
            QWidget *_widget=(QWidget*)_curve->getControlWidget();
            s_widgetUI.toolboxOption->addItem(_widget,_curve->name());
            connect(_curve,SIGNAL(nameChanged()),this,SLOT(updateCurvesName()));
            _curve->enableUpdate();
        } else {
            qWarning() << "MainWindow::importXMLCurve() invalid curve "<< n;
            //QMessageBox::warning(0,"MainWindow::importXML", QString("Invalid curve %1").arg(n));
            return false;
        }
    }
    return true;
}
