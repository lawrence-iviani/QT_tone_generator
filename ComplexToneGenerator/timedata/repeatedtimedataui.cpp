#include "repeatedtimedataui.h"

RepeatedTimeDataUI::RepeatedTimeDataUI(QWidget *widget ) :
    PartialTimeDataUI(widget)
{
    this->initControlWidget();
}

RepeatedTimeDataUI::~RepeatedTimeDataUI() {

}

void RepeatedTimeDataUI::initControlWidget() {
    QWidget * _widget=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    QHBoxLayout * l=new QHBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    _widget->setLayout(l);
    _widget->setFont(f);

//    //set duration
    m_repeatedDataControl.widgetBlankTime = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_repeatedDataControl.widgetBlankTime->setDigitAccuracy(3);
    m_repeatedDataControl.widgetBlankTime->setScale(0,TIMEDATA_DEFAULT_PROJECT_TIME,TIMEDATA_DEFAULT_TIMESTEP);//TODO: this needs to be set from an external part, ie the base class
    m_repeatedDataControl.widgetBlankTime->setName("Blank Time");
    m_repeatedDataControl.widgetBlankTime->setMeasureUnit("Sec.");
    m_repeatedDataControl.widgetBlankTime->setFont(f);
    connect(m_repeatedDataControl.widgetBlankTime,SIGNAL(valueChanged(qreal)),this,SIGNAL(blankTimeUIChanged(qreal)));

    //layouting
    l->addWidget(m_repeatedDataControl.widgetBlankTime,1,Qt::AlignLeft);

    this->addWidget(_widget, "Repeated time Controls");
}

void RepeatedTimeDataUI::blankTimeUIUpdate(qreal blanktime) {
    if (blanktime!=m_repeatedDataControl.widgetBlankTime->value())
        m_repeatedDataControl.widgetBlankTime->setValue(blanktime);
}

void RepeatedTimeDataUI::setBlankTimeScale(qreal maxDuration) {
    if (maxDuration>=0) {
        m_repeatedDataControl.widgetBlankTime->setScale(0,maxDuration,TIMEDATA_DEFAULT_TIMESTEP);
    }
}
