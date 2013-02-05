#include "partialtimedataui.h"

PartialTimeDataUI::PartialTimeDataUI(QWidget *widget ) :
    GenericTimeDataUI(widget)
{
    this->initControlWidget();
}

PartialTimeDataUI::~PartialTimeDataUI() {

}

void PartialTimeDataUI::initControlWidget() {
    QWidget * _widget=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    QHBoxLayout * l=new QHBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    _widget->setLayout(l);
    _widget->setFont(f);

    //set duration
    m_partialDataControl.widgetDuration = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_partialDataControl.widgetDuration->setDigitAccuracy(3);
    m_partialDataControl.widgetDuration->setScale(0,TIMEDATA_DEFAULT_PROJECT_TIME,TIMEDATA_DEFAULT_TIMESTEP);//TODO: this needs to be set from an external part, ie the base class
    m_partialDataControl.widgetDuration->setName("Duration");
    m_partialDataControl.widgetDuration->setMeasureUnit("Sec.");
    m_partialDataControl.widgetDuration->setFont(f);
    connect(m_partialDataControl.widgetDuration,SIGNAL(valueChanged(qreal)),this,SIGNAL(durationUIChanged(qreal)));

    //set t0
    m_partialDataControl.widget_t0 = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_partialDataControl.widget_t0->setDigitAccuracy(3);
    m_partialDataControl.widget_t0->setScale(0,TIMEDATA_DEFAULT_PROJECT_TIME,TIMEDATA_DEFAULT_TIMESTEP);
    m_partialDataControl.widget_t0->setName("Start Time");
    m_partialDataControl.widget_t0->setMeasureUnit("Sec.");
    m_partialDataControl.widget_t0->setFont(f);
    connect(m_partialDataControl.widget_t0,SIGNAL(valueChanged(qreal)),this,SIGNAL(t0UIChanged(qreal)));

    //layouting
    l->addWidget(m_partialDataControl.widgetDuration,1,Qt::AlignLeft);
    l->addWidget(m_partialDataControl.widget_t0,1,Qt::AlignLeft);

    this->addWidget(_widget, "Partial time Controls");
}

void PartialTimeDataUI::durationUIUpdate(qreal duration) {
    if (duration!=m_partialDataControl.widgetDuration->value())
        m_partialDataControl.widgetDuration->setValue(duration);
}

void PartialTimeDataUI::t0UIUpdate(qreal t0) {
    if (t0!=m_partialDataControl.widget_t0->value())
        m_partialDataControl.widget_t0->setValue(t0);
}

//void PartialTimeDataUI::durationChange(qreal duration) {
//    bool _prevValue=m_partialDataControl.widgetDuration->blockSignals(true);
//    m_partialDataControl.widgetDuration->setMaxScaleValue(duration);
//    m_partialDataControl.widgetDuration->blockSignals(_prevValue);

//    _prevValue=m_partialDataControl.widget_t0->blockSignals(true);
//    m_partialDataControl.widget_t0->setMaxScaleValue(duration);
//    m_partialDataControl.widget_t0->blockSignals(_prevValue);
//}


