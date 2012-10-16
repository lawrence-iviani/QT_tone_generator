#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_plotTime(new TimePlotWidget),
    m_plotFreq(new FreqPlotWidget),
    m_audioPlayer(new AudioPlayer),
    m_indexGenerator(1),
    m_toolBoxFixedItem(0)
{
    ui->setupUi(this);

    ui->centralWidget->setLayout(ui->gridLayoutCentralWidget);
    m_FramePlot=ui->framePlot;
    m_FrameCurves=ui->frameCurves;
    m_FrameButton=ui->frameButton;
    m_WidgetFreqPlot=ui->widgetFreqPlot;
    m_WidgetTimePlot=ui->widgetTimePlot;

    m_FramePlot->setLayout(ui->layoutPlotFrame);
    m_FrameCurves->setLayout(ui->layoutActualCurvesFrame);
    m_FrameButton->setLayout(ui->layoutButtonFrame);

    m_WidgetFreqPlot->setLayout(ui->layoutFreqPlot);
    m_WidgetTimePlot->setLayout(ui->layoutTimePlot);

    ui->layoutFreqPlot->addWidget(m_plotFreq);
    ui->layoutTimePlot->addWidget(m_plotTime);

    ui->scrollAreaWidgetContents->setLayout(ui->scrollAreaLayout);

    m_plotTime->setBothAxisScale(TIMEDATA_DEFAULT_MIN_TIME,TIMEDATA_DEFAULT_MAX_TIME,-1.0,1.0);

    //Adding widget to the toolbox option
    ui->toolBoxOptions->removeItem(m_toolBoxFixedItem);//QT creator doesn't leaves to remove the first items. Little trick
    ui->toolBoxOptions->insertItem(m_toolBoxFixedItem++,m_plotTime->getControlWidget(),"Time Option");
    ui->toolBoxOptions->insertItem(m_toolBoxFixedItem++,m_plotFreq->getControlWidget(),"Freq Option");

    //Setting audio player control and digest curve stream
    ui->toolBoxOptions->insertItem(m_toolBoxFixedItem++,m_audioPlayer->getAudioControlWidget(),"Audio Player");
    m_plotFreq->setBothAxisScale(PlotWidget::Logarithmic,20.0,20000.0,PlotWidget::Linear, -40.0,0.0);

    //connect digest curve to handle update in the plots
    connect(m_plotTime->getDigestCurve(),SIGNAL(dataUpdated()),this,SLOT(digestCurveChanged()));
    connect(m_audioPlayer,SIGNAL(streamTimePositionChanged(qreal)) ,this,SLOT(streamPositionUpdate(qreal)));

    m_digestCurveStream=new InternalStreamDevice(AudioUtils::getStandardFormat(AudioUtils::DAT));
    m_digestCurveStream->setAudioData(m_plotTime->getDigestCurve()->getSignalData(),m_plotTime->getDigestCurve()->sampleNumber());
    m_audioPlayer->setStream(m_digestCurveStream);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newCurve() {
    QString name= "Curve_";
    name.append(QString::number(m_indexGenerator++));

    SelectCurveWindowHelper * selectCurveHelper=SelectCurveWindowDialog::getDialogCurveHelper();
    this->setupCurves(selectCurveHelper);
    SelectCurveWindowDialog * selectDialog=new SelectCurveWindowDialog(selectCurveHelper,this);
    selectDialog->exec();
    GenericTimeData * s=this->decodeSelectedCurve(selectCurveHelper);

    s->setColor(Qt::white);
    s->setName(name);
    connect(s,SIGNAL(nameChanged()),this,SLOT(updateCurvesName()));

    //adding data to the plot time
    m_plotTime->addTimeData(s);

    //adding controls to plot
    ui->toolBoxOptions->addItem(s->getControlWidget(),s->name());

    delete selectCurveHelper;
    delete selectDialog;
}

void MainWindow::setupCurves(SelectCurveWindowHelper * selectCurveHelper) {
    //TODO, this should be decoded with a file xml or similar

    S_DataCurve t;
    t.name="Base curve";
    t.description="  This curve generate a constant zero signal (not so useful :) )  ";
    selectCurveHelper->addData(t);

    t.name="Tone generator";
    t.description=" This curve generate a pure tone ";
    selectCurveHelper->addData(t);
}

GenericTimeData *  MainWindow::decodeSelectedCurve(SelectCurveWindowHelper * selectCurveHelper) {
    GenericTimeData * retval=NULL;

    if (QString::compare(selectCurveHelper->getSelectedDataCurve().name,"Base curve") ) {
        retval=new GenericTimeData(m_plotTime->duration() , m_plotTime->sampleRate());
        return retval;
    }
    if (QString::compare(selectCurveHelper->getSelectedDataCurve().name,"Tone generator") ) {
        SinusData * s=new SinusData(m_plotTime->duration() , m_plotTime->sampleRate());
        s->setStartTime(0.4);
        s->setDuration(5.1);
        s->setAmplitudeFrequencyAndPhase(0.707,1000,90);
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
            if (!m_plotTime->removeTimeData(i)) {
                qWarning() << "MainWindow::removeCurve: can't remove GenericTimeData@index=" <<i;
            } else {
                qDebug() << "MainWindow::removeCurve: removed GenericTimeData@index=" <<i;
            }

     }
     delete removeDialog;
}

void MainWindow::digestCurveChanged() {
    m_digestCurveStream->setAudioData(m_plotTime->getDigestCurve()->getSignalData(),m_plotTime->getDigestCurve()->sampleNumber());
}

void MainWindow::streamPositionUpdate(qreal position) {
    //TODO, ACTUALLY IF THE SR OF THE PROJECT CHANGES STREAM REMAIN AT DEFAULT SR
    Q_ASSERT(m_plotTime->sampleRate()==m_digestCurveStream->getAudioFormat().sampleRate());

    qDebug() << "MainWindow::streamPositionUpdate: position=" <<position <<  "/"<< m_audioPlayer->actualStreamTotalTime();
    m_plotTime->setRubberBandPosition( position );
}

void MainWindow::updateCurvesName() {
    GenericTimeData * s;
    int i=0;
    s=m_plotTime->getTimeDataList(i);
    while(s) {
        ui->toolBoxOptions->setItemText(m_toolBoxFixedItem+i, s->name());
        s=m_plotTime->getTimeDataList(++i);
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
                format= SF_FORMAT_FLAC | SF_FORMAT_PCM_24;
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

    double * s=m_plotTime->getDigestCurve()->getSignalData();

    //Manca Min time!!!
    double length=m_plotTime->duration();

    outfile.write(s, length*sampleRate);

    QString msg="Saved file ";
    msg.append(outfilename);
    QMessageBox::information(this,"Save ok!",msg);
}
