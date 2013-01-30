#include <QtGui/QApplication>
#include "scaledsliderwidget.h"
#include  <QMainWindow>
#include  <QVBoxLayout>


void setupWin1() {
    ScaledSliderWidget * m_slider_H1=new ScaledSliderWidget(NULL, Qt::Horizontal,  ScaledSlider::Logarithmic );
    ScaledSliderWidget * m_slider_H2=new ScaledSliderWidget(NULL, Qt::Horizontal,  ScaledSlider::Linear );
    ScaledSliderWidget * m_slider_V1=new ScaledSliderWidget(NULL, Qt::Vertical,  ScaledSlider::Linear );
    ScaledSliderWidget * m_slider_V2=new ScaledSliderWidget(NULL, Qt::Vertical,  ScaledSlider::Logarithmic );
    QWidget * _win=new QWidget();
    QHBoxLayout * _l=new QHBoxLayout(_win);
    _l->setSpacing(0);
    _l->setSizeConstraint(QLayout::SetMinAndMaxSize);

    m_slider_H1->setScale(1,100000,0.1);
    m_slider_H1->setName("Log Horizontal Slider");
    m_slider_H1->setMeasureUnit("Log measure");
    m_slider_H1->setDigitAccuracy(4);
    m_slider_H1->setValue(610.2);
    _l->addWidget(m_slider_H1);

    m_slider_H2->setScale(1,1000,1);
    m_slider_H2->setName("Lin Ver Slider");
    m_slider_H2->setMeasureUnit("Lin measure");
    m_slider_H2->setDigitAccuracy(3);
    m_slider_H2->setValue(10.2);
    _l->addWidget(m_slider_H2);

    m_slider_V1->setScale(1,1000,1);
    m_slider_V1->setName("Lin Ver Slider");
    m_slider_V1->setMeasureUnit("Lin measure");
    m_slider_V1->setDigitAccuracy(3);
    m_slider_V1->setValue(10.2);
    _l->addWidget(m_slider_V1);

    m_slider_V2->setScale(1,100000,0.1);
    m_slider_V2->setName("Log Horizontal Slider");
    m_slider_V2->setMeasureUnit("Log measure");
    m_slider_V2->setDigitAccuracy(4);
    m_slider_V2->setValue(610.2);
    _l->addWidget(m_slider_V2);

    _win->show();

}

void setupWin2() {
    ScaledSliderWidget * m_slider_V1=new ScaledSliderWidget(NULL, Qt::Vertical,  ScaledSlider::Linear );
    ScaledSliderWidget * m_slider_V2=new ScaledSliderWidget(NULL, Qt::Vertical,  ScaledSlider::Logarithmic );
    ScaledSliderWidget * m_slider_H1=new ScaledSliderWidget(NULL, Qt::Horizontal,  ScaledSlider::Logarithmic );
    ScaledSliderWidget * m_slider_H2=new ScaledSliderWidget(NULL, Qt::Horizontal,  ScaledSlider::Linear );
    QWidget * _win=new QWidget();
    QVBoxLayout * _l=new QVBoxLayout(_win);
    _l->setSpacing(0);
    _l->setSizeConstraint(QLayout::SetMinAndMaxSize);

    m_slider_V1->setScale(1,1000,1);
    m_slider_V1->setName("Lin Ver Slider");
    m_slider_V1->setMeasureUnit("Lin measure");
    m_slider_V1->setDigitAccuracy(3);
    m_slider_V1->setValue(10.2);
    _l->addWidget(m_slider_V1);

    m_slider_V2->setScale(1,100000,0.1);
    m_slider_V2->setName("Log Horizontal Slider");
    m_slider_V2->setMeasureUnit("Log measure");
    m_slider_V2->setDigitAccuracy(4);
    m_slider_V2->setValue(610.2);
    _l->addWidget(m_slider_V2);

    m_slider_H1->setScale(1,100000,0.1);
    m_slider_H1->setName("Log Horizontal Slider");
    m_slider_H1->setMeasureUnit("Log measure");
    m_slider_H1->setDigitAccuracy(4);
    m_slider_H1->setValue(610.2);
    _l->addWidget(m_slider_H1);

    m_slider_H2->setScale(1,1000,1);
    m_slider_H2->setName("Lin Ver Slider");
    m_slider_H2->setMeasureUnit("Lin measure");
    m_slider_H2->setDigitAccuracy(3);
    m_slider_H2->setValue(10.2);
    _l->addWidget(m_slider_H2);

    _win->show();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setupWin1();
    setupWin2();
    
    return a.exec();
}
