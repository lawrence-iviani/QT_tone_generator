#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_plotTime(new TimePlotWidget),
    m_plotFreq(new FreqPlotWidget),
    m_indexGenerator(1),
    m_toolBoxFixedItem(0),
    ui(new Ui::MainWindow),
    m_TreeWidgetshowXML(0),
    m_widgetStyleUI("darkorange"),
    m_widgetStylePlot("lightorange")
{
    TimePlotParams* _params=dynamic_cast<TimePlotParams*> (m_plotTime->getDataParameters());
    Q_ASSERT(_params);
    m_audioPlayer=new AudioPlayer(this);
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
    QAudioFormat _af=AudioUtils::getStandardFormat(AudioUtils::DAT);
    TimePlotParams* _params=dynamic_cast<TimePlotParams*> (m_plotTime->getDataParameters());
    Q_ASSERT(_params);
    _af.setSampleRate(_params->sampleRate());
    m_digestCurveStream=new InternalStreamDevice(_af);
    m_digestCurveStream->setAudioData((qreal*) m_plotTime->getDigestCurve()->getSignalData(),m_plotTime->getDigestCurve()->getSampleNumber());
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

    //If the sample rate change i need to reset the audio stream
   // connect(m_plotTime->getDigestCurve(),SIGNAL(sampleRateChanged(qreal)),this,SLOT(sampleRateChange(qreal)));

    //connect digest curve to handle update in the plots
    Q_ASSERT(connect(m_plotTime->getDigestCurve(),SIGNAL(dataChanged()),this,SLOT(digestCurveChanged())));

    //Connect position slider
    Q_ASSERT(connect(m_audioPlayer,SIGNAL(streamTimePositionChanged(qreal)) ,this,SLOT(streamPositionUpdate(qreal))));
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

    setupStyle();
}

void MainWindow::setupStyle() {
    //What i want to setup with a different style?
    m_widgetStyleUI.setStyle(s_widgetUI.toolboxOption);
    m_widgetStyleUI.setStyle(s_widgetUI.toolboxOptionFrame);
    m_widgetStyleUI.setStyle(s_widgetUI.buttonsFrame);
    m_widgetStyleUI.setStyle(s_widgetUI.commandSplitter);
    m_widgetStylePlot.setStyle(s_widgetUI.globalSplitter);
    m_widgetStylePlot.setStyle(s_widgetUI.plotSplitter);
}

void MainWindow::connectMenusAndShortcut() {
       // connect(ui-> ,SIGNAL(triggered()),this,SLOT );
    //---------File menu
    {
        //NEW
        Q_ASSERT(connect(ui->actionNew_Project,SIGNAL(triggered()),this,SLOT(newProject())));
        //LOAD
        Q_ASSERT(connect(ui->actionLoad_Project ,SIGNAL(triggered()),this,SLOT(load())));
        //SAVE
        Q_ASSERT(connect(ui->actionSave_project,SIGNAL(triggered()),this,SLOT(save())));
        //SAVE AS
        Q_ASSERT(connect(ui->actionSave_as,SIGNAL(triggered()),this,SLOT(saveAs())));
        //Import curve
        Q_ASSERT(connect(ui->actionImport_curve,SIGNAL(triggered()),this,SLOT( importCurve())));
        //EXPORT AUDIO FILE
        Q_ASSERT(connect(ui->actionExport_audio_file,SIGNAL(triggered()),this,SLOT(exportDigestCurve())));
    }

    //---------Curves menu
    {
        //NEW CURVE
        Q_ASSERT(connect(ui->actionAdd_curve,SIGNAL(triggered()),this,SLOT(newCurve())));
        //DUPLICATE
        connect(ui->actionDuplicate_curves,SIGNAL(triggered()),this,SLOT(duplicateCurves()));
        //REMOVE ALL
        Q_ASSERT(connect(ui->actionRemove_all_curves,SIGNAL(triggered()),this,SLOT(removeAllCurvesWithDialog())));
        //REMOVE ONE
        Q_ASSERT(connect(ui->actionRemove_curve ,SIGNAL(triggered()),this,SLOT(removeCurve())));
    }

    //---------Show menu
    // connect(ui->actionShow_Proj_struct,SIGNAL(triggered()),this,SLOT(showXML()));

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
    m_plotTime->setBothAxisScale(TIMEDATA_DEFAULT_MIN_TIME,TIMEDATA_DEFAULT_PROJECT_TIME,-1.0,1.0);
    m_plotFreq->setBothAxisScale(PlotWidget::Logarithmic,20.0,20000.0,PlotWidget::Linear, -40.0,0.0);
}

//SLOTS----------
void MainWindow::newCurve() {
    GenericTimeData* s=CustomCurveFactory::newDialogCurve(this,&m_widgetStyleUI);
    if (s==NULL) return;
    TimePlotParams* _tParams=dynamic_cast<TimePlotParams*>(m_plotTime->getDataParameters());
    Q_ASSERT(_tParams);
    s->setTimePlotParams(_tParams);
    QString name= "Curve_";
    name.append(QString::number(m_indexGenerator++));
    GenericTimeDataParams* _gtdParams=dynamic_cast<GenericTimeDataParams*>(s->getDataParameters());
    Q_ASSERT(_gtdParams);
    _gtdParams->setColor(Qt::green);
    _gtdParams->setName(name);
    _gtdParams->setShowCurve(true);

    addCurve(s);
}

void MainWindow::addCurve(GenericTimeData* gtd) {

    Q_ASSERT(connect(gtd,SIGNAL(nameChanged(QString)),this,SLOT(updateCurvesName())));
    //adding data to the plot time
    m_plotTime->addTimeData(gtd);

    //adding controls to plot
    GenericTimeDataParams* _curveParams=dynamic_cast<GenericTimeDataParams*> (gtd->getDataParameters());
    Q_ASSERT(_curveParams);
    s_widgetUI.toolboxOption->addItem(gtd->getControlWidget(),_curveParams->name());
}

void MainWindow::timeDataUpdated() {
    m_plotTime->replot();
}

void MainWindow::removeCurve(){
     QStringList  sl=m_plotTime->getTimeDataStringList();
     SelectMultipleCurvesWindowDialog * removeDialog=new SelectMultipleCurvesWindowDialog(&sl,this);
     removeDialog->setActionDialog("remove");
     m_widgetStyleUI.setStyle(removeDialog);
     removeDialog->exec();
     QList<int> _selctedCurvesList=removeDialog->getSelectedCurvesIndex();
     qSort(_selctedCurvesList);
     int _extracted=0;
     bool _prevValueEnablePlot=m_plotTime->setEnablePlot(false);
     foreach (int i, _selctedCurvesList) {
            int _index=i-_extracted;
            int _indexui=_index+m_toolBoxFixedItem;
            _extracted++;
            qDebug() << "MainWindow::removeCurve: extraction n. " << _extracted
                     << " absolute ID " << i
                     << "going to extract GenericTimeData@index=" <<_index
                     << " UIindex="<< _indexui;
            if (!m_plotTime->removeTimeData(_index)) {               
                qWarning() << "MainWindow::removeCurve: can't remove GenericTimeData@index=" <<_index;
            } else {
                 s_widgetUI.toolboxOption->removeItem(_indexui);
                //delete s_widgetUI.toolboxOption->widget(_indexui);
                qDebug() << "MainWindow::removeCurve: removed GenericTimeData@index=" <<_index;
            }
     }
     m_plotTime->setEnablePlot(_prevValueEnablePlot);
     m_plotTime->recalcAndUpdatePlot();
     this->digestCurveChanged();
     delete removeDialog;
}

void MainWindow::removeAllCurves() {
    //Disable update
    bool _prevValueEnablePlot=m_plotTime->setEnablePlot(false);
    while (m_plotTime->removeTimeData(0))  {
        s_widgetUI.toolboxOption->removeItem(m_toolBoxFixedItem);
        //delete s_widgetUI.toolboxOption->widget(m_toolBoxFixedItem);
     }
    //enable again and force recalc
     m_plotTime->setEnablePlot(_prevValueEnablePlot);
     m_plotTime->recalcAndUpdatePlot();
     this->digestCurveChanged();
}

void MainWindow::newProject() {
    if( QMessageBox::question(this,"Confirm New Porject","Do you want create a new project?\nAll Data will be lost",QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton) == QMessageBox::No ) return;
    CTG_app * _app=(CTG_app*) qApp;
    _app->setProjectName("");
    _app->setAudioDigestSaveName("");
    m_audioPlayer->setStart(false);
    removeAllCurves();
    this->setWindowTitle("New Complex Generator project");}

void MainWindow::removeAllCurvesWithDialog() {
    if( QMessageBox::question(this,"Confirm remove curves","Do you want to remove all the curves?",QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton) == QMessageBox::No ) return;
    removeAllCurves();
}

void MainWindow::digestCurveChanged() {
    TimePlotParams* _tParams=dynamic_cast<TimePlotParams*>(m_plotTime->getDataParameters());
    Q_ASSERT(_tParams);
    if (_tParams->sampleRate()!=m_audioPlayer->getAudioFormat().sampleRate()) {
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"incompatible project/audio stream sample rate project has SR=%1 Hz stream will be reproduced at m_audioPlayer->getAudioFormat()=%2")
                            .arg( ((GenericTimeDataParams*)m_plotTime->getDigestCurve()->getDataParameters())->sampleRate())
                            .arg(m_audioPlayer->getAudioFormat().sampleRate()));
    }

    //setting new curve with length and the actual format selected for the output stream
    m_digestCurveStream->setAudioData((qreal*)m_plotTime->getDigestCurve()->getSignalData(),m_plotTime->getDigestCurve()->getSampleNumber(),m_audioPlayer->getAudioFormat());
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"setting digest curve, digest SR=%1 Hz and stream SR=%2")
                      .arg( ((GenericTimeDataParams*)m_plotTime->getDigestCurve()->getDataParameters())->sampleRate())
                      .arg(m_audioPlayer->getAudioFormat().sampleRate()));
}

void MainWindow::sampleRateChange(qreal SR) {
    TimePlotParams* _tParams=dynamic_cast<TimePlotParams*>(m_plotTime->getDataParameters());
    Q_ASSERT(_tParams);
    qDebug() << "MainWindow::sampleRateChange changing sample rate to " << SR << " Hz";
    QAudioFormat _af=m_audioPlayer->getAudioFormat();
    _af.setSampleRate(SR);
    m_audioPlayer->setAudioFormat(_af);
    if (_tParams->sampleRate()!=m_audioPlayer->getAudioFormat().sampleRate()) {
        ErrorMessage _err=ErrorMessage(Q_FUNC_INFO,QString("incompatible project/audio stream sample rate project has SR=%1 Hz stream will be reproduced at m_audioPlayer->getAudioFormat()=%2")
                                       .arg(m_audioPlayer->getAudioFormat().sampleRate())
                                       .arg(_tParams->sampleRate())
                                       ,ErrorMessage::WARNINGMESSAGE);
        QMessageBox::warning(0,"MainWindow::sampleRateChange",_err.message());
    }
}

void MainWindow::streamPositionUpdate(qreal position) {

    //    Q_ASSERT(m_plotTime->sampleRate()==m_digestCurveStream->getAudioFormat().sampleRate());

    //qDebug() << "MainWindow::streamPositionUpdate: position=" <<position <<  "/"<< m_audioPlayer->actualStreamTotalTime();
    m_plotTime->setRubberBandPosition( position );
}

void MainWindow::updateCurvesName() {
    GenericTimeData * s;
    int i=0;
    s=m_plotTime->getTimeData(i);
    while(s) {
        s_widgetUI.toolboxOption->setItemText(m_toolBoxFixedItem+i, ((GenericTimeDataParams*) s->getDataParameters())->name());
        s=m_plotTime->getTimeData(++i);
    }
}

void MainWindow::exportDigestCurve() {
    ExportAudioFileDialog dialog;
    TimePlotParams* _tParams=dynamic_cast<TimePlotParams*>(m_plotTime->getDataParameters());
    Q_ASSERT(_tParams);
    int format;
    int channels=1;
    int sampleRate=(int) _tParams->sampleRate();
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
    qreal length=_tParams->maxDuration();

    outfile.write(s, length*sampleRate);

    QString msg="Saved file ";
    msg.append(outfilename);
    QMessageBox::information(this,"Save ok!",msg);
}

void MainWindow::duplicateCurves() {
     QStringList  sl=m_plotTime->getTimeDataStringList();
     SelectMultipleCurvesWindowDialog * duplicateDialog=new SelectMultipleCurvesWindowDialog(&sl,this);
     duplicateDialog->setActionDialog("duplicate");
     m_widgetStyleUI.setStyle(duplicateDialog);
     duplicateDialog->exec();

     //Create a list of selected items
     QList<GenericTimeData*> _tempListPointer;
     GenericTimeData * _pGtd;
     foreach (int i, duplicateDialog->getSelectedCurvesIndex()) {
         _pGtd=m_plotTime->getTimeData(i);
         if (_pGtd==NULL) {
             PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"can't import GenericTimeData @ index %1, return a NULL pointer. Going ahead")
                                 .arg(i));
             continue;
         }
         _tempListPointer.append(_pGtd);
     }

     TimePlotParams* _pTpParams=dynamic_cast<TimePlotParams*> (m_plotTime->getDataParameters());
     Q_ASSERT(_pTpParams);
     ErrorMessage _err;
     m_plotTime->setEnablePlot(false);
     foreach (_pGtd, _tempListPointer) {
         _err.clear();
         Q_ASSERT(_pGtd);
         GenericTimeDataParams* _pGtdParams=dynamic_cast<GenericTimeDataParams*> (_pGtd->getDataParameters());
         Q_ASSERT(_pGtdParams);
         GenericTimeData* _curve=NULL;
         QString _objTypeName=_pGtd->metaObject()->className();

         _curve=(GenericTimeData*)  CustomCurveFactory::instance()->newCurve(_objTypeName);//;,_pTpParams);
         _curve->setTimePlotParams(_pTpParams);
         if (!_curve) {
             QMessageBox::warning(this,"Error duplicate", QString("Curve %1 can't be duplicated").arg(_pGtdParams->name()));
             continue;
         }
         if (!_curve->importXML(_pGtd->exportXML(),&_err)) {
            QMessageBox::warning(this,"Error duplicate", QString("Error duplicating curve %1\n%2").arg(_pGtdParams->name()).arg(_err.message()));
            continue;
         }
         GenericTimeDataParams* _pCurveParams=dynamic_cast<GenericTimeDataParams*> (_curve->getDataParameters());
         Q_ASSERT(_pCurveParams);
         _pCurveParams->setName(QString("%1_%2").arg(_pGtdParams->name()).arg("copy"));
         addCurve(_curve);
     }
     m_plotTime->setEnablePlot(true);
     delete duplicateDialog;
}

QDomDocument MainWindow::composeDomDocument() {
    //Collecting the nodes
    QList<QDomNode> _nodeList;
    QDomNode _nTp=m_plotTime->exportXML().firstChild();
    Q_ASSERT(!_nTp.isNull());
    _nodeList.append(_nTp);

    for (int i=0; i<m_plotTime->getTimeDataList().length() ;i++) {
        qDebug() << Q_FUNC_INFO << " i=" << i;
        GenericTimeData* _gtd=m_plotTime->getTimeDataList().at(i);
        QDomNode _n=_gtd->exportXML().firstChild();
        Q_ASSERT(!_n.isNull());
        //qDebug() << Q_FUNC_INFO << " Inserting node\n" << DomHelperUtility::nodeToString(&_n);
       _nodeList.append(_n);
    }

    //Set up the main document
    QDomDocument _d=DomHelperUtility::createDocFromNodesList(_nodeList,
                                                             PROJECT_DOCTYPE,
                                                             PROJECT_TAG,
                                                             PROJECT_DOCVERSION);
    return _d;
}

void MainWindow::save() {
    CTG_app * _app=(CTG_app*) qApp;

    //if the project wasn't already saved...
    if (_app->projectName()=="") {
        saveAs();
        return;
    }

    QString _fileName=QString("%1/%2.%3").arg(_app->projectSavePath()).arg(_app->projectName()).arg(PROJECT_SUFFIX);

    ErrorMessage _err;
    if (!DomHelperUtility::save(_fileName,composeDomDocument(),&_err)) {
        QMessageBox::warning(NULL,QString("Error saving file"), QString("Error saving project %1\n%2")
                             .arg(_fileName)
                             .arg(_err.message()));
    }

}

void MainWindow::saveAs() {
    CTG_app * _app=(CTG_app*) qApp;

    QString _fileName=QString("%1/%2.%3").arg(_app->projectSavePath()).arg(_app->projectName()).arg(PROJECT_SUFFIX);
    _fileName = QFileDialog::getSaveFileName(this, tr("Save CTG project"),
                                                    _app->projectSavePath(),
                                             tr("Curve file (*.%1 *.%2)")
                                                   .arg(QString(PROJECT_SUFFIX).toLower())
                                                   .arg(QString(PROJECT_SUFFIX).toUpper()));
    if(_fileName=="") return;

    ErrorMessage _err;
    QDomDocument _d=composeDomDocument();
   // qDebug() << Q_FUNC_INFO << "FINAL DOCUMENT \n" << _d.toString();
    if (!DomHelperUtility::save(_fileName,_d,&_err)) {
        QMessageBox::warning(NULL,QString("Error saving file"), QString("Error saving project %1\n%2")
                             .arg(_fileName)
                             .arg(_err.message()));
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

void MainWindow::load() {
    CTG_app * _app=(CTG_app*) qApp;
    QString _fileName = QFileDialog::getOpenFileName(this, tr("Load CTG project"),
                                                     _app->projectSavePath(),
                                                     tr("Curve file (*.%1 *.%2)")
                                                    .arg(QString(PROJECT_SUFFIX).toLower())
                                                    .arg(QString(PROJECT_SUFFIX).toUpper()));
    if (_fileName=="") return;

    //saving path,name & title
    QFileInfo _fi(_fileName);
    QString _path=_fi.absolutePath();
    QString _name=_fi.baseName();
    if (_path!="")
        _app->setProjectSavePath(_path);

    QDomDocument _project;
    ErrorMessage _err;

    //clear all curves
    if( QMessageBox::question(this,"Confirm remove curves","Do you want to remove all the curves?",QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton) == QMessageBox::No ) return;
    //Store old curve data, in order to go back if something goes wrong and then removes all curves.
    QDomDocument _prevProject=composeDomDocument();//TODO, revert curve!
    removeAllCurves();

    //Load File
    if (!DomHelperUtility::load(_fileName,_project,&_err)) {
        QMessageBox::warning(NULL,QString("Error loading file"), QString("Error loading project %1\n%2")
                             .arg(_fileName)
                             .arg(_err.message()));
        return;
    }

    //Import the DOM document
    if (!importDomDocument(_project,&_err) ) {
        QMessageBox::warning(NULL,QString("Error import curve"), QString("Error importing project data\n%1").arg(_err.message()));
        _err.clear();
        removeAllCurves();
        if (importDomDocument(_prevProject,&_err) )
            QMessageBox::warning(NULL,QString("Previous project reverted"), QString("The previous project was succesfully reverted"));
        else {
            QMessageBox::warning(NULL,QString("Error reverting project"), QString("Error reverting project/n%1").arg(_err.message()));
        }
        return;
    }
    //Changing window title
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

    //Save file path
    QFileInfo _fi(_fileName);
    QString _path=_fi.absolutePath();
    if (_path!="")
        _app->setCurveSavePath(_path);
    QDomDocument _doc;
    ErrorMessage _err;

    //load curve
    if (!DomHelperUtility::load(_fileName,&_doc,&_err)) {
        QMessageBox::warning(NULL,QString("Error importing curve"), QString("Error importing curve %1\n%2")
                             .arg(_fileName)
                             .arg(_err.message()));
        return;
    }

    //Create&set curve
    QDomNode _node=_doc.firstChild();
    if (_node.isNull()) {
        QMessageBox::warning(NULL,QString("Error loading file"), QString("Error loading curve %1").arg(_fileName));
        return;
    }
    QString _objTypeName=GenericTimeData::getObjectType(_node);
    GenericTimeData* _curve=NULL;
    TimePlotParams* _tpp=dynamic_cast<TimePlotParams*> (m_plotTime->getDataParameters());
    Q_ASSERT(_tpp);
    _curve=(GenericTimeData*)  CustomCurveFactory::instance()->newCurve(_objTypeName);
    _curve->setTimePlotParams(_tpp);
    Q_ASSERT(_curve);
    //Setting data to the curve
    if (!_curve->importXML(_node,&_err)) {
        QMessageBox::warning(NULL,QString("Error setting curve"), QString("Error setting curve %1\n%2").arg(_fileName).arg(_err.message()));
        return;
    }
    //adding the curve
    addCurve(_curve);
}

bool MainWindow::importDomDocument(const QDomDocument& doc, ErrorMessage *err) {
    ErrorMessage _localErr;
    QDomNode _rootNode=doc.firstChild();
    if (doc.isNull() || !doc.isDocument() || _rootNode.isNull()) {
        QString msg="Importing Null data document";
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,msg,ErrorMessage::WARNINGMESSAGE));
        return false;
    }

    //Verify doctype
    if (doc.doctype().name()!=PROJECT_DOCTYPE) {
        QString msg=QString("Invalid doc type, expected -%1- found -%2-").arg(doc.doctype().toText().data()).arg(PROJECT_DOCTYPE);
        if (err){
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        }  PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,_localErr.message(),ErrorMessage::WARNINGMESSAGE));
        return false;
    }

//////------------- 1 Setting project Parameters
//////------------- 2 Importing and setting curves
    //1a------------- RETRIEVING TIMEDATAPARAMS NODE
    QDomNodeList _nodeListProjectTimeDataParams;
    if(!DomHelperUtility::nodeListByTagName(_nodeListProjectTimeDataParams,
                                            _rootNode,
                                            PROJECTTIMEPARAMETERS_TAG,
                                            PROJECTTIMEPARAMS_DOCVERSION,
                                            &_localErr)) {
        if (err) {
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,_localErr.message(),ErrorMessage::WARNINGMESSAGE));
        return false;
    }
    //Verify node
    if (_nodeListProjectTimeDataParams.length()==0) {
        QString msg="Error parsing project parameters, no valid parameters node";
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,msg,ErrorMessage::WARNINGMESSAGE));
        return false;
    }
    if (_nodeListProjectTimeDataParams.length()>1) {
        QString msg="Error parsing project parameters, too many parameters nodes";
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,msg,ErrorMessage::WARNINGMESSAGE));
        return false;
    }
    //1B------------- SETTING TIME DATA PARAMS
    if (!m_plotTime->importXML(_nodeListProjectTimeDataParams.at(0),&_localErr)) {
        QString msg="Error setting project parameters";
        if (err) {
            _localErr.setMethod(Q_FUNC_INFO);
            _localErr.setMessage(msg);
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,msg,ErrorMessage::WARNINGMESSAGE));
        return false;
    }

    //2A------------- IMPORTING CURVES
    QDomNodeList _nodeListCurves;
    if(!DomHelperUtility::nodeListByTagName(_nodeListCurves,
                                            _rootNode,
                                            TIMEDATACURVE_TAG,
                                            TIMEDATACURVE_DOCVERSION,
                                            &_localErr)) {
        if (err) {
            err->appendMessage(_localErr);
        } else PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,_localErr.message(),ErrorMessage::WARNINGMESSAGE));
        return false;
    }
    TimePlotParams* _tpp=dynamic_cast<TimePlotParams*> (m_plotTime->getDataParameters());
    Q_ASSERT(_tpp);

    //2B------------- CREATE & SETTING CURVE
    m_plotTime->setEnablePlot(false);
    for (unsigned int n=0; n < _nodeListCurves.length(); n++) {
        QDomNode _node=_nodeListCurves.at(n);
        QString _objTypeName=GenericTimeData::getObjectType(_node);
        GenericTimeData* _curve=NULL;
        _curve=(GenericTimeData*)  CustomCurveFactory::instance()->newCurve(_objTypeName);
        _curve->setTimePlotParams(_tpp);
        Q_ASSERT(_curve);
        //Setting data to the curve
        ErrorMessage _errAddingCurve;
        if (!_curve->importXML(_node,&_errAddingCurve)) {
            if (err) {
                err->appendMessage(_errAddingCurve);
            } else PRINT_WARNING(ErrorMessage(Q_FUNC_INFO,_errAddingCurve.message(),ErrorMessage::WARNINGMESSAGE));
            m_plotTime->setEnablePlot(true);
            return false;
        }
        //adding the curve
        addCurve(_curve);
    }
    m_plotTime->setEnablePlot(true);
    return true;
}

