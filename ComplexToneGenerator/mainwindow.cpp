#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sinusdata.h"
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
    //TODO: missing the sync between the curve and the controls!!!

    s->setColor(Qt::black);
    s->setName(name);

    //adding data to the plot time
    m_plotTime->addTimeData(s);

    //connecting data to the interface
    connect(s,SIGNAL(dataUpdated()),this,SLOT(dataUpdated()));

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
    t.description="This curve generate a constant zero signal (not so useful :) )";
    selectCurveHelper->addData(t);

    t.name="Tone generator";
    t.description="This curve generate a pure tone";
    selectCurveHelper->addData(t);
}

GenericTimeData *  MainWindow::decodeSelectedCurve(SelectCurveWindowHelper * selectCurveHelper) {
    GenericTimeData * retval=NULL;

    if (QString::compare(selectCurveHelper->getSelectedDataCurve().name,"Base curve") ) {
        retval=new GenericTimeData();
        return retval;
    }
    if (QString::compare(selectCurveHelper->getSelectedDataCurve().name,"Tone generator") ) {
        SinusData * s=new SinusData();
        s->setStartTime(0.4);
        s->setDuration(5.1);
        s->setAmplitudeFrequencyAndPhase(0.707,10,90);
        retval=(GenericTimeData*) s;
        return retval;
    }
    return retval;
}

void MainWindow::dataUpdated() {
    ui->comboBoxCurve->setItemText(m_lastIndexCurve,m_plotTime->getTimeData(m_lastIndexCurve)->name());
    m_plotTime->replot();
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
        disconnect(s,SIGNAL(dataUpdated()),this,SLOT(dataUpdated()));

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
