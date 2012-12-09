#include "repeatedtimedataui.h"

RepeatedTimeDataUI::RepeatedTimeDataUI(RepeatedTimeData *rtd, QWidget *widget ) :
    CustomCurveUI(widget),
    m_repeatedTimeData(rtd)
{
    this->initControlWidget();
    m_repeatedTimeData->getControlWidget()->addControlFrame(this,"RTD control");
}

void RepeatedTimeDataUI::updateControlUI() {
    qDebug() << "RepeatedTimeDataUI::updateControlUI called";

    bool sigStatus=m_repeatedDataControl.widgetBlankTime->blockSignals(true);
    m_repeatedDataControl.widgetBlankTime->setValue(m_repeatedTimeData->blankTime());
    m_repeatedDataControl.widgetBlankTime->blockSignals(sigStatus);
    emit (controlUIUpdated());
}

void RepeatedTimeDataUI::initControlWidget() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    QHBoxLayout * l=new QHBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    this->setLayout(l) ;
    this->setFont(f);

//    //set duration
    m_repeatedDataControl.widgetBlankTime = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_repeatedDataControl.widgetBlankTime->setScale(0,TIMEDATA_DEFAULT_MAX_TIME,TIMEDATA_DEFAULT_TIMESTEP);//TODO: this needs to be set from an external part, ie the base class
    m_repeatedDataControl.widgetBlankTime->setName("Blank Time");
    m_repeatedDataControl.widgetBlankTime->setMeasureUnit("Sec.");
    m_repeatedDataControl.widgetBlankTime->setFont(f);
    connect(m_repeatedDataControl.widgetBlankTime,SIGNAL(valueChanged(qreal)),m_repeatedTimeData,SLOT(setBlankTime(qreal)));

    //layouting
    l->addWidget(m_repeatedDataControl.widgetBlankTime,1,Qt::AlignLeft);

    //Setting values
    m_repeatedDataControl.widgetBlankTime->setValue(m_repeatedTimeData->blankTime());

}
