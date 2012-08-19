#include "scaledsliderwidget.h"


ScaledSliderWidget::ScaledSliderWidget( QWidget *parent, Qt::Orientation orientation,  ScaledSlider::Scale type) {

    if (orientation==Qt::Horizontal) {
        m_slider=new ScaledSlider(parent, orientation, QwtSlider::BottomScale, QwtSlider::Trough | QwtSlider::Groove,type);
        this->horizontalLayout(parent);
    } else {
        m_slider=new ScaledSlider(parent, orientation, QwtSlider::LeftScale, QwtSlider::Trough | QwtSlider::Groove,type);
        this->verticalLayout(parent);
    }
    connect(m_slider,SIGNAL(convertedValueChanged(double)), this ,SLOT(updateValue(double)));
}

ScaledSliderWidget::ScaledSliderWidget( QWidget *parent, Qt::Orientation orientation) {
    if (orientation==Qt::Horizontal) {
        m_slider=new ScaledSlider(parent, orientation, QwtSlider::BottomScale, QwtSlider::Trough | QwtSlider::Groove); 
        this->horizontalLayout(parent);
    } else {
        m_slider=new ScaledSlider(parent, orientation, QwtSlider::LeftScale, QwtSlider::Trough | QwtSlider::Groove);
        this->verticalLayout(parent);
    }
    connect(m_slider,SIGNAL(convertedValueChanged(double)), this ,SLOT(updateValue(double)));
}

void ScaledSliderWidget::setScale(double vmin, double vmax, double step) {
    m_slider->setScale(vmin,vmax,step);
}

void ScaledSliderWidget::verticalLayout(QWidget *parent) {
    QGridLayout * l=new QGridLayout(parent);

   // l->setColumnMinimumWidth(0,(int)horizontalOrientation_minWidth*3/4);
    //l->setColumnMinimumWidth(1,(int)horizontalOrientation_minWidth*1/4);

    //Define spacing variable for layout geometry
    l->setSpacing(spacing);
    l->setColumnStretch(0,2);
    l->setColumnStretch(1,2);

    m_slider->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    l->addWidget(m_slider,0,0,3,1);
    m_slider->setHandleSize(4, 10);

    m_sliderName=new QLabel("Name, a very long name",parent);
    m_sliderName->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    m_sliderName->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    l->addWidget(m_sliderName,0,1,1,4);

    m_magnitudeMeasure=new QLabel("(Value)",parent);
    m_magnitudeMeasure->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    m_magnitudeMeasure->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    l->addWidget(m_magnitudeMeasure,1,1,1,4);

    m_magnitudeValue=new QLineEdit(parent);
    m_magnitudeValue->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    m_magnitudeValue->setDisabled(true);
    m_magnitudeValue->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    l->addWidget(m_magnitudeValue,2,1,1,4);

    QFont f=m_slider->font();
    f.setPointSize(fontBasePoint);
    this->setFont(f);
    this->setLayout(l);
    this->setMinimumWidth(verticalOrientation_minWidth);
    this->setMinimumHeight(verticalOrientation_minWidth);
}

void ScaledSliderWidget::horizontalLayout(QWidget *parent)
{
    QGridLayout * l=new QGridLayout(parent);

    l->setSpacing(spacing);
    l->setColumnStretch(0,3);
    l->setColumnStretch(1,2);

    //Define spacing variable for layout geometry
    m_slider->setHandleSize(4, 8);
    m_slider->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    l->addWidget(m_slider,1,0,Qt::AlignTop);

    m_sliderName=new QLabel("Name, a very long name",parent);
    m_sliderName->setAlignment(Qt::AlignCenter);
    m_sliderName->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    l->addWidget(m_sliderName,0,0,Qt::AlignHCenter|Qt::AlignBottom);

    m_magnitudeMeasure=new QLabel("(Value)",parent);
    m_magnitudeMeasure->setAlignment(Qt::AlignCenter);
    m_magnitudeMeasure->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    l->addWidget(m_magnitudeMeasure,0,1,Qt::AlignHCenter|Qt::AlignBottom);

    m_magnitudeValue=new QLineEdit(parent);
    m_magnitudeValue->setAlignment(Qt::AlignCenter);
    m_magnitudeValue->setDisabled(true);
    m_magnitudeValue->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    l->addWidget(m_magnitudeValue,1,1,Qt::AlignTop);

    QFont f=m_slider->font();
    f.setPointSize(fontBasePoint);
    this->setFont(f);
    this->setLayout(l);
    this->setMinimumWidth(horizontalOrientation_minWidth);
    this->setMinimumHeight(horizontalOrientation_minHeight);

}

void ScaledSliderWidget::updateValue(double val) {
    m_magnitudeValue->setText(QString::number ( val, 'f', 2 ) );
    emit(valueChanged(val));
}


void ScaledSliderWidget::setName(QString name) {
    m_sliderName->setText(name);
}

void ScaledSliderWidget::setMeasureUnit(QString name) {
    m_magnitudeMeasure->setText(name);
}

QString ScaledSliderWidget::name() {return m_sliderName->text();}
QString ScaledSliderWidget::measureUnit(){return m_sliderName->text();}

void ScaledSliderWidget::setValue(double val) { m_slider->setNotConvertedValue(val);}
double ScaledSliderWidget::value() { return m_slider->value();}

void ScaledSliderWidget::setFont(QFont  f) {
    QWidget::setFont(f);
    m_sliderName->setFont(f);
    m_magnitudeMeasure->setFont(f);
    m_magnitudeValue->setFont(f);
    f.setPointSize(f.pointSize()-2);
    m_slider->setFont(f);
}
