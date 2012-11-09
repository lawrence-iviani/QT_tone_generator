#include "partialtimedataui.h"

PartialTimeDataUI::PartialTimeDataUI(PartialTimeData *ptd, QWidget *widget ) :
    CustomCurveUI(widget),
    m_partiaTimeData(ptd)
{
    this->initControlWidget();
    m_partiaTimeData->getControlWidget()->addControlFrame(this,"PTD control");
}

void PartialTimeDataUI::updateControlUI() {
    qDebug() << "PartialTimeDataUI::updateControlUI called";

    bool sigStatus=m_partialDataControl.widgetDuration->blockSignals(true);
    m_partialDataControl.widgetDuration->setValue(m_partiaTimeData->duration());
    m_partialDataControl.widgetDuration->blockSignals(sigStatus);

    sigStatus=m_partialDataControl.widget_t0->blockSignals(true);
    m_partialDataControl.widget_t0->setValue(m_partiaTimeData->startTime());
    m_partialDataControl.widget_t0->blockSignals(sigStatus);

}

void PartialTimeDataUI::initControlWidget() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    QVBoxLayout * l=new QVBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    this->setLayout(l) ;
    this->setFont(f);

    //set duration
    m_partialDataControl.widgetDuration = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_partialDataControl.widgetDuration->setScale(0,TIMEDATA_DEFAULT_MAX_TIME,TIMEDATA_DEFAULT_TIMESTEP);//TODO: this needs to be set from an external part, ie the base class
    m_partialDataControl.widgetDuration->setName("Duration");
    m_partialDataControl.widgetDuration->setMeasureUnit("Sec.");
    m_partialDataControl.widgetDuration->setFont(f);
    connect(m_partialDataControl.widgetDuration,SIGNAL(valueChanged(qreal)),m_partiaTimeData,SLOT(setDuration(qreal)));

    //set t0
    m_partialDataControl.widget_t0 = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_partialDataControl.widget_t0->setScale(0,TIMEDATA_DEFAULT_MAX_TIME,TIMEDATA_DEFAULT_TIMESTEP);

    m_partialDataControl.widget_t0->setName("Start Time");
    m_partialDataControl.widget_t0->setMeasureUnit("Sec.");
    m_partialDataControl.widget_t0->setFont(f);
    connect(m_partialDataControl.widget_t0,SIGNAL(valueChanged(qreal)),m_partiaTimeData,SLOT(setStartTime(qreal)));

    //layouting
    l->addWidget(m_partialDataControl.widgetDuration,1,Qt::AlignCenter);
    l->addWidget(m_partialDataControl.widget_t0,1,Qt::AlignLeft);

    //Setting values
    m_partialDataControl.widgetDuration->setValue(m_partiaTimeData->duration());
    m_partialDataControl.widget_t0->setValue(m_partiaTimeData->startTime());

}
