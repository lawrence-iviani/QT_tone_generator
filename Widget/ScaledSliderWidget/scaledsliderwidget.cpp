#include "scaledsliderwidget.h"
#include <QDebug>

ScaledSliderWidget::ScaledSliderWidget( QWidget *parent, Qt::Orientation orientation,  ScaledSlider::Scale type) :
    QWidget(parent),
    m_sliderIsMoving(false),
    m_digitAccuracy(SCALED_SLIDER_DEFAULT_DIGIT_ACCURACY)
{
    if (orientation==Qt::Horizontal) {
        m_slider=new ScaledSlider(parent, orientation, QwtSlider::BottomScale,QwtSlider::Groove,type);
        this->horizontalLayout(parent);
    } else {
        m_slider=new ScaledSlider(parent, orientation, QwtSlider::LeftScale, QwtSlider::Groove,type);
        this->verticalLayout(parent);
    }    
    initWidget();
}

void ScaledSliderWidget::initWidget() {
    connect(m_slider,SIGNAL(sliderPressed()),this,SLOT(sliderStartMoving()));
    connect(m_slider,SIGNAL(sliderReleased()) ,this,SLOT(sliderStopMoving()));
    connect(m_magnitudeValue,SIGNAL(editingFinished()), this, SLOT(textValueEdited()));
    m_sliderIsMoving=false;
    connect(m_slider,SIGNAL(convertedValueChanged(qreal)), this ,SLOT(updateValue(qreal)));
}

void ScaledSliderWidget::setScale(qreal vmin, qreal vmax, qreal step) {
    m_slider->setScale(vmin,vmax,step);
}

void ScaledSliderWidget::verticalLayout(QWidget *parent) {
    QGridLayout * l=new QGridLayout(parent);
    this->setLayout(l);

     m_slider->setBorderWidth(1);
     m_slider->setHandleSize( 10, 20 );
     m_slider->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
     l->addWidget(m_slider,0,0,3,1);

    m_sliderName=new QLabel("Name, a very long name",parent);
    m_sliderName->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    l->addWidget(m_sliderName,0,1);

    m_magnitudeMeasure=new QLabel("(Value)",parent);
    m_magnitudeMeasure->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    l->addWidget(m_magnitudeMeasure,1,1);

    m_magnitudeValue=new QLineEdit(parent);
    m_magnitudeValue->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    m_magnitudeValue->setMinimumSize(minLengthLineEdit(),20);
    m_magnitudeValue->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Fixed);
    l->addWidget(m_magnitudeValue,2,1,1,1);

    //Define spacing variable for layout geometry
    this->setMaximumSize(275,120);
    l->setSpacing(2);
    l->setColumnStretch(0,1);
    l->setColumnStretch(1,1);

    QFont f=m_slider->font();
    f.setPointSize(fontBasePoint);
    this->setFont(f);
}

void ScaledSliderWidget::horizontalLayout(QWidget *parent)
{
    QGridLayout * l=new QGridLayout(parent);
     this->setLayout(l);

    m_slider->setBorderWidth(1);
    m_slider->setHandleSize( 25, 10 );
    m_slider->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    l->addWidget(m_slider,1,0,2,2);

    m_sliderName=new QLabel("Name, a very long name",parent);
    m_sliderName->setAlignment(Qt::AlignCenter);
    l->addWidget(m_sliderName,0,0,1,1,Qt::AlignCenter|Qt::AlignBottom);

    m_magnitudeMeasure=new QLabel("(Value)",parent);
    m_magnitudeMeasure->setAlignment(Qt::AlignCenter);
    l->addWidget(m_magnitudeMeasure,0,3,1,1,Qt::AlignCenter|Qt::AlignBottom);

    m_magnitudeValue=new QLineEdit(parent);
    m_magnitudeValue->setAlignment(Qt::AlignCenter);  
    m_magnitudeValue->setMinimumSize(minLengthLineEdit(),20);
    m_magnitudeValue->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Fixed);
    l->addWidget(m_magnitudeValue,1,3,1,2,Qt::AlignBottom);

    this->setMaximumSize(375,100);
    l->setSpacing(2);
    QFont f=m_slider->font();
    f.setPointSize(fontBasePoint);
    this->setFont(f);

}

int ScaledSliderWidget::minLengthLineEdit() {
    return (m_magnitudeValue->font().pointSize()* m_magnitudeValue->text().length());
}

void ScaledSliderWidget::textValueEdited() {
    bool _isCorrectConverted=false;
    qreal _val=m_magnitudeValue->text().toDouble(&_isCorrectConverted);

    //verify conversion
    if (!_isCorrectConverted) {
        //qDebug() << "ScaledSliderWidget::textValueEdited conversion fail";
        m_magnitudeValue->setText(QString::number ( m_previousValue, 'f', m_digitAccuracy ) );
        return;
    }
    //verify limit
    if ( (_val < m_slider->getMinimumScaleValue()) || _val > m_slider->getMaximumScaleValue() ) {
        //qDebug() << "ScaledSliderWidget::textValueEdited "<< _val <<" out of range ("<< m_slider->minValue() << ":" << m_slider->maxValue() << ")";
        m_magnitudeValue->setText(QString::number ( m_previousValue, 'f', m_digitAccuracy ) );
        return;
    }
    if (_val==m_previousValue) {
        //qDebug() << "ScaledSliderWidget::textValueEdited same previous value, do nothing ";
        return;
    }
    //qDebug() << "ScaledSliderWidget::textValueEdited setting "<< _val;
    m_slider->setNotConvertedValue(_val);
    m_magnitudeValue->setMinimumSize(minLengthLineEdit(),20);
}

void ScaledSliderWidget::updateValue(qreal val) {
    m_magnitudeValue->setText(QString::number ( val, 'f', m_digitAccuracy ) );
    if ((!m_sliderIsMoving) && (m_previousValue!=val)) {
        //qDebug() << "ScaledSliderWidget::updateValue emiting valueChanged("<< val <<")";
        emit(valueChanged(val));
        m_previousValue=val;
        m_magnitudeValue->setMinimumSize(minLengthLineEdit(),20);
    }
}

void ScaledSliderWidget::sliderStartMoving() {
    m_sliderIsMoving=true;
}

void ScaledSliderWidget::sliderStopMoving() {
    m_sliderIsMoving=false;
    this->updateValue(m_slider->convertedValue());
}

void ScaledSliderWidget::setName(QString name) {
    m_sliderName->setText(name);
}

void ScaledSliderWidget::setMeasureUnit(QString name) {
    m_magnitudeMeasure->setText(name);
}

QString ScaledSliderWidget::name() {return m_sliderName->text();}
QString ScaledSliderWidget::measureUnit(){return m_sliderName->text();}

void ScaledSliderWidget::setValue(qreal val) { m_slider->setNotConvertedValue(val); }
qreal ScaledSliderWidget::value() { return m_slider->value();}

void ScaledSliderWidget::setFont(QFont  f) {
    QWidget::setFont(f);
    m_sliderName->setFont(f);
    m_magnitudeMeasure->setFont(f);
    m_magnitudeValue->setFont(f);
    f.setPointSize(f.pointSize()-2);
    m_slider->setFont(f);
    m_magnitudeValue->setMinimumSize(minLengthLineEdit(),20);
}

void ScaledSliderWidget::setMaxScaleValue(qreal vmax) {
    m_slider->setScale(m_slider->minValue(),vmax,m_slider->step());
}
