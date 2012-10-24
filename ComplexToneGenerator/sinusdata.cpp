#include "sinusdata.h"



SinusData::SinusData(double duration, double SRGen) :
    GenericTimeData(duration,SRGen)
{
    m_initPhase=SINUSDATA_DEFAULT_INITPHASE;
    m_frequency=SINUSDATA_DEFAULT_FREQUENCY;
    m_amplitude=SINUSDATA_DEFAULT_AMPLITUDE;

    //Init a figure with the same duration of the base class
    this->initControl();
    this->setExtendedControl(m_widgetControl);
    //connect(this,SIGNAL(maxDurationUpdate(double)),this,SLOT(maxDurationChanged(double)));
    this->updateData();
}

SinusData::~SinusData() {

}

SinusData::SinusData( double duration, double SRGen,double amplitude, double frequency, double initPhase) :
    GenericTimeData(duration,SRGen)
{
    m_initPhase=initPhase;
    m_frequency=frequency;
    m_amplitude=amplitude;

    this->initControl();
    this->setExtendedControl(m_widgetControl);
    this->updateData();
}

void SinusData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz") << " - SinusData::recalc() ---------------- " << this->name();
    double * m_sinus=this->getSignalData();
    double * t=this->getTimeData();
    double phase=SinusData::deg2rad(this->initPhase());

    int n_dw=(this->minStartTime())*this->sampleRate();
    qDebug() << "initPhase=" << this->initPhase() << " frequency=" << this->frequency() << " amplitude=" << this->amplitude() ;
    qDebug() << " m_min_t0=" << this->minStartTime() << " n_dw=" << n_dw << " NSample=" << this->sampleNumber();
    Q_ASSERT( n_dw >=0);
    Q_ASSERT(n_dw <=this->sampleNumber());

    int n_up=(this->duration())*this->sampleRate();
    qDebug() << "m_max_Duration=" << this->maxDuration() <<" m_duration=" << this->duration()  << " n_up=" << n_up << " NSample=" << this->sampleNumber();
    Q_ASSERT( n_up>=0 );
    Q_ASSERT(n_up<=this->sampleNumber());
    Q_ASSERT(n_dw<=n_up);

    for (int n=n_dw; n < n_up; n++) {
        m_sinus[n]=this->amplitude()*sin(2*M_PI*this->frequency()*t[n]+phase);
    }
}

void SinusData::setAmplitudeFrequencyAndPhase(double amplitude,double frequency,double initPhase) {
    m_amplitude=amplitude;
    m_frequency=frequency;
    m_initPhase=initPhase;

    //updating UI
    bool sigStatus=m_dataControl.sliderFrequency->blockSignals(true);
    m_dataControl.sliderFrequency->setValue(m_frequency);
    m_dataControl.sliderFrequency->blockSignals(sigStatus);

    sigStatus=m_dataControl.sliderAmplitude->blockSignals(true);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->blockSignals(sigStatus);

    sigStatus=m_dataControl.sliderInitPhase->blockSignals(true);
    m_dataControl.sliderInitPhase->setValue(m_initPhase);
    m_dataControl.sliderInitPhase->blockSignals(sigStatus);

    this->updateData();
}

void SinusData::setAmplitude(double amplitude) {
    m_amplitude=amplitude;
    //updating UI
    bool sigStatus=m_dataControl.sliderAmplitude->blockSignals(true);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->blockSignals(sigStatus);
    this->updateData();
}

void SinusData::setFrequency(double frequency) {
    m_frequency=frequency;
    //updating UI
    bool sigStatus=m_dataControl.sliderFrequency->blockSignals(true);
    m_dataControl.sliderFrequency->setValue(m_frequency);
    m_dataControl.sliderFrequency->blockSignals(sigStatus);
    this->updateData();
}

void SinusData::setInitPhase(double initPhase) {
    m_initPhase=initPhase;
    //updating UI
    bool sigStatus=m_dataControl.sliderInitPhase->blockSignals(true);
    m_dataControl.sliderInitPhase->setValue(m_initPhase);
    m_dataControl.sliderInitPhase->blockSignals(sigStatus);
    this->updateData();
}


void SinusData::initControl() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    m_widgetControl=new QFrame();
    QVBoxLayout * l=new QVBoxLayout();
    m_widgetControl->setLayout(l) ;
    m_widgetControl->hide();
    m_widgetControl->setFont(f);

    //set frequency
    m_dataControl.sliderFrequency= new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Logarithmic) ;
    m_dataControl.sliderFrequency->setScale(10,22000,0.01);
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

    //Lay out all the control);
    l->addWidget(m_dataControl.sliderFrequency,1,Qt::AlignLeft);
    l->addWidget(m_dataControl.sliderAmplitude,1,Qt::AlignLeft);
    l->addWidget(m_dataControl.sliderInitPhase,1,Qt::AlignLeft);

}




//------------PartialSinusData init here --------------


PartialSinusData::PartialSinusData(double duration, double SRGen) :
    PartialTimeData(duration,SRGen)
{
    m_initPhase=SINUSDATA_DEFAULT_INITPHASE;
    m_frequency=SINUSDATA_DEFAULT_FREQUENCY;
    m_amplitude=SINUSDATA_DEFAULT_AMPLITUDE;

    //Init a figure with the same duration of the base class  
    this->initControl();
    this->setExtendedControl(m_widgetControl);
    this->setDuration(duration);
    //connect(this,SIGNAL(maxDurationUpdate(double)),this,SLOT(maxDurationChanged(double)));
    this->updateData();
}

PartialSinusData::~PartialSinusData() {

}

PartialSinusData::PartialSinusData( double duration, double SRGen,double amplitude, double frequency, double initPhase) :
    PartialTimeData(duration,SRGen)
{
    m_initPhase=initPhase;
    m_frequency=frequency;
    m_amplitude=amplitude;

    this->initControl();
    this->setExtendedControl(m_widgetControl);
    this->setDuration(duration);
    connect(this,SIGNAL(maxDurationUpdate(double)),this,SLOT(signalLimitsChanged()));
    this->updateData();
}

void PartialSinusData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz") << " - PartialSinusData::recalc() ---------------- " << this->name();
    double * m_sinus=this->getSignalData();
    double * t=this->getTimeData();
    double phase=PartialSinusData::deg2rad(this->initPhase());

    int n_dw=(this->startTime()-this->minStartTime())*this->sampleRate();
    qDebug() << "initPhase=" << this->initPhase() << " frequency=" << this->frequency() << " amplitude=" << this->amplitude() ;
    qDebug() << "m_t0=" << this->startTime() << " m_min_t0=" << this->minStartTime() << " n_dw=" << n_dw << " NSample=" << this->sampleNumber();
    Q_ASSERT( n_dw >=0);
    Q_ASSERT(n_dw <=this->sampleNumber());

    int n_up=(this->startTime()+this->duration())*this->sampleRate();
    qDebug() << "m_max_Duration=" << this->maxDuration() <<" m_duration=" << this->duration()  << " n_up=" << n_up << " NSample=" << this->sampleNumber();
    Q_ASSERT( n_up>=0 );
    Q_ASSERT(n_up<=this->sampleNumber());
    Q_ASSERT(n_dw<=n_up);

    for (int n=n_dw; n < n_up; n++) {
        m_sinus[n]=this->amplitude()*sin(2*M_PI*this->frequency()*t[n]+phase);
    }
}

void PartialSinusData::setAmplitudeFrequencyAndPhase(double amplitude,double frequency,double initPhase) {
    m_amplitude=amplitude;
    m_frequency=frequency;
    m_initPhase=initPhase;

    //updating UI
    bool sigStatus=m_dataControl.sliderFrequency->blockSignals(true);
    m_dataControl.sliderFrequency->setValue(m_frequency);
    m_dataControl.sliderFrequency->blockSignals(sigStatus);

    sigStatus=m_dataControl.sliderAmplitude->blockSignals(true);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->blockSignals(sigStatus);

    sigStatus=m_dataControl.sliderInitPhase->blockSignals(true);
    m_dataControl.sliderInitPhase->setValue(m_initPhase);
    m_dataControl.sliderInitPhase->blockSignals(sigStatus);

    this->updateData();
}

void PartialSinusData::setAmplitude(double amplitude) {
    m_amplitude=amplitude;
    //updating UI
    bool sigStatus=m_dataControl.sliderAmplitude->blockSignals(true);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->blockSignals(sigStatus);
    this->updateData();
}

void PartialSinusData::setFrequency(double frequency) {
    m_frequency=frequency;
    //updating UI
    bool sigStatus=m_dataControl.sliderFrequency->blockSignals(true);
    m_dataControl.sliderFrequency->setValue(m_frequency);
    m_dataControl.sliderFrequency->blockSignals(sigStatus);
    this->updateData();
}

void PartialSinusData::setInitPhase(double initPhase) {
    m_initPhase=initPhase;
    //updating UI
    bool sigStatus=m_dataControl.sliderInitPhase->blockSignals(true);
    m_dataControl.sliderInitPhase->setValue(m_initPhase);
    m_dataControl.sliderInitPhase->blockSignals(sigStatus);
    this->updateData();
}


void PartialSinusData::initControl() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    m_widgetControl=new QFrame();
    QVBoxLayout * l=new QVBoxLayout();
    m_widgetControl->setLayout(l) ;
    m_widgetControl->hide();
    m_widgetControl->setFont(f);

    //set frequency
    m_dataControl.sliderFrequency= new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Logarithmic) ;
    m_dataControl.sliderFrequency->setScale(10,22000,0.01);
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

    //Lay out all the control);
    l->addWidget(m_dataControl.sliderFrequency,1,Qt::AlignLeft);
    l->addWidget(m_dataControl.sliderAmplitude,1,Qt::AlignLeft);
    l->addWidget(m_dataControl.sliderInitPhase,1,Qt::AlignLeft);
    //Control Widget from partialtimedata
    l->addWidget(m_partialDataControl.widgetDuration,1,Qt::AlignLeft);
    l->addWidget(m_partialDataControl.widget_t0,1,Qt::AlignLeft);
}
