#include "scaledslider.h"

ScaledSlider::ScaledSlider( QWidget *parent,
        Qt::Orientation orientation, ScalePos scalePos,
        BackgroundStyles bgStyle,
        ScaledSlider::Scale type):
        QwtSlider( parent, orientation,scalePos,bgStyle )
{
    m_scale=type;
    switch (m_scale) {
        case ScaledSlider::Logarithmic:
            setScaleEngine(new QwtLog10ScaleEngine);
            break;
        default:
            setScaleEngine(new QwtLinearScaleEngine() );
            break;
    }
    connect(this, SIGNAL(valueChanged(double )) ,this,SLOT (convertSliderValue(double)));
}

ScaledSlider::ScaledSlider( QWidget *parent,
        Qt::Orientation orientation, ScalePos scalePos,
        BackgroundStyles bgStyle ):
        QwtSlider( parent, orientation,scalePos,bgStyle )
{
    m_scale=ScaledSlider::Linear;

    connect(this, SIGNAL(valueChanged(double )) ,this,SLOT (convertSliderValue(double)));
}


void ScaledSlider::setScale(double vmin, double vmax, double step) {
    switch (m_scale) {
        case ScaledSlider::Logarithmic:
            QwtSlider::setRange(this->value2logslider(vmin),this->value2logslider(vmax),step);
            break;
        default:
            QwtSlider::setRange(vmin,vmax,step);
            break;
    }
    QwtSlider::setScale(vmin,vmax);
}

void  ScaledSlider::convertSliderValue(double value) {
    switch (m_scale) {
        case ScaledSlider::Logarithmic:
            emit(convertedValueChanged(this->logslider2value(value)));
            break;
        default:
            emit(convertedValueChanged(value));
            break;
    }

}

void ScaledSlider::setNotConvertedValue(double val) {
    switch (m_scale) {
        case ScaledSlider::Logarithmic:
            this->setValue(this->value2logslider(val));
            break;
        default:
            this->setValue(val);
            break;
    }
}

double ScaledSlider::convertedValue() {
    double retval;
    switch (m_scale) {
        case ScaledSlider::Logarithmic:
            retval=this->logslider2value(this->value());
            break;
        default:
            retval=this->value();
            break;
    }
    return retval;

}

