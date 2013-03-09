#include "squaredata.h"


//---------- PARAMETERS ----------
SquareParams::SquareParams() :
    m_amplitude(SQUAREDATA_DEFAULT_AMPLITUDE),
    m_frequency(SQUAREDATA_DEFAULT_FREQUENCY),
    m_mean(SQUAREDATA_DEFAULT_MEAN)
{}

SquareParams::SquareParams(qreal amplitude, qreal frequency, qreal meanValue):
    m_amplitude(amplitude),
    m_frequency(frequency),
    m_mean(meanValue)
{}

//---------- UI ----------
SquareUI::SquareUI(QWidget *widget ) :
    QWidget(widget)
{
    this->initControlWidget();
}

void SquareUI::initControlWidget() {
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
    m_squareDataControl.sliderFrequency= new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Logarithmic) ;
    m_squareDataControl.sliderFrequency->setScale(10,22000,0.001);
    m_squareDataControl.sliderFrequency->setName("Freq.");
    m_squareDataControl.sliderFrequency->setMeasureUnit("Sec.");
    m_squareDataControl.sliderFrequency->setFont(f);
    connect(m_squareDataControl.sliderFrequency,SIGNAL(valueChanged(qreal)),this,SIGNAL(frequencyUIChanged(qreal)));

    //set amplitude
    m_squareDataControl.sliderAmplitude=new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Linear) ;
    m_squareDataControl.sliderAmplitude->setScale(0,1.0,0.01);
    m_squareDataControl.sliderAmplitude->setName("Amplitude");
    m_squareDataControl.sliderAmplitude->setMeasureUnit("0-1");
    m_squareDataControl.sliderAmplitude->setFont(f);
    connect(m_squareDataControl.sliderAmplitude,SIGNAL(valueChanged(qreal)),this,SIGNAL(amplitudeUIChanged(qreal)));

    //set amplitude
    m_squareDataControl.sliderMeanValue=new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Linear) ;
    m_squareDataControl.sliderMeanValue->setScale(-1.0,1.0,0.01);
    m_squareDataControl.sliderMeanValue->setName("Mean value");
    m_squareDataControl.sliderMeanValue->setMeasureUnit("0-1");
    m_squareDataControl.sliderMeanValue->setFont(f);
    connect(m_squareDataControl.sliderMeanValue,SIGNAL(valueChanged(qreal)),this,SIGNAL(amplitudeUIChanged(qreal)));

    //Lay out all the control);
    l->addWidget(m_squareDataControl.sliderFrequency,1,Qt::AlignLeft);
    l->addWidget(m_squareDataControl.sliderAmplitude,1,Qt::AlignLeft);
    l->addWidget(m_squareDataControl.sliderMeanValue,1,Qt::AlignLeft);

}

void SquareUI::amplitudeUIUpdate(qreal amplitude) {
    if (amplitude!=m_squareDataControl.sliderAmplitude->value())
        m_squareDataControl.sliderAmplitude->setValue(amplitude);
}

void SquareUI::frequencyUIUpdate(qreal frequency) {
    if (frequency!=m_squareDataControl.sliderFrequency->value())
        m_squareDataControl.sliderFrequency->setValue(frequency);
}

void SquareUI::meanValueUIUpdate(qreal meanValue) {
    if (meanValue!=m_squareDataControl.sliderMeanValue->value())
        m_squareDataControl.sliderMeanValue->setValue(meanValue);
}

