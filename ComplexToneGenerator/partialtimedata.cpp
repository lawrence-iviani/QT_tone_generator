#include "partialtimedata.h"

PartialTimeData::PartialTimeData() :
    GenericTimeData()
{
    m_t0=TIMEDATA_DEFAULT_INITTIME;
    this->initControl();
    this->setDuration(TIMEDATA_DEFAULT_DURATION);
}

PartialTimeData::PartialTimeData(double duration, double SRGen) :
    GenericTimeData(duration,SRGen)
{
    m_t0=TIMEDATA_DEFAULT_INITTIME;
    this->initControl();
    this->setDuration(duration);
}

void PartialTimeData::setDuration(double duration) {
    if (duration < 0) {
        duration=0;
    }

    /*This avoid start time happens afer max duration.
      */
    if (this->startTime() > this->maxDuration()) this->setStartTime(this->maxDuration());

    double maxtime=this->minStartTime()+this->maxDuration();//The max time allowed by the base class
    if ((this->startTime()+duration) > maxtime) {
        m_duration=maxtime-this->startTime();
    } else {
        m_duration=duration;
    }
    //Update UI
    bool sigStatus=m_partialDataControl.widgetDuration->blockSignals(true);
    m_partialDataControl.widgetDuration->setValue(m_duration);
    m_partialDataControl.widgetDuration->blockSignals(sigStatus);
    this->updateData();
}

void PartialTimeData::setStartTime(double t0) {
    double maxtime=this->minStartTime()+this->maxDuration();//The max time allowed by the base class

    if (t0 < this->minStartTime()) {
        m_t0=this->minStartTime();
    } else if ( t0>maxtime) {
        m_t0=maxtime;
    } else {
        m_t0=t0;
    }
    //Update UI
    bool sigStatus=m_partialDataControl.widget_t0->blockSignals(true);
    m_partialDataControl.widget_t0->setValue(m_t0);
    m_partialDataControl.widget_t0->blockSignals(sigStatus);
    //Call set duration to verify if the duration must be clipped and to recalc  the curve
    this->setDuration(m_duration);
}

void PartialTimeData::setMaxDuration(double maxDuration) {
    if ( (this->startTime()+this->duration())>maxDuration ) {
        this->setMaxDurationAndUpdate(maxDuration,false);
        this->setDuration(maxDuration-this->startTime());
    } else {
        this->setMaxDurationAndUpdate(maxDuration,true);
    }
}

void PartialTimeData::initControl() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //set duration
    m_partialDataControl.widgetDuration = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_partialDataControl.widgetDuration->setScale(0,TIMEDATA_DEFAULT_MAX_TIME,TIMEDATA_DEFAULT_TIMESTEP);//TODO: this needs to be set from an external part, ie the base class
    m_partialDataControl.widgetDuration->setValue(this->duration());
    m_partialDataControl.widgetDuration->setName("Duration");
    m_partialDataControl.widgetDuration->setMeasureUnit("Sec.");
    m_partialDataControl.widgetDuration->setFont(f);
    connect(m_partialDataControl.widgetDuration,SIGNAL(valueChanged(double)),this,SLOT(setDuration(double)));

    //set t0
    m_partialDataControl.widget_t0 = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_partialDataControl.widget_t0->setScale(0,TIMEDATA_DEFAULT_MAX_TIME,TIMEDATA_DEFAULT_TIMESTEP);
    m_partialDataControl.widget_t0->setValue(this->startTime());
    m_partialDataControl.widget_t0->setName("Start Time");
    m_partialDataControl.widget_t0->setMeasureUnit("Sec.");
    m_partialDataControl.widget_t0->setFont(f);
    connect(m_partialDataControl.widget_t0,SIGNAL(valueChanged(double)),this,SLOT(setStartTime(double)));
}

