#include "sinusdata.h"



SinusData::SinusData(qreal duration, qreal SRGen) :
    GenericTimeData(duration,SRGen)
{
    m_initPhase=SINUSDATA_DEFAULT_INITPHASE;
    m_frequency=SINUSDATA_DEFAULT_FREQUENCY;
    m_amplitude=SINUSDATA_DEFAULT_AMPLITUDE;

    //Init a figure with the same duration of the base class
    this->initControl();
    //this->getControlWidget()->setExtendedControl(m_widgetControl);
    //connect(this,SIGNAL(maxDurationUpdate(qreal)),this,SLOT(maxDurationChanged(qreal)));
    this->updateData();
}

SinusData::~SinusData() {

}

SinusData::SinusData( qreal duration, qreal SRGen,qreal amplitude, qreal frequency, qreal initPhase) :
    GenericTimeData(duration,SRGen)
{
    m_initPhase=initPhase;
    m_frequency=frequency;
    m_amplitude=amplitude;

    this->initControl();
    //this->getControlWidget()->setExtendedControl(m_widgetControl);
    this->updateData();
}

void SinusData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz") << " - SinusData::recalc() ---------------- " << this->name();
    const qreal * t=this->getTimeData();
    qreal phase=PartialSinusData::deg2rad(this->initPhase());

    qint64 n_dw=this->lowestSampleIndexForModification();
    qint64 n_up=this->highestSampleIndexForModification();
    qDebug() << "SinusData::recalc() m_max_Duration=" << this->maxDuration() <<" m_duration=" << this->duration()  << " n_dw=" << n_dw << " n_up=" << n_up << " nsample=" << this->sampleNumber();

    for (qint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(this->insertSignalValue(n,this->amplitude()*sin(2*M_PI*this->frequency()*t[n]+phase)));
    }
}

void SinusData::setAmplitudeFrequencyAndPhase(qreal amplitude,qreal frequency,qreal initPhase) {
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

void SinusData::setAmplitude(qreal amplitude) {
    m_amplitude=amplitude;
    //updating UI
    bool sigStatus=m_dataControl.sliderAmplitude->blockSignals(true);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->blockSignals(sigStatus);
    this->updateData();
}

void SinusData::setFrequency(qreal frequency) {
    m_frequency=frequency;
    //updating UI
    bool sigStatus=m_dataControl.sliderFrequency->blockSignals(true);
    m_dataControl.sliderFrequency->setValue(m_frequency);
    m_dataControl.sliderFrequency->blockSignals(sigStatus);
    this->updateData();
}

void SinusData::setInitPhase(qreal initPhase) {
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
    connect(m_dataControl.sliderFrequency,SIGNAL(valueChanged(qreal)),this,SLOT(setFrequency(qreal)));

    //set amplitude
    m_dataControl.sliderAmplitude=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_dataControl.sliderAmplitude->setScale(0,1.0,0.01);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->setName("Amplitude");
    m_dataControl.sliderAmplitude->setMeasureUnit("0-1");
    m_dataControl.sliderAmplitude->setFont(f);
    connect(m_dataControl.sliderAmplitude,SIGNAL(valueChanged(qreal)),this,SLOT(setAmplitude(qreal)));

    //set init phase
    m_dataControl.sliderInitPhase=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_dataControl.sliderInitPhase->setScale(-180,180,1);
    m_dataControl.sliderInitPhase->setValue(m_initPhase);
    m_dataControl.sliderInitPhase->setName("Phase");
    m_dataControl.sliderInitPhase->setMeasureUnit("deg.");
    m_dataControl.sliderInitPhase->setFont(f);
    connect(m_dataControl.sliderInitPhase,SIGNAL(valueChanged(qreal)),this,SLOT(setInitPhase(qreal)));

    //Lay out all the control);
    l->addWidget(m_dataControl.sliderFrequency,1,Qt::AlignLeft);
    l->addWidget(m_dataControl.sliderAmplitude,1,Qt::AlignLeft);
    l->addWidget(m_dataControl.sliderInitPhase,1,Qt::AlignLeft);

}




//------------PartialSinusData init here --------------


PartialSinusData::PartialSinusData(qreal duration, qreal SRGen) :
    PartialTimeData(duration,SRGen)
{
    m_initPhase=SINUSDATA_DEFAULT_INITPHASE;
    m_frequency=SINUSDATA_DEFAULT_FREQUENCY;
    m_amplitude=SINUSDATA_DEFAULT_AMPLITUDE;

    //Init a figure with the same duration of the base class  
    this->initControl();
    //this->getControlWidget()->setExtendedControl(m_widgetControl);
    this->setDuration(duration);
    //connect(this,SIGNAL(maxDurationUpdate(qreal)),this,SLOT(maxDurationChanged(qreal)));
    this->updateData();
}

PartialSinusData::~PartialSinusData() {

}

PartialSinusData::PartialSinusData( qreal duration, qreal SRGen,qreal amplitude, qreal frequency, qreal initPhase) :
    PartialTimeData(duration,SRGen)
{
    m_initPhase=initPhase;
    m_frequency=frequency;
    m_amplitude=amplitude;

    this->initControl();
    //this->getControlWidget()->setExtendedControl(m_widgetControl);
    this->setDuration(duration);
    connect(this,SIGNAL(maxDurationUpdate(qreal)),this,SLOT(signalLimitsChanged()));
    this->updateData();
}

void PartialSinusData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz") << " - PartialSinusData::recalc() ---------------- " << this->name();
    const qreal * t=this->getTimeData();
    qreal phase=PartialSinusData::deg2rad(this->initPhase());

    qint64 n_dw=this->lowestSampleIndexForModification();
    qint64 n_up=this->highestSampleIndexForModification();
    qDebug() << "PartialSinusData::recalc() m_max_Duration=" << this->maxDuration() <<" m_duration=" << this->duration()  << " n_dw=" << n_dw << " n_up=" << n_up << " nsample=" << this->sampleNumber();

    for (qint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(this->insertSignalValue(n,this->amplitude()*sin(2*M_PI*this->frequency()*t[n]+phase)));
    }
}

void PartialSinusData::setAmplitudeFrequencyAndPhase(qreal amplitude,qreal frequency,qreal initPhase) {
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

void PartialSinusData::setAmplitude(qreal amplitude) {
    m_amplitude=amplitude;
    //updating UI
    bool sigStatus=m_dataControl.sliderAmplitude->blockSignals(true);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->blockSignals(sigStatus);
    this->updateData();
}

void PartialSinusData::setFrequency(qreal frequency) {
    m_frequency=frequency;
    //updating UI
    bool sigStatus=m_dataControl.sliderFrequency->blockSignals(true);
    m_dataControl.sliderFrequency->setValue(m_frequency);
    m_dataControl.sliderFrequency->blockSignals(sigStatus);
    this->updateData();
}

void PartialSinusData::setInitPhase(qreal initPhase) {
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
    connect(m_dataControl.sliderFrequency,SIGNAL(valueChanged(qreal)),this,SLOT(setFrequency(qreal)));

    //set amplitude
    m_dataControl.sliderAmplitude=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_dataControl.sliderAmplitude->setScale(0,1.0,0.01);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->setName("Amplitude");
    m_dataControl.sliderAmplitude->setMeasureUnit("0-1");
    m_dataControl.sliderAmplitude->setFont(f);
    connect(m_dataControl.sliderAmplitude,SIGNAL(valueChanged(qreal)),this,SLOT(setAmplitude(qreal)));

    //set init phase
    m_dataControl.sliderInitPhase=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_dataControl.sliderInitPhase->setScale(-180,180,1);
    m_dataControl.sliderInitPhase->setValue(m_initPhase);
    m_dataControl.sliderInitPhase->setName("Phase");
    m_dataControl.sliderInitPhase->setMeasureUnit("deg.");
    m_dataControl.sliderInitPhase->setFont(f);
    connect(m_dataControl.sliderInitPhase,SIGNAL(valueChanged(qreal)),this,SLOT(setInitPhase(qreal)));

    //Lay out all the control);
    l->addWidget(m_dataControl.sliderFrequency,1,Qt::AlignLeft);
    l->addWidget(m_dataControl.sliderAmplitude,1,Qt::AlignLeft);
    l->addWidget(m_dataControl.sliderInitPhase,1,Qt::AlignLeft);
    //Control Widget from partialtimedata
    //l->addWidget(m_partialDataControl.widgetDuration,1,Qt::AlignLeft);
    //l->addWidget(m_partialDataControl.widget_t0,1,Qt::AlignLeft);
}
