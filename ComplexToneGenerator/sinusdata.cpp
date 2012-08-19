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
    double * m_sinus=new double[this->sampleNumber()];
    double * t=this->getTimeData();
    double phase=SinusData::deg2rad(this->initPhase());

    for (int n=0; n < this->sampleNumber(); n++) {
        m_sinus[n]=this->amplitude()*sin(2*M_PI*this->frequency()*t[n]+phase);
    }
   this->setSignalData(m_sinus,this->sampleNumber());
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
    m_dataControl.sliderFrequency= new ScaledSliderWidget(NULL, Qt::Horizontal,ScaledSlider::Logarithmic) ;
    m_dataControl.sliderFrequency->setScale(10,22000,1);
    m_dataControl.sliderFrequency->setValue(m_frequency);
    m_dataControl.sliderFrequency->setName("Freq.");
    m_dataControl.sliderFrequency->setMeasureUnit("Sec.");
    m_dataControl.sliderFrequency->setFont(f);
    m_dataControl.sliderFrequency->setMaximumWidth( CONTROL_WIDTH);
    m_dataControl.sliderFrequency->setMinimumWidth( CONTROL_WIDTH/2);
    m_dataControl.sliderFrequency->setSizePolicy(QSizePolicy::Maximum ,QSizePolicy::Maximum);
    m_dataControl.sliderFrequency->setSizePolicy(QSizePolicy::Maximum ,QSizePolicy::Maximum);
    connect(m_dataControl.sliderFrequency,SIGNAL(valueChanged(double)),this,SLOT(setFrequency(double)));

    //set amplitude
    m_dataControl.sliderAmplitude=new ScaledSliderWidget(NULL, Qt::Horizontal,ScaledSlider::Linear) ;
    m_dataControl.sliderAmplitude->setScale(0,1.0,0.01);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->setName("Amplitude");
    m_dataControl.sliderAmplitude->setMeasureUnit("0-1");
    m_dataControl.sliderAmplitude->setFont(f);
    m_dataControl.sliderAmplitude->setMaximumWidth( CONTROL_WIDTH);
    m_dataControl.sliderAmplitude->setMinimumWidth( CONTROL_WIDTH/2);
    m_dataControl.sliderAmplitude->setSizePolicy(QSizePolicy::Maximum ,QSizePolicy::Maximum);
    connect(m_dataControl.sliderAmplitude,SIGNAL(valueChanged(double)),this,SLOT(setAmplitude(double)));

    //set init phase
    m_dataControl.sliderInitPhase=new ScaledSliderWidget(NULL, Qt::Horizontal,ScaledSlider::Linear) ;
    m_dataControl.sliderInitPhase->setScale(-180,180,1);
    m_dataControl.sliderInitPhase->setValue(m_initPhase);
    m_dataControl.sliderInitPhase->setName("Phase");
    m_dataControl.sliderInitPhase->setMeasureUnit("deg.");
    m_dataControl.sliderInitPhase->setFont(f);
    m_dataControl.sliderInitPhase->setMaximumWidth( CONTROL_WIDTH);
    m_dataControl.sliderInitPhase->setMinimumWidth( CONTROL_WIDTH/2);
    m_dataControl.sliderInitPhase->setSizePolicy(QSizePolicy::Maximum ,QSizePolicy::Maximum);
    connect(m_dataControl.sliderInitPhase,SIGNAL(valueChanged(double)),this,SLOT(setInitPhase(double)));

    //Lay out all the control);
    l->addWidget(m_dataControl.sliderFrequency,1,Qt::AlignCenter);
    l->addWidget(m_dataControl.sliderAmplitude,1,Qt::AlignCenter);
    l->addWidget(m_dataControl.sliderInitPhase,1,Qt::AlignCenter);
}
