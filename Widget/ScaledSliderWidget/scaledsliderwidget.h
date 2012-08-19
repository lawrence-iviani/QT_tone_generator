#ifndef SCALEDSLIDERWIDGET_H
#define SCALEDSLIDERWIDGET_H

#include <QWidget>
#include <QLabel>
#include  <QLineEdit>
#include  <QLayout>
#include  <QSizePolicy>
#include  <QFont>
#include "scaledslider.h"

class ScaledSliderWidget : public QWidget
{
    Q_OBJECT
public:
    ScaledSliderWidget( QWidget *parent, Qt::Orientation orientation,  ScaledSlider::Scale type);
    ScaledSliderWidget( QWidget *parent, Qt::Orientation orientation);

    void setScale(double vmin, double vmax, double step);
    void setName(QString name);
    void setMeasureUnit(QString name);
    QString name();
    QString measureUnit();
    void setValue(double val);
    double value();
    void setFont(QFont  f);
signals:
    void valueChanged(double);
public slots:
    void updateValue(double val);
private:
    void horizontalLayout(QWidget *parent);
    void verticalLayout(QWidget *parent);
    static const int  spacing=1;

    //These contant specify the minimum Geometry in terms of min Height/Width
    static const int  horizontalOrientation_minHeight=88;
    static const int  horizontalOrientation_minWidth=320;
    static const int  verticalOrientation_minHeight=150;
    static const int  verticalOrientation_minWidth=145;
    static const int  fontBasePoint=9;

    ScaledSlider * m_slider;
    QLabel * m_sliderName;
    QLabel * m_magnitudeMeasure;
    QLineEdit * m_magnitudeValue;
};

#endif // SCALEDSLIDERWIDGET_H
