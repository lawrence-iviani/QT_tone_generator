#include "timeplotwidget.h"

TimePlotWidget::TimePlotWidget(QWidget *parent, int xScaleType, int yScaleType) :
    PlotWidget(parent,  xScaleType,  yScaleType)
{
    m_SR=TIMEDATA_DEFAULT_SR;
    m_t0=TIMEDATA_DEFAULT_MIN_TIME;
    m_duration=TIMEDATA_DEFAULT_MAX_TIME;
    this->createControlWidget();
}

void TimePlotWidget::setSampleRate(double SR) {
    int n=0;
    GenericTimeData * gtd=this->getTimeData(n);
    while (gtd!=NULL) {
        gtd->setSampleRate(SR);
        gtd=this->getTimeData(++n);
    }
    m_SR=SR;
    m_baseControl.sliderSR->setValue(SR);
}

void TimePlotWidget::setDuration(double duration) {
    int n=0;
    GenericTimeData * gtd=this->getTimeData(n);
    while (gtd!=NULL) {
        gtd->setMaxDuration(duration);
        gtd=this->getTimeData(++n);
    }
    m_duration=duration;
    m_baseControl.sliderDuration->setValue(m_duration);
}

void TimePlotWidget::createControlWidget() {
    //Init the control, ZMP. Other??
    m_baseControl.m_zmp=new ZMP_Handler(this->canvas());

    m_allControl=new QWidget();
    m_allControl->setLayout(new QVBoxLayout());
    QVBoxLayout * l=(QVBoxLayout*)m_allControl->layout();

    //Adding Option button control, this remains always visible
    m_baseControl.toggleButtonOptionEnable=new QPushButton("Time plot option");
    m_baseControl.toggleButtonOptionEnable->setCheckable(true);
    m_baseControl.toggleButtonOptionEnable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    l->addWidget(m_baseControl.toggleButtonOptionEnable ,1,Qt::AlignHCenter|Qt::AlignTop);
    connect(m_baseControl.toggleButtonOptionEnable,SIGNAL(clicked()),this,SLOT(timeOptionPressed()) );

    this->initBaseControlWidget();
    l->addWidget(m_baseControl.baseControlWidget,1,Qt::AlignHCenter|Qt::AlignTop);
}

void TimePlotWidget::initBaseControlWidget() {
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout for the real control option
    m_baseControl.baseControlWidget=new QWidget();
    QVBoxLayout * lBase=new QVBoxLayout();
    m_baseControl.baseControlWidget->setLayout(lBase) ;
    m_baseControl.baseControlWidget->hide();
    m_baseControl.baseControlWidget->setFont(f);

//    //Adding the ZMP button control
    m_baseControl.toggleButtonZMP = new QPushButton( "Pan");
    m_baseControl.toggleButtonZMP->setCheckable(true);
    lBase->addWidget( m_baseControl.toggleButtonZMP,1,Qt::AlignHCenter|Qt::AlignTop);
    connect(m_baseControl.toggleButtonZMP,SIGNAL(pressed()),this,SLOT(zoomPanButtonPressed()));

    //set Sample rate
    m_baseControl.sliderSR = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_baseControl.sliderSR->setScale(24000,96000,12000);
    m_baseControl.sliderSR->setValue(m_SR);
    m_baseControl.sliderSR->setName("SR Generation");
    m_baseControl.sliderSR->setMeasureUnit("Hz");
    m_baseControl.sliderSR->setFont(f);
    lBase->addWidget( m_baseControl.sliderSR,1,Qt::AlignHCenter|Qt::AlignTop);
    connect(m_baseControl.sliderSR,SIGNAL(valueChanged(double)),this,SLOT(setSampleRate(double)));

    //set duration
    m_baseControl.sliderDuration = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_baseControl.sliderDuration->setScale(0,TIMEDATA_DEFAULT_MAX_TIME,TIMEDATA_DEFAULT_TIMESTEP);
    m_baseControl.sliderDuration->setValue(m_duration);
    m_baseControl.sliderDuration->setName("Duration Generated file");
    m_baseControl.sliderDuration->setMeasureUnit("Sec.");
    m_baseControl.sliderDuration->setFont(f);
    lBase->addWidget( m_baseControl.sliderDuration,1, Qt::AlignHCenter|Qt::AlignTop);
    connect(m_baseControl.sliderDuration,SIGNAL(valueChanged(double)),this,SLOT(setDuration(double)));

    this->timeOptionPressed();
}

void TimePlotWidget::timeOptionPressed() {
    Q_ASSERT(m_baseControl.baseControlWidget!=NULL);
    if (m_baseControl.toggleButtonOptionEnable->isChecked()) {
        m_baseControl.baseControlWidget->show();
    } else {
        m_baseControl.baseControlWidget->hide();
    }
}

void TimePlotWidget::zoomPanButtonPressed() {
    if (m_baseControl.toggleButtonZMP->isChecked()) {
        m_baseControl.toggleButtonZMP->setText("Pan");
        m_baseControl.m_zmp->enablePanner();
    } else {
        m_baseControl.toggleButtonZMP->setText("Zoom");
        m_baseControl.m_zmp->enableZoomer();
    }
}
