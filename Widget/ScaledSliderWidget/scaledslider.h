#ifndef SCALESLIDER_H
#define SCALESLIDER_H

#include <QWidget>
#include <qwt_slider.h>
#include <qwt_scale_engine.h>
#include  <math.h>



class ScaledSlider :  public QwtSlider
{
    Q_OBJECT
    Q_ENUMS( Scale )
public:
    enum Scale {Linear=0, Logarithmic=1};
    ScaledSlider( QWidget *parent, Qt::Orientation orientation, ScalePos scalePos, BackgroundStyles bgStyle, ScaledSlider::Scale type);
    ScaledSlider( QWidget *parent, Qt::Orientation orientation, ScalePos scalePos, BackgroundStyles bgStyle);

    virtual void setScale(qreal vmin, qreal vmax, qreal step);
    qreal convertedValue();

    qreal getMinimumScaleValue();
    qreal getMaximumScaleValue();
signals:
    void convertedValueChanged(qreal);
public slots:
    void setNotConvertedValue(qreal val);
protected:
  //  virtual void valueChange();
    int m_scale;

    QwtScaleEngine * m_scaleEngine;
private:
    qreal logslider2value(qreal v) { return pow(10.0,v); }
    qreal value2logslider(qreal v) { return log10(v); }

private slots:
    void convertSliderValueR(qreal value);
    void convertSliderValueD(double value);
};

#endif // SCALESLIDER_H
