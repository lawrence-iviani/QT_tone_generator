#include "sinusdataui.h"

SinusDataUI::SinusDataUI(SinusData *sd, QWidget *widget) :
    CustomCurveUI(widget),
    m_sinusData(sd),
    m_partialSinusData(NULL),
    m_repeatedSinusData(NULL)
{
    initControl();
    m_sinusData->getControlWidget()->addControlFrame(this,"SinusData control");
    connectingControlSignal();
}

SinusDataUI::SinusDataUI(PartialSinusData *psd, QWidget *widget) :
    CustomCurveUI(widget),
    m_sinusData(NULL),
    m_partialSinusData(psd),
    m_repeatedSinusData(NULL)
{
    initControl();
    m_partialSinusData->getControlWidget()->addControlFrame(this,"PartialSinusData control");
    connectingControlSignal();
}

SinusDataUI::SinusDataUI(RepeatedSinusData *rsd, QWidget *widget) :
    CustomCurveUI(widget),
    m_sinusData(NULL),
    m_partialSinusData(NULL),
    m_repeatedSinusData(rsd)
{
    initControl();
    m_repeatedSinusData->getControlWidget()->addControlFrame(this,"RepeatedSinusData control");
    connectingControlSignal();
}

void SinusDataUI::updateControlUI() {
    qDebug() << "SinusDataUI::updateControlUI called";

    if (m_partialSinusData!=NULL) {
        Q_ASSERT(m_sinusData==NULL && m_repeatedSinusData==NULL);
        setSliderValue(m_sinusDataControl.sliderFrequency, m_partialSinusData->frequency());
        setSliderValue(m_sinusDataControl.sliderAmplitude,m_partialSinusData->amplitude());
        setSliderValue(m_sinusDataControl.sliderInitPhase,m_partialSinusData->initPhase());
    }
    if (m_sinusData!=NULL) {
        Q_ASSERT(m_partialSinusData==NULL && m_repeatedSinusData==NULL);
        setSliderValue(m_sinusDataControl.sliderFrequency, m_sinusData->frequency());
        setSliderValue(m_sinusDataControl.sliderAmplitude, m_sinusData->amplitude());
        setSliderValue(m_sinusDataControl.sliderInitPhase, m_sinusData->initPhase());
    }
    if (m_repeatedSinusData!=NULL) {
        Q_ASSERT(m_partialSinusData==NULL && m_sinusData==NULL);
        setSliderValue(m_sinusDataControl.sliderFrequency, m_repeatedSinusData->frequency());
        setSliderValue(m_sinusDataControl.sliderAmplitude, m_repeatedSinusData->amplitude());
        setSliderValue(m_sinusDataControl.sliderInitPhase, m_repeatedSinusData->initPhase());
    }

}

void SinusDataUI::connectingControlSignal() {
    if (m_partialSinusData!=NULL) {
        Q_ASSERT(m_sinusData==NULL && m_repeatedSinusData==NULL);
        connect(m_sinusDataControl.sliderFrequency,SIGNAL(valueChanged(qreal)),m_partialSinusData,SLOT(setFrequency(qreal)));
        connect(m_sinusDataControl.sliderAmplitude,SIGNAL(valueChanged(qreal)),m_partialSinusData,SLOT(setAmplitude(qreal)));
        connect(m_sinusDataControl.sliderInitPhase,SIGNAL(valueChanged(qreal)),m_partialSinusData,SLOT(setInitPhase(qreal)));
    }
    if (m_sinusData!=NULL) {
        Q_ASSERT(m_partialSinusData==NULL && m_repeatedSinusData==NULL);
        connect(m_sinusDataControl.sliderFrequency,SIGNAL(valueChanged(qreal)),m_sinusData,SLOT(setFrequency(qreal)));
        connect(m_sinusDataControl.sliderAmplitude,SIGNAL(valueChanged(qreal)),m_sinusData,SLOT(setAmplitude(qreal)));
        connect(m_sinusDataControl.sliderInitPhase,SIGNAL(valueChanged(qreal)),m_sinusData,SLOT(setInitPhase(qreal)));
    }
    if (m_repeatedSinusData!=NULL) {
        Q_ASSERT(m_partialSinusData==NULL && m_sinusData==NULL);
        connect(m_sinusDataControl.sliderFrequency,SIGNAL(valueChanged(qreal)),m_repeatedSinusData,SLOT(setFrequency(qreal)));
        connect(m_sinusDataControl.sliderAmplitude,SIGNAL(valueChanged(qreal)),m_repeatedSinusData,SLOT(setAmplitude(qreal)));
        connect(m_sinusDataControl.sliderInitPhase,SIGNAL(valueChanged(qreal)),m_repeatedSinusData,SLOT(setInitPhase(qreal)));
    }
}

void SinusDataUI::initControl() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    QHBoxLayout * l=new QHBoxLayout();
    this->setLayout(l);
    this->setFont(f);

    //set frequency
    m_sinusDataControl.sliderFrequency= new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Logarithmic) ;
    m_sinusDataControl.sliderFrequency->setScale(10,22000,0.001);
    m_sinusDataControl.sliderFrequency->setName("Freq.");
    m_sinusDataControl.sliderFrequency->setMeasureUnit("Sec.");
    m_sinusDataControl.sliderFrequency->setFont(f);

    //set amplitude
    m_sinusDataControl.sliderAmplitude=new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Linear) ;
    m_sinusDataControl.sliderAmplitude->setScale(0,1.0,0.01);
    m_sinusDataControl.sliderAmplitude->setName("Amplitude");
    m_sinusDataControl.sliderAmplitude->setMeasureUnit("0-1");
    m_sinusDataControl.sliderAmplitude->setFont(f);

    //set init phase
    m_sinusDataControl.sliderInitPhase=new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Linear) ;
    m_sinusDataControl.sliderInitPhase->setScale(-180,180,1);
    m_sinusDataControl.sliderInitPhase->setName("Phase");
    m_sinusDataControl.sliderInitPhase->setMeasureUnit("deg.");
    m_sinusDataControl.sliderInitPhase->setFont(f);

    //Lay out all the control);
    l->addWidget(m_sinusDataControl.sliderFrequency,1,Qt::AlignLeft);
    l->addWidget(m_sinusDataControl.sliderAmplitude,1,Qt::AlignLeft);
    l->addWidget(m_sinusDataControl.sliderInitPhase,1,Qt::AlignLeft);

    //Setting values
    updateControlUI();
}

void SinusDataUI::setSliderValue(ScaledSliderWidget * slider, qreal val) {
    if (slider==NULL ) return;
//    qDebug() << "SinusDataUI::setSliderValue slider@" <<slider << " value=" <<val;
    bool sig=slider->blockSignals(true);
    slider->setValue(val);
    slider->blockSignals(sig);
}
