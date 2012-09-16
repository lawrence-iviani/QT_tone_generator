#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_plotTime(new TimePlotWidget),
    m_plotFreq(new FreqPlotWidget)
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
    ui->scrollAreaLayout->addWidget(m_plotTime->getControlWidget(),1,Qt::AlignTop);
    ui->scrollAreaLayout->addWidget(m_plotFreq->getControlWidget(),1,Qt::AlignTop);

    m_plotFreq->setBothAxisScale(PlotWidget::Logarithmic,20.0,20000.0,PlotWidget::Linear, -40.0,0.0);

    m_lastIndexCurve=-1;
    m_indexGenerator=1;
    QFont f=ui->comboBoxCurve->font();
    f.setPointSize(9);
    ui->comboBoxCurve->setFont(f);
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

    s->setColor(Qt::black);
    s->setName(name);

    //adding data to the plot time
    m_plotTime->addTimeData(s);

    //adding controls to plot
    ui->scrollAreaLayout->addWidget(s->getControlWidget());

    //Adding data to the combo box
    ui->comboBoxCurve->addItem(name);
    ui->comboBoxCurve->setCurrentIndex(ui->comboBoxCurve->count()-1);
    m_lastIndexCurve=ui->comboBoxCurve->currentIndex();
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
        s->setAmplitudeFrequencyAndPhase(0.707,10,90);
        retval=(GenericTimeData*) s;
        return retval;
    }
    return retval;
}

void MainWindow::timeDataUpdated() {
    ui->comboBoxCurve->setItemText(m_lastIndexCurve,m_plotTime->getTimeData(m_lastIndexCurve)->name());
    m_plotTime->replot();
    //replot the digest curve too!
}

void MainWindow::removeCurve(){
    int index=ui->comboBoxCurve->currentIndex();

    GenericTimeData * s=(GenericTimeData*)m_plotTime->getTimeData(index);
    if (s!=NULL) {
        //Removing from combo box
        ui->comboBoxCurve->removeItem(index);

        //Removing controls widget
        ui->scrollAreaLayout->removeWidget(s->getControlWidget());
        delete s->getControlWidget();

        //Disconnecting signal
        //disconnect(s,SIGNAL(dataUpdated()),this,SLOT(timeDataUpdated()));

        //Remove curve from plot
        m_plotTime->removeTimeData(index);
    }

    //Setting new values
    m_lastIndexCurve=ui->comboBoxCurve->currentIndex();
    s=(GenericTimeData*)m_plotTime->getTimeData(m_lastIndexCurve);
    if  (s!=NULL) {
        s->getControlWidget()->show();
    }
}

void MainWindow::changedCurve(int index) {
    if (index==m_lastIndexCurve) return;
    GenericTimeData * s=(GenericTimeData*)m_plotTime->getTimeData(m_lastIndexCurve);
    if (s!=NULL) {
        s->getControlWidget()->hide();
    }

    index=ui->comboBoxCurve->currentIndex();
    s=(GenericTimeData*)m_plotTime->getTimeData(index);
    if  (s!=NULL) {
        s->getControlWidget()->show();
        //adding controls to plot
        m_lastIndexCurve=index;
    } else {
        m_lastIndexCurve=-1;
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
