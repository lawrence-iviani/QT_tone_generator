#include "sinusdata.h"

SinusData::SinusData() :
    GenericTimeData()
{
    m_initPhase=0;
    m_frequency=5;
    m_amplitude=1.0;
    this->initControl();
    this->setExtendedControl(m_widgetControl);
    this->updateData();
}

SinusData::SinusData(double t0, double duration, double SRGen) :
    GenericTimeData(t0,duration,SRGen) {
    m_initPhase=0;
    m_frequency=5;
    m_amplitude=1.0;
    this->initControl();
    this->setExtendedControl(m_widgetControl);
    this->updateData();
}

SinusData::~SinusData() {

}

SinusData::SinusData(double frequency, double amplitude, double t0, double duration, double SRGen) :
    GenericTimeData(t0,duration,SRGen)
{
    m_initPhase=0;
    m_frequency=frequency;
    m_amplitude=amplitude;
    this->initControl();
    this->updateData();
}

void SinusData::recalc() {
    //double * m_sinus=new double[this->sampleNumber()];
    double * m_sinus=this->getSignalData();
    double * t=this->getTimeData();
    double phase=SinusData::deg2rad(this->initPhase());
    int n_dw=m_t0*this->sampleRateGeneration();
    int n_up=(m_t0+m_duration)*this->sampleRateGeneration();
    for (int n=n_dw; n < n_up; n++) {
        m_sinus[n]=this->amplitude()*sin(2*M_PI*this->frequency()*t[n]+phase);
    }
   //this->setSignalData(m_sinus,this->sampleNumber());
}

void SinusData::setAmplitudeFrequencyAndPhase(double amplitude,double frequency,double initPhase) {
    m_amplitude=amplitude;
    m_frequency=frequency;
    m_initPhase=initPhase;
    this->updateData();
}

void SinusData::setAmplitude(double amplitude) {
    m_amplitude=amplitude;
    this->updateData();
}

void SinusData::setFrequency(double frequency) {
    m_frequency=frequency;
    this->updateData();
}

void SinusData::setInitPhase(double initPhase) {
    m_initPhase=initPhase;
    this->updateData();
}

void SinusData::setDuration(double duration) {
    if (duration < 0) {
        m_duration=0;
    } else if ((m_t0 + duration) > this->maxEndTime() ) {
        m_duration=this->maxEndTime()-m_t0;
    } else {
        m_duration=this->maxDuration();
    }
     this->updateData();
}

void SinusData::setStartTime(double t0) {
    if (t0 < this->minStartTime()) {
        m_t0=this->minStartTime();
    } else if (t0>this->maxEndTime()) {
        m_t0=this->maxEndTime();
    } else {
        m_t0=t0;
    }
    this->setDuration(m_duration);
}

void SinusData::initControl() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(BASE_SIZE);

    //Widget container and layout
    m_widgetControl=new QWidget();
    QVBoxLayout * l=new QVBoxLayout();
    m_widgetControl->setLayout(l) ;
    m_widgetControl->hide();
    m_widgetControl->setFont(f);

    //set frequency
    m_dataControl.sliderFrequency= new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Logarithmic) ;
    m_dataControl.sliderFrequency->setScale(10,22000,1);
    m_dataControl.sliderFrequency->setValue(m_frequency);
    m_dataControl.sliderFrequency->setName("Freq.");
    m_dataControl.sliderFrequency->setMeasureUnit("Sec.");
    m_dataControl.sliderFrequency->setFont(f);
    connect(m_dataControl.sliderFrequency,SIGNAL(valueChanged(double)),this,SLOT(setFrequency(double)));

    //set amplitude
    m_dataControl.sliderAmplitude=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_dataControl.sliderAmplitude->setScale(0,1.0,0.01);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->setName("Amplitude");
    m_dataControl.sliderAmplitude->setMeasureUnit("0-1");
    m_dataControl.sliderAmplitude->setFont(f);
    connect(m_dataControl.sliderAmplitude,SIGNAL(valueChanged(double)),this,SLOT(setAmplitude(double)));

    //set init phase
    m_dataControl.sliderInitPhase=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_dataControl.sliderInitPhase->setScale(-180,180,1);
    m_dataControl.sliderInitPhase->setValue(m_initPhase);
    m_dataControl.sliderInitPhase->setName("Phase");
    m_dataControl.sliderInitPhase->setMeasureUnit("deg.");
    m_dataControl.sliderInitPhase->setFont(f);
    connect(m_dataControl.sliderInitPhase,SIGNAL(valueChanged(double)),this,SLOT(setInitPhase(double)));

    //set duration
    m_dataControl.sliderDuration = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_dataControl.sliderDuration->setScale(0,10,0.1);//TODO: this needs to be set from an external part, ie the base class
    m_dataControl.sliderDuration->setValue(m_duration);
    m_dataControl.sliderDuration->setName("Duration");
    m_dataControl.sliderDuration->setMeasureUnit("Sec.");
    m_dataControl.sliderDuration->setFont(f);
    connect(m_dataControl.sliderDuration,SIGNAL(valueChanged(double)),this,SLOT(setDuration(double)));

    //set t0
    m_dataControl.slider_t0 = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_dataControl.slider_t0->setScale(-10,+10,0.1);
    m_dataControl.slider_t0->setValue(m_t0);
    m_dataControl.slider_t0->setName("Start Time");
    m_dataControl.slider_t0->setMeasureUnit("Sec.");
    m_dataControl.slider_t0->setFont(f);
    connect(m_dataControl.slider_t0,SIGNAL(valueChanged(double)),this,SLOT(setStartTime(double)));

    //Lay out all the control);
    l->addWidget(m_dataControl.sliderFrequency,1);//,Qt::AlignCenter);
    l->addWidget(m_dataControl.sliderAmplitude,1);//,Qt::AlignCenter);
    l->addWidget(m_dataControl.sliderInitPhase,1);//,Qt::AlignCenter);
    l->addWidget(m_dataControl.sliderDuration,1);
    l->addWidget(m_dataControl.slider_t0,1);
}
