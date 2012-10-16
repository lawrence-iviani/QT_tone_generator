#include "zmp_handler.h"


ZMP_Handler::ZMP_Handler(QwtPlotCanvas *canvas, QObject *parent) :
    QObject(parent)
{
    this->init(canvas);
}

void ZMP_Handler::init(QwtPlotCanvas * canvas) {
    // zoom in/out with the wheel
    m_magnifier=new QwtPlotMagnifier(canvas);

    // panning with the left mouse button
    m_pan=new QwtPlotPanner(canvas);

    // zoom in/out with the wheel
    m_zoom=new QwtPlotZoomer(canvas);//QwtPlot::xTop , QwtPlot::yLeft,canvas);
    m_zoom->setRubberBand(QwtPicker::RectRubberBand);

    //Setting Picker cross with a click
    m_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, canvas);
    m_picker->setStateMachine(new QwtPickerDragPointMachine());
    m_picker->setRubberBand(QwtPicker::CrossRubberBand);

    //set color
    this->setBaseColor(QColor(Qt::darkGray));

    this->disableAll();
}

void ZMP_Handler::setBaseColor(QColor color) {
    m_picker->setRubberBandPen(color);
    m_picker->setTrackerPen(color);
    m_zoom->setRubberBandPen(color);
    m_zoom->setTrackerPen(color);
}

void ZMP_Handler::disableAll() {
    m_zoom->setEnabled(false);
    m_pan->setEnabled(false);
    m_magnifier->setEnabled(false);
    m_picker->setEnabled(false);
}

void ZMP_Handler::enableZoomer() {
    this->disableAll();
    m_zoom->setEnabled(true);
}

void ZMP_Handler::enablePanner() {
    this->disableAll();
    m_pan->setEnabled(true);
    m_magnifier->setEnabled(true);
}

void ZMP_Handler::enablePicker() {
    this->disableAll();
    m_picker->setEnabled(true);
}


