#ifndef SCALEDSLIDERWIDGET_H
#define SCALEDSLIDERWIDGET_H

#include <QWidget>
#include <QLabel>
#include  <QLineEdit>
#include  <QLayout>
#include  <QSizePolicy>
#include  <QFont>
#include  <QDebug>
#include "scaledslider.h"


static const unsigned short int SCALED_SLIDER_DEFAULT_DIGIT_ACCURACY=2;

class ScaledSliderWidget : public QWidget
{
    Q_OBJECT
public:
    ScaledSliderWidget( QWidget *parent, Qt::Orientation orientation,  ScaledSlider::Scale type);
    ScaledSliderWidget( QWidget *parent, Qt::Orientation orientation);

    void setScale(qreal vmin, qreal vmax, qreal step);
    void setName(QString name);
    void setMeasureUnit(QString name);
    QString name();
    QString measureUnit();
    void setValue(qreal val);
    qreal value();
    void setFont(QFont f);
    unsigned short int digitAccuracy() {return m_digitAccuracy;}
signals:
    void valueChanged(qreal);
public slots:
    void setDigitAccuracy(unsigned short int accuracy) {m_digitAccuracy=accuracy;}
    void setMaxScaleValue(qreal vmax);
    void updateValue(qreal val);
    void sliderStartMoving();
    void sliderStopMoving();
    void textValueEdited();
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
    qreal m_previousValue;
    bool m_sliderIsMoving;
    unsigned short int m_digitAccuracy;
};

#endif // SCALEDSLIDERWIDGET_H
