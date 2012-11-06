#include "constanttimedata.h"

PartialConstantTimeData::PartialConstantTimeData(qreal duration, qreal SRGen) :
    PartialTimeData(duration,SRGen)
{
    m_amplitude=CONSTDATA_DEFAULT_AMPLITUDE;
    //Init a figure with the same duration of the base class
    this->initControl();
    //this->getControlWidget()->setExtendedControl(m_widgetControl);
    this->updateData();
}

void PartialConstantTimeData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz") << " - PartialConstantTimeData::recalc() ---------------- " << this->name();
    //qreal * m_const=this->getSignalData();//get the pointer from super class. The base class values are all 0

    qint64 n_dw=this->lowestSampleIndexForModification();
    //qDebug() << "amplitude=" << this->amplitude() << "m_t0=" << this->startTime() << " m_min_t0=" << this->minStartTime() << " n_dw=" << n_dw << " NSample=" << this->sampleNumber();
    //Q_ASSERT( n_dw >=0);
    //Q_ASSERT(n_dw <=this->sampleNumber());

    qint64 n_up=this->highestSampleIndexForModification();
    //qDebug() << "m_max_Duration=" << this->maxDuration() <<" m_duration=" << this->duration()  << " n_up=" << n_up << " NSample=" << this->sampleNumber();
    //Q_ASSERT( n_up>=0 );
    //Q_ASSERT(n_up<=this->sampleNumber());
    //Q_ASSERT(n_dw<=n_up);

    for (qint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(this->insertSignalValue(n,this->amplitude()));
    }
}

void PartialConstantTimeData::setAmplitude(qreal amplitude) {
    m_amplitude=amplitude;
    //updating UI
    bool sigStatus=m_dataControl.sliderAmplitude->blockSignals(true);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->blockSignals(sigStatus);
    this->updateData();
}

void PartialConstantTimeData::initControl() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    m_widgetControl=new QFrame();
    QVBoxLayout * l=new QVBoxLayout();
    m_widgetControl->setLayout(l) ;
    m_widgetControl->hide();
    m_widgetControl->setFont(f);

    //set amplitude
    m_dataControl.sliderAmplitude=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_dataControl.sliderAmplitude->setScale(-1.0,1.0,0.01);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->setName("Amplitude");
    m_dataControl.sliderAmplitude->setMeasureUnit("-1 - +1");
    m_dataControl.sliderAmplitude->setFont(f);
    connect(m_dataControl.sliderAmplitude,SIGNAL(valueChanged(qreal)),this,SLOT(setAmplitude(qreal)));

    //Lay out all the control);
    l->addWidget(m_dataControl.sliderAmplitude,1,Qt::AlignLeft);
    //Control Widget from partialtimedata
    //l->addWidget(m_partialDataControl.widgetDuration,1,Qt::AlignLeft);
    //l->addWidget(m_partialDataControl.widget_t0,1,Qt::AlignLeft);
}

//-----------------------------
//Here Start ConstantTimeDate

ConstantTimeData::ConstantTimeData(qreal duration, qreal SRGen) :
    GenericTimeData(duration,SRGen)
{
    m_amplitude=CONSTDATA_DEFAULT_AMPLITUDE;
    //Init a figure with the same duration of the base class
    this->initControl();
    //this->getControlWidget()->setExtendedControl(m_widgetControl);
    this->updateData();
}

void ConstantTimeData::recalc() {
    qDebug()<< QTime::currentTime().toString("hh:mm:ss.zzz") << " - ConstantTimeData::recalc() ---------------- " << this->name();
    //qreal * m_const=this->getSignalData();//get the pointer from super class. The base class values are all 0

    qint64 n_dw=this->lowestSampleIndexForModification();
    //qDebug() << "amplitude=" << this->amplitude() << "m_t0=" << this->startTime() << " m_min_t0=" << this->minStartTime() << " n_dw=" << n_dw << " NSample=" << this->sampleNumber();
    //Q_ASSERT( n_dw >=0);
    //Q_ASSERT(n_dw <=this->sampleNumber());

    qint64 n_up=this->highestSampleIndexForModification();
    //qDebug() << "m_max_Duration=" << this->maxDuration() <<" m_duration=" << this->duration()  << " n_up=" << n_up << " NSample=" << this->sampleNumber();
    //Q_ASSERT( n_up>=0 );
    //Q_ASSERT(n_up<=this->sampleNumber());
    //Q_ASSERT(n_dw<=n_up);

    for (qint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(this->insertSignalValue(n,this->amplitude()));
    }
}

void ConstantTimeData::setAmplitude(qreal amplitude) {
    m_amplitude=amplitude;
    //updating UI
    bool sigStatus=m_dataControl.sliderAmplitude->blockSignals(true);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->blockSignals(sigStatus);
    this->updateData();
}

void ConstantTimeData::initControl() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    m_widgetControl=new QFrame();
    QVBoxLayout * l=new QVBoxLayout();
    m_widgetControl->setLayout(l) ;
    m_widgetControl->hide();
    m_widgetControl->setFont(f);

    //set amplitude
    m_dataControl.sliderAmplitude=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_dataControl.sliderAmplitude->setScale(-1.0,1.0,0.01);
    m_dataControl.sliderAmplitude->setValue(m_amplitude);
    m_dataControl.sliderAmplitude->setName("Amplitude");
    m_dataControl.sliderAmplitude->setMeasureUnit("-1 - +1");
    m_dataControl.sliderAmplitude->setFont(f);
    connect(m_dataControl.sliderAmplitude,SIGNAL(valueChanged(qreal)),this,SLOT(setAmplitude(qreal)));

    //Lay out all the control);
    l->addWidget(m_dataControl.sliderAmplitude,1,Qt::AlignLeft);
}
