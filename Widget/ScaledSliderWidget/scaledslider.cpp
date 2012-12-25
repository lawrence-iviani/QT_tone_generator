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
    connect(this, SIGNAL(valueChanged(double)) ,this,SLOT (convertSliderValueD(double)));
}

ScaledSlider::ScaledSlider( QWidget *parent,
        Qt::Orientation orientation, ScalePos scalePos,
        BackgroundStyles bgStyle ):
        QwtSlider( parent, orientation,scalePos,bgStyle )
{
    m_scale=ScaledSlider::Linear;

    connect(this, SIGNAL(valueChanged(double )) ,this,SLOT (convertSliderValueD(double)));
}


void ScaledSlider::setScale(qreal vmin, qreal vmax, qreal step) {
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

void  ScaledSlider::convertSliderValueD(double value) {
    convertSliderValueR((qreal)value);
}

void  ScaledSlider::convertSliderValueR(qreal value) {
    switch (m_scale) {
        case ScaledSlider::Logarithmic:
            emit(convertedValueChanged(this->logslider2value(value)));
            break;
        default:
            emit(convertedValueChanged(value));
            break;
    }

}

void ScaledSlider::setNotConvertedValue(qreal val) {
    switch (m_scale) {
        case ScaledSlider::Logarithmic:
            this->setValue(this->value2logslider(val));
            break;
        default:
            this->setValue(val);
            break;
    }
}

qreal ScaledSlider::convertedValue() {
    qreal retval;
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


qreal ScaledSlider::getMinimumScaleValue() {
    qreal retval;
    switch (m_scale) {
        case ScaledSlider::Logarithmic:
            retval=this->logslider2value(this->minValue());
            break;
        default:
            retval=this->minValue();
            break;
    }
    return retval;
}

qreal ScaledSlider::getMaximumScaleValue(){
    qreal retval;
    switch (m_scale) {
        case ScaledSlider::Logarithmic:
            retval=this->logslider2value(this->maxValue());
            break;
        default:
            retval=this->maxValue();
            break;
    }
    return retval;
}
