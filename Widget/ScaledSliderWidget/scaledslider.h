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

    virtual void setScale(double vmin, double vmax, double step);
    double convertedValue();
signals:
    void convertedValueChanged(double);
public slots:
    void setNotConvertedValue(double val);
protected:
  //  virtual void valueChange();
    int m_scale;

    QwtScaleEngine * m_scaleEngine;
private:
    double logslider2value(double v) { return pow(10.0,v); }
    double value2logslider(double v) { return log10(v); }

private slots:
    void convertSliderValue(double value);
};

#endif // SCALESLIDER_H
