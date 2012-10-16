#include "timeplotwidget.h"



TimePlotWidget::TimePlotWidget(QWidget *parent, int xScaleType, int yScaleType) :
    PlotWidget(parent,  xScaleType,  yScaleType)
{
    m_SR=TIMEDATA_DEFAULT_SR;
    m_t0=TIMEDATA_DEFAULT_MIN_TIME;
    m_duration=TIMEDATA_DEFAULT_MAX_TIME;
    m_digestCurve=new DigestTimeData(&m_curveList,m_duration,m_SR);
    m_digestCurve->getCurve()->attach(this);
    this->createControlWidget();

    this->setRubberBandPosition(0);
}

TimePlotWidget::~TimePlotWidget() {

}

void TimePlotWidget::setSampleRate(double SR) {
    m_SR=SR;

    int n=0;
    GenericTimeData * gtd=this->getTimeDataList(n);
    bool sigStatus;
    while (gtd!=NULL) {
        sigStatus=gtd->blockSignals(true);
        gtd->setSampleRate(SR);
        gtd->blockSignals(sigStatus);
        gtd=this->getTimeDataList(++n);
    }
    //update the digest curve
    sigStatus=m_digestCurve->blockSignals(true);
    m_digestCurve->setSampleRate(SR);
    m_digestCurve->blockSignals(sigStatus);

    //Update UI
    sigStatus=m_baseControl.sliderSR->blockSignals(true);
    m_baseControl.sliderSR->setValue(SR);
    this->updatePlot();
    m_baseControl.sliderSR->blockSignals(sigStatus);

    //Replot and recalc digest
    //this->updatePlot();
}

void TimePlotWidget::setDuration(double duration) {
    m_duration=duration;

    int n=0;
    GenericTimeData * gtd=this->getTimeDataList(n);
    bool sigStatus;
    while (gtd!=NULL) {
        sigStatus=gtd->blockSignals(true);
        gtd->setMaxDuration(duration);
        gtd->blockSignals(sigStatus);
        gtd=this->getTimeDataList(++n);
    }

    //update the digest curve
    sigStatus=m_digestCurve->blockSignals(true);
    m_digestCurve->setMaxDuration(m_duration);
    m_digestCurve->blockSignals(sigStatus);

    //Update UI
    sigStatus=m_baseControl.sliderDuration->blockSignals(true);
    m_baseControl.sliderDuration->setValue(m_duration);
    m_baseControl.sliderDuration->blockSignals(sigStatus);
    this->setAxisScale(xBottom, this->axisInterval(xBottom).minValue(), 1.1*duration);
    //Replot and recalc digest
    this->updatePlot();

}

void TimePlotWidget::createControlWidget() {
    //Init the control, ZMP. Other??
    m_baseControl.m_zmp=new ZMP_Handler(this->canvas());

    m_allControl=new QFrame();
    m_allControl->setFrameShape(QFrame::WinPanel);
    m_allControl->setFrameShadow(QFrame::Raised);
    m_allControl->setLayout(new QVBoxLayout());
    QVBoxLayout * l=(QVBoxLayout*)m_allControl->layout();

    this->initBaseControlWidget();
    l->addWidget(m_baseControl.baseControlWidget,1,Qt::AlignHCenter|Qt::AlignTop);
}

void TimePlotWidget::initBaseControlWidget() {
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout for the real control option
    m_baseControl.baseControlWidget=new QFrame();
   // m_baseControl.baseControlWidget->setSizePolicy(QSizePolicy::Maximum);
    QVBoxLayout * lBase=new QVBoxLayout();
    m_baseControl.baseControlWidget->setLayout(lBase) ;
    m_baseControl.baseControlWidget->hide();
    m_baseControl.baseControlWidget->setFont(f);

    //Adding the ZMP button control
    m_baseControl.ZMP_RadioButton.groupBox = new QGroupBox("Mouse Selection",NULL);
    m_baseControl.ZMP_RadioButton.PICKER =  new QRadioButton("Picker",(QWidget*)m_baseControl.ZMP_RadioButton.groupBox);
    m_baseControl.ZMP_RadioButton.PAN = new QRadioButton("Pan",(QWidget*)m_baseControl.ZMP_RadioButton.groupBox);
    m_baseControl.ZMP_RadioButton.ZOOM = new QRadioButton("Zoom",(QWidget*)m_baseControl.ZMP_RadioButton.groupBox);

    QVBoxLayout * radioButtonLayout = new QVBoxLayout();
    radioButtonLayout->addWidget(m_baseControl.ZMP_RadioButton.PICKER,1);
    radioButtonLayout->addWidget(m_baseControl.ZMP_RadioButton.PAN,1);
    radioButtonLayout->addWidget(m_baseControl.ZMP_RadioButton.ZOOM,1);
    m_baseControl.ZMP_RadioButton.groupBox->setLayout(radioButtonLayout);
    connect(m_baseControl.ZMP_RadioButton.PICKER,SIGNAL(clicked()),this,SLOT(ZMP_statusChanged()));
    connect(m_baseControl.ZMP_RadioButton.PAN,SIGNAL(clicked()),this,SLOT(ZMP_statusChanged()));
    connect(m_baseControl.ZMP_RadioButton.ZOOM,SIGNAL(clicked()),this,SLOT(ZMP_statusChanged()));
    m_baseControl.ZMP_RadioButton.PICKER->setChecked(true);
    this->ZMP_statusChanged();
    lBase->addWidget( m_baseControl.ZMP_RadioButton.groupBox,1,Qt::AlignLeft|Qt::AlignTop);

    //set Sample rate
    m_baseControl.sliderSR = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_baseControl.sliderSR->setScale(TIMEDATA_DEFAULT_MIN_SR,TIMEDATA_DEFAULT_MAX_SR,TIMEDATA_DEFAULT_STEP_SR);
    m_baseControl.sliderSR->setValue(m_SR);
    m_baseControl.sliderSR->setName("SR Generation");
    m_baseControl.sliderSR->setMeasureUnit("Hz");
    m_baseControl.sliderSR->setFont(f);
    lBase->addWidget( m_baseControl.sliderSR,1,Qt::AlignLeft|Qt::AlignTop);
    connect(m_baseControl.sliderSR,SIGNAL(valueChanged(double)),this,SLOT(setSampleRate(double)));

    //set duration
    m_baseControl.sliderDuration = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_baseControl.sliderDuration->setScale(0,TIMEDATA_DEFAULT_MAX_TIME,TIMEDATA_DEFAULT_TIMESTEP);
    m_baseControl.sliderDuration->setValue(m_duration);
    m_baseControl.sliderDuration->setName("Duration Generated file");
    m_baseControl.sliderDuration->setMeasureUnit("Sec.");
    m_baseControl.sliderDuration->setFont(f);
    lBase->addWidget( m_baseControl.sliderDuration,1, Qt::AlignLeft|Qt::AlignTop);
    connect(m_baseControl.sliderDuration,SIGNAL(valueChanged(double)),this,SLOT(setDuration(double)));

    m_baseControl.baseControlWidget->show();
}


void TimePlotWidget::ZMP_statusChanged() {
    if (m_baseControl.ZMP_RadioButton.PICKER->isChecked()) {
        m_baseControl.m_zmp->enablePicker();
    }
    if (m_baseControl.ZMP_RadioButton.PAN->isChecked()) {
        m_baseControl.m_zmp->enablePanner();
    }
    if (m_baseControl.ZMP_RadioButton.ZOOM->isChecked()) {
        m_baseControl.m_zmp->enableZoomer();
    }
}

void TimePlotWidget::setRubberBandPosition(qreal position) {
    m_scrollRubberBand->setValue(position);
}

