#include "constanttimedata.h"

PartialConstantTimeData::PartialConstantTimeData(double duration, double SRGen) :
    PartialTimeData(duration,SRGen)
{
    m_amplitude=CONSTDATA_DEFAULT_AMPLITUDE;
    //Init a figure with the same duration of the base class
    this->initControl();
    this->setExtendedControl(m_widgetControl);
    this->updateData();
}

void PartialConstantTimeData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz") << " - PartialConstantTimeData::recalc() ---------------- " << this->name();
    double * m_const=this->getSignalData();//get the pointer from super class. The base class values are all 0

    int n_dw=(this->startTime()-this->minStartTime())*this->sampleRate();
    qDebug() << "amplitude=" << this->amplitude() << "m_t0=" << this->startTime() << " m_min_t0=" << this->minStartTime() << " n_dw=" << n_dw << " NSample=" << this->sampleNumber();
    Q_ASSERT( n_dw >=0);
    Q_ASSERT(n_dw <=this->sampleNumber());

    int n_up=(this->startTime()+this->duration())*this->sampleRate();
    qDebug() << "m_max_Duration=" << this->maxDuration() <<" m_duration=" << this->duration()  << " n_up=" << n_up << " NSample=" << this->sampleNumber();
    Q_ASSERT( n_up>=0 );
    Q_ASSERT(n_up<=this->sampleNumber());
    Q_ASSERT(n_dw<=n_up);

    for (int n=n_dw; n < n_up; n++) {
        m_const[n]=this->amplitude();//write the wanted values
    }
}

void PartialConstantTimeData::setAmplitude(double amplitude) {
    m_amplitude=amplitude;
    //updating UI
    bool sigStatus=m_dataControl.sliderAmplitude->blockSignals(true);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->blockSignals(sigStatus);
    this->updateData();
}

void PartialConstantTimeData::initControl() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    m_widgetControl=new QFrame();
    QVBoxLayout * l=new QVBoxLayout();
    m_widgetControl->setLayout(l) ;
    m_widgetControl->hide();
    m_widgetControl->setFont(f);

    //set amplitude
    m_dataControl.sliderAmplitude=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_dataControl.sliderAmplitude->setScale(-1.0,1.0,0.01);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->setName("Amplitude");
    m_dataControl.sliderAmplitude->setMeasureUnit("-1 - +1");
    m_dataControl.sliderAmplitude->setFont(f);
    connect(m_dataControl.sliderAmplitude,SIGNAL(valueChanged(double)),this,SLOT(setAmplitude(double)));

    //Lay out all the control);
    l->addWidget(m_dataControl.sliderAmplitude,1,Qt::AlignLeft);
    //Control Widget from partialtimedata
    l->addWidget(m_partialDataControl.widgetDuration,1,Qt::AlignLeft);
    l->addWidget(m_partialDataControl.widget_t0,1,Qt::AlignLeft);
}

//-----------------------------
//Here Start ConstantTimeDate

ConstantTimeData::ConstantTimeData(double duration, double SRGen) :
    GenericTimeData(duration,SRGen)
{
    m_amplitude=CONSTDATA_DEFAULT_AMPLITUDE;
    //Init a figure with the same duration of the base class
    this->initControl();
    this->setExtendedControl(m_widgetControl);
    this->updateData();
}

void ConstantTimeData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz") << " - ConstantTimeData::recalc() ---------------- " << this->name();
    double * m_const=this->getSignalData();//get the pointer from super class. The base class values are all 0

    int n_dw=(this->minStartTime())*this->sampleRate();
    Q_ASSERT( n_dw >=0);
    Q_ASSERT(n_dw <=this->sampleNumber());

    int n_up=(this->minStartTime()+this->duration())*this->sampleRate();
    qDebug() << "amplitude= " << this->amplitude() << "m_max_Duration=" << this->maxDuration() <<" m_duration=" << this->duration()  << " n_up=" << n_up << " NSample=" << this->sampleNumber();
    Q_ASSERT( n_up>=0 );
    Q_ASSERT(n_up<=this->sampleNumber());
    Q_ASSERT(n_dw<=n_up);

    for (int n=n_dw; n < n_up; n++) {
        m_const[n]=this->amplitude();//write the wanted values
    }
}

void ConstantTimeData::setAmplitude(double amplitude) {
    m_amplitude=amplitude;
    //updating UI
    bool sigStatus=m_dataControl.sliderAmplitude->blockSignals(true);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->blockSignals(sigStatus);
    this->updateData();
}

void ConstantTimeData::initControl() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    m_widgetControl=new QFrame();
    QVBoxLayout * l=new QVBoxLayout();
    m_widgetControl->setLayout(l) ;
    m_widgetControl->hide();
    m_widgetControl->setFont(f);

    //set amplitude
    m_dataControl.sliderAmplitude=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_dataControl.sliderAmplitude->setScale(-1.0,1.0,0.01);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->setName("Amplitude");
    m_dataControl.sliderAmplitude->setMeasureUnit("-1 - +1");
    m_dataControl.sliderAmplitude->setFont(f);
    connect(m_dataControl.sliderAmplitude,SIGNAL(valueChanged(double)),this,SLOT(setAmplitude(double)));

    //Lay out all the control);
    l->addWidget(m_dataControl.sliderAmplitude,1,Qt::AlignLeft);
}
