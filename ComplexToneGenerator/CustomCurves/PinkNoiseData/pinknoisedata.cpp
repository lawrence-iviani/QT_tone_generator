#include "pinknoisedata.h"


//---------- PARAMETERS ----------
PinkNoiseParams::PinkNoiseParams() :
    m_average(PINKNOISE_DEFAULT_AVERAGE),
    m_gain(PINKNOISE_DEFAULT_GAIN)
{}

PinkNoiseParams::PinkNoiseParams(qreal average,qreal gain):
    m_average(average),
    m_gain(gain)
{}

//---------- UI ----------
PinkNoiseUI::PinkNoiseUI(QWidget *widget ) :
    QWidget(widget)
{
    this->initControlWidget();
}

void PinkNoiseUI::initControlWidget() {
   // QWidget * _widget=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    QHBoxLayout * l=new QHBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    this->setLayout(l);
    this->setFont(f);

    //set average
    m_pinkNoiseDataControl.sliderAverage= new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Linear) ;
    m_pinkNoiseDataControl.sliderAverage->setScale(-1,1,0.01);
    m_pinkNoiseDataControl.sliderAverage->setName("Average");
    m_pinkNoiseDataControl.sliderAverage->setMeasureUnit("");
    m_pinkNoiseDataControl.sliderAverage->setFont(f);
    connect(m_pinkNoiseDataControl.sliderAverage,SIGNAL(valueChanged(qreal)),this,SIGNAL(averageUIChanged(qreal)));

    //set gain
    m_pinkNoiseDataControl.sliderGain= new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Linear) ;
    m_pinkNoiseDataControl.sliderGain->setScale(0,2,0.01);
    m_pinkNoiseDataControl.sliderGain->setName("Linear\n gain");
    m_pinkNoiseDataControl.sliderGain->setMeasureUnit("");
    m_pinkNoiseDataControl.sliderGain->setFont(f);
    connect(m_pinkNoiseDataControl.sliderGain,SIGNAL(valueChanged(qreal)),this,SIGNAL(gainUIChanged(qreal)));

    //Lay out all the control);
    l->addWidget(m_pinkNoiseDataControl.sliderGain,1,Qt::AlignLeft);
    l->addWidget(m_pinkNoiseDataControl.sliderAverage,1,Qt::AlignLeft);

}

void PinkNoiseUI::averageUIUpdate(qreal average) {
    if (average!=m_pinkNoiseDataControl.sliderAverage->value())
        m_pinkNoiseDataControl.sliderAverage->setValue(average);
}

void PinkNoiseUI::gainUIUpdate(qreal gain) {
    if (gain!=m_pinkNoiseDataControl.sliderGain->value())
        m_pinkNoiseDataControl.sliderGain->setValue(gain);
}

