#include "zmp_handler.h"


//ZMP_Handler::ZMP_Handler(QObject *parent) :
//    QObject(parent)
//{
//}

ZMP_Handler::ZMP_Handler(QwtPlotCanvas *canvas, QObject *parent) :
    QObject(parent)
{
    m_togglePanZoom=true;
    this->init(canvas);
}

void ZMP_Handler::init(QwtPlotCanvas * canvas) {
    // zoom in/out with the wheel
    m_magnifier=new QwtPlotMagnifier(canvas);
    // panning with the left mouse button
    m_pan=new QwtPlotPanner(canvas);
    // zoom in/out with the wheel
    m_zoom=new QwtPlotZoomer(canvas);
    this->switchZP();
}

void ZMP_Handler::enableZoomer() {
    m_togglePanZoom=false;
    this->switchZP();
}

void ZMP_Handler::enablePanner() {
    m_togglePanZoom=true;
    this->switchZP();
}

void ZMP_Handler::switchZP() {
    m_zoom->setEnabled(!m_togglePanZoom);
    m_pan->setEnabled(m_togglePanZoom);
    m_magnifier->setEnabled(m_togglePanZoom);
}


