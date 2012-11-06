#include "partialtimedataui.h"

PartialTimeDataUI::PartialTimeDataUI(PartialTimeData *ptd, QWidget *parent ) :
    GenericTimeDataUI((GenericTimeData *)ptd,parent),
    m_partiaTimeData(ptd)
{
    this->initControl();
    m_partiaTimeData->getControlWidget()->addControlFrame(m_partialDataControl.controlFrame,"PTD control");

}

void PartialTimeDataUI::updateControlUI() {
    GenericTimeDataUI::updateControlUI();

    bool sigStatus=m_partialDataControl.widgetDuration->blockSignals(true);
    m_partialDataControl.widgetDuration->setValue(m_partiaTimeData->duration());
    m_partialDataControl.widgetDuration->blockSignals(sigStatus);

    sigStatus=m_partialDataControl.widget_t0->blockSignals(true);
    m_partialDataControl.widget_t0->setValue(m_partiaTimeData->startTime());
    m_partialDataControl.widget_t0->blockSignals(sigStatus);

}

void PartialTimeDataUI::initControl() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    m_partialDataControl.controlFrame=new CustomCurveUI();
    QVBoxLayout * l=new QVBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    m_partialDataControl.controlFrame->setLayout(l) ;
    m_partialDataControl.controlFrame->setFont(f);

    //set duration
    m_partialDataControl.widgetDuration = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_partialDataControl.widgetDuration->setScale(0,TIMEDATA_DEFAULT_MAX_TIME,TIMEDATA_DEFAULT_TIMESTEP);//TODO: this needs to be set from an external part, ie the base class
    m_partialDataControl.widgetDuration->setValue(m_partiaTimeData->duration());
    m_partialDataControl.widgetDuration->setName("Duration");
    m_partialDataControl.widgetDuration->setMeasureUnit("Sec.");
    m_partialDataControl.widgetDuration->setFont(f);
    connect(m_partialDataControl.widgetDuration,SIGNAL(valueChanged(qreal)),m_partiaTimeData,SLOT(setDuration(qreal)));

    //set t0
    m_partialDataControl.widget_t0 = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_partialDataControl.widget_t0->setScale(0,TIMEDATA_DEFAULT_MAX_TIME,TIMEDATA_DEFAULT_TIMESTEP);
    m_partialDataControl.widget_t0->setValue(m_partiaTimeData->startTime());
    m_partialDataControl.widget_t0->setName("Start Time");
    m_partialDataControl.widget_t0->setMeasureUnit("Sec.");
    m_partialDataControl.widget_t0->setFont(f);
    connect(m_partialDataControl.widget_t0,SIGNAL(valueChanged(qreal)),m_partiaTimeData,SLOT(setStartTime(qreal)));

    l->addWidget(m_partialDataControl.widgetDuration,1,Qt::AlignCenter);
    l->addWidget(m_partialDataControl.widget_t0,1,Qt::AlignLeft);

}
