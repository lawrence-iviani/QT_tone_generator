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

//    bool sigStatus=m_partialDataControl.widgetDuration->blockSignals(true);
//    m_partialDataControl.widgetDuration->setValue(m_partiaTimeData->duration());
//    m_partialDataControl.widgetDuration->blockSignals(sigStatus);



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
//    m_partialDataControl.widgetDuration = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
//    m_partialDataControl.widgetDuration->setScale(0,TIMEDATA_DEFAULT_MAX_TIME,TIMEDATA_DEFAULT_TIMESTEP);//TODO: this needs to be set from an external part, ie the base class
//    m_partialDataControl.widgetDuration->setName("Duration");
//    m_partialDataControl.widgetDuration->setMeasureUnit("Sec.");
//    m_partialDataControl.widgetDuration->setFont(f);
//    connect(m_partialDataControl.widgetDuration,SIGNAL(valueChanged(qreal)),m_partiaTimeData,SLOT(setDuration(qreal)));

    //layouting
 //   l->addWidget(m_partialDataControl.widgetDuration,1,Qt::AlignLeft);

    //Setting values
 //   m_partialDataControl.widget_t0->setValue(m_partiaTimeData->startTime());

}
