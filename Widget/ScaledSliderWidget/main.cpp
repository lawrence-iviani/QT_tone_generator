#include <QtGui/QApplication>
#include "scaledsliderwidget.h"
#include  <QMainWindow>
#include  <QVBoxLayout>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    QVBoxLayout * layout=new QVBoxLayout();
    ScaledSliderWidget * m_slider_H=new ScaledSliderWidget(NULL, Qt::Horizontal,  ScaledSlider::Logarithmic );
    ScaledSliderWidget * m_slider_V=new ScaledSliderWidget(NULL, Qt::Vertical,  ScaledSlider::Linear );

    m_slider_H->setScale(1,100000,0.1);
    m_slider_H->setName("Log Horizontal Slider");
    m_slider_H->setMeasureUnit("Log measure");
    m_slider_V->setScale(1,1000,1);
    m_slider_V->setName("Lin Ver Slider");
    m_slider_V->setMeasureUnit("Lin measure");
    layout->addWidget(m_slider_H);
    layout->addWidget(m_slider_V);
    m_slider_H->setValue(610.2);
    m_slider_H->show();
    m_slider_V->setValue(10.2);
    m_slider_V->show();

    
    return a.exec();
}
