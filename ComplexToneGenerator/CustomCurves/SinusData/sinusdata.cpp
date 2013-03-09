#include "sinusdata.h"


//---------- PARAMETERS ----------
SinusParams::SinusParams() :
    m_amplitude(SINUSDATA_DEFAULT_AMPLITUDE),
    m_frequency(SINUSDATA_DEFAULT_FREQUENCY),
    m_initPhase(SINUSDATA_DEFAULT_INITPHASE)
{}

SinusParams::SinusParams(qreal amplitude, qreal frequency, qreal initPhase):
    m_amplitude(amplitude),
    m_frequency(frequency),
    m_initPhase(initPhase)
{}

//---------- UI ----------
SinusUI::SinusUI(QWidget *widget ) :
    QWidget(widget)
{
    this->initControlWidget();
}

void SinusUI::initControlWidget() {
   // QWidget * _widget=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    QHBoxLayout * l=new QHBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    this->setLayout(l);
    this->setFont(f);

    //set frequency
    m_sinusDataControl.sliderFrequency= new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Logarithmic) ;
    m_sinusDataControl.sliderFrequency->setScale(10,22000,0.001);
    m_sinusDataControl.sliderFrequency->setName("Freq.");
    m_sinusDataControl.sliderFrequency->setMeasureUnit("Sec.");
    m_sinusDataControl.sliderFrequency->setFont(f);
    connect(m_sinusDataControl.sliderFrequency,SIGNAL(valueChanged(qreal)),this,SIGNAL(frequencyUIChanged(qreal)));

    //set amplitude
    m_sinusDataControl.sliderAmplitude=new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Linear) ;
    m_sinusDataControl.sliderAmplitude->setScale(0,1.0,0.01);
    m_sinusDataControl.sliderAmplitude->setName("Amplitude");
    m_sinusDataControl.sliderAmplitude->setMeasureUnit("0-1");
    m_sinusDataControl.sliderAmplitude->setFont(f);
    connect(m_sinusDataControl.sliderAmplitude,SIGNAL(valueChanged(qreal)),this,SIGNAL(amplitudeUIChanged(qreal)));


    //set init phase
    m_sinusDataControl.sliderInitPhase=new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Linear) ;
    m_sinusDataControl.sliderInitPhase->setScale(-180,180,1);
    m_sinusDataControl.sliderInitPhase->setName("Phase");
    m_sinusDataControl.sliderInitPhase->setMeasureUnit("deg.");
    m_sinusDataControl.sliderInitPhase->setFont(f);
    connect(m_sinusDataControl.sliderInitPhase,SIGNAL(valueChanged(qreal)),this,SIGNAL(initPhaseUIChanged(qreal)));

    //Lay out all the control);
    l->addWidget(m_sinusDataControl.sliderFrequency,1,Qt::AlignLeft);
    l->addWidget(m_sinusDataControl.sliderAmplitude,1,Qt::AlignLeft);
    l->addWidget(m_sinusDataControl.sliderInitPhase,1,Qt::AlignLeft);

}

void SinusUI::amplitudeUIUpdate(qreal amplitude) {
    if (amplitude!=m_sinusDataControl.sliderAmplitude->value())
        m_sinusDataControl.sliderAmplitude->setValue(amplitude);
}

void SinusUI::frequencyUIUpdate(qreal frequency) {
    if (frequency!=m_sinusDataControl.sliderFrequency->value())
        m_sinusDataControl.sliderFrequency->setValue(frequency);
}

void SinusUI::initPhaseUIUpdate(qreal initphase) {
    if (initphase!=m_sinusDataControl.sliderInitPhase->value())
        m_sinusDataControl.sliderInitPhase->setValue(initphase);
}

