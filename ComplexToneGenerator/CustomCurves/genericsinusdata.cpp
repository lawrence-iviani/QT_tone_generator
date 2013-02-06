#include "genericsinusdata.h"

//---------- PARAMETERS ----------

GenericSinusParams::GenericSinusParams(QObject * parent) : //,TimePlotParams* params, QObject *parent) :
    GenericTimeDataParams(parent),
    m_amplitude(SINUSDATA_DEFAULT_AMPLITUDE),
    m_frequency(SINUSDATA_DEFAULT_FREQUENCY),
    m_initPhase(SINUSDATA_DEFAULT_INITPHASE)
{}

GenericSinusParams::GenericSinusParams(GenericTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent) :
    GenericTimeDataParams(baseProperty,params,parent),
    m_amplitude(SINUSDATA_DEFAULT_AMPLITUDE),
    m_frequency(SINUSDATA_DEFAULT_FREQUENCY),
    m_initPhase(SINUSDATA_DEFAULT_INITPHASE)
{}

GenericSinusParams::GenericSinusParams(GenericTimeDataParams * baseProperty,TimePlotParams* params, QObject *parent, qreal amplitude, qreal frequency, qreal initPhase):
    GenericTimeDataParams(baseProperty,params,parent),
    m_amplitude(amplitude),
    m_frequency(frequency),
    m_initPhase(initPhase)
{}

void GenericSinusParams::setAmplitude(qreal amplitude) {
    if (m_amplitude!=amplitude) {
        m_amplitude=amplitude;
        emit(amplitudeChanged(amplitude));
    }
}

void GenericSinusParams::setFrequency(qreal frequency) {
    if (m_frequency!=frequency) {
        m_frequency=frequency;
        emit(frequencyChanged(frequency));
    }
}

void GenericSinusParams::setInitPhase(qreal initPhase) {
    if (m_initPhase!=initPhase) {
        m_initPhase=initPhase;
        emit(initPhaseChanged(initPhase));
    }
}

//---------- UI ----------
GenericSinusUI::GenericSinusUI(QWidget *widget ) :
    GenericTimeDataUI(widget)
{
    this->initControlWidget();
}

void GenericSinusUI::initControlWidget() {
    QWidget * _widget=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    QHBoxLayout * l=new QHBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    _widget->setLayout(l);
    _widget->setFont(f);

    //set frequency
    m_sinusDataControl.sliderFrequency= new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Logarithmic) ;
    m_sinusDataControl.sliderFrequency->setScale(10,22000,0.001);
    m_sinusDataControl.sliderFrequency->setName("Freq.");
    m_sinusDataControl.sliderFrequency->setMeasureUnit("Sec.");
    m_sinusDataControl.sliderFrequency->setFont(f);
    connect(m_sinusDataControl.sliderFrequency,SIGNAL(valueChanged(qreal)),this,SIGNAL(frequencyUIChanged(qreal)));

    //set amplitude
    m_sinusDataControl.sliderAmplitude=new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Linear) ;
    m_sinusDataControl.sliderAmplitude->setScale(0,1.0,0.01);
    m_sinusDataControl.sliderAmplitude->setName("Amplitude");
    m_sinusDataControl.sliderAmplitude->setMeasureUnit("0-1");
    m_sinusDataControl.sliderAmplitude->setFont(f);
    connect(m_sinusDataControl.sliderAmplitude,SIGNAL(valueChanged(qreal)),this,SIGNAL(amplitudeUIChanged(qreal)));


    //set init phase
    m_sinusDataControl.sliderInitPhase=new ScaledSliderWidget(this, Qt::Vertical,ScaledSlider::Linear) ;
    m_sinusDataControl.sliderInitPhase->setScale(-180,180,1);
    m_sinusDataControl.sliderInitPhase->setName("Phase");
    m_sinusDataControl.sliderInitPhase->setMeasureUnit("deg.");
    m_sinusDataControl.sliderInitPhase->setFont(f);
    connect(m_sinusDataControl.sliderInitPhase,SIGNAL(valueChanged(qreal)),this,SIGNAL(initPhaseUIChanged(qreal)));

    //Lay out all the control);
    l->addWidget(m_sinusDataControl.sliderFrequency,1,Qt::AlignLeft);
    l->addWidget(m_sinusDataControl.sliderAmplitude,1,Qt::AlignLeft);
    l->addWidget(m_sinusDataControl.sliderInitPhase,1,Qt::AlignLeft);

    this->addWidget(_widget, "Generic tone controls");
}

void GenericSinusUI::amplitudeUIUpdate(qreal amplitude) {
    if (amplitude!=m_sinusDataControl.sliderAmplitude->value())
        m_sinusDataControl.sliderAmplitude->setValue(amplitude);
}

void GenericSinusUI::frequencyUIUpdate(qreal frequency) {
    if (frequency!=m_sinusDataControl.sliderFrequency->value())
        m_sinusDataControl.sliderFrequency->setValue(frequency);
}

void GenericSinusUI::initPhaseUIUpdate(qreal initphase) {
    if (initphase!=m_sinusDataControl.sliderInitPhase->value())
        m_sinusDataControl.sliderInitPhase->setValue(initphase);
}


//---------- FRONTEND ----------
GenericSinusData::GenericSinusData(QObject * parent) :
    GenericTimeData (parent)
{
    init(SINUSDATA_DEFAULT_AMPLITUDE,SINUSDATA_DEFAULT_FREQUENCY,SINUSDATA_DEFAULT_INITPHASE);
}

GenericSinusData::GenericSinusData(TimePlotParams * timePlotParams, QObject * parent) :
    GenericTimeData(timePlotParams,parent)
{
    init(SINUSDATA_DEFAULT_AMPLITUDE,SINUSDATA_DEFAULT_FREQUENCY,SINUSDATA_DEFAULT_INITPHASE);
}

GenericSinusData::GenericSinusData(TimePlotParams * timePlotParams, qreal amplitude, qreal frequency, qreal initPhase , QObject * parent) :
    GenericTimeData(timePlotParams,parent)
{
    init(amplitude,frequency,initPhase);
}

void GenericSinusData::init(qreal amplitude, qreal frequency, qreal initPhase) {
    GenericSinusParams* _derivedProp=new GenericSinusParams((QObject*)this);
    getDelegate()->replacePropertiesAndUI(dynamic_cast<DataUiHandlerProperty*>(_derivedProp),
                                      dynamic_cast<DataUiHandlerUI*> (new GenericSinusUI() ));

    //Set any eventual default parameters or passed by the constructor argmuents
    GenericSinusParams* _gsp=dynamic_cast<GenericSinusParams*>(getDataParameters());
    Q_ASSERT(_gsp);
    _gsp->setAmplitude(amplitude);
    _gsp->setFrequency(frequency);
    _gsp->setInitPhase(initPhase);
    connectSignals();
}

void GenericSinusData::connectSignals() {
    GenericTimeData::connectSignals();

    GenericSinusParams* _gsp=dynamic_cast<GenericSinusParams*>(getDataParameters());
    Q_ASSERT(_gsp);
    Q_ASSERT(connect(_gsp,SIGNAL(amplitudeChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_gsp,SIGNAL(frequencyChanged(qreal)),this,SLOT(updateData())));
    Q_ASSERT(connect(_gsp,SIGNAL(initPhaseChanged(qreal)),this,SLOT(updateData())));
}

void GenericSinusData::recalc() {
    GenericSinusParams* _gsp=dynamic_cast<GenericSinusParams*>(getDataParameters());
    Q_ASSERT(_gsp);

    //Verify going to recalc with the enable flag true
    Q_ASSERT(isEnableRecalc());

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ RECALC %1 with SR=%2-----")
                        .arg(_gsp->name())
                        .arg(_gsp->sampleRate()));

    //Getting the minimum and maximum value where recalculate the data
    quint64 n_dw=this->lowestSampleIndexForModification();
    quint64 n_up=this->highestSampleIndexForModification();
    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"------ indexlow=%1/%2 indexhi=%3/%2")
                        .arg(n_dw)
                        .arg(getSampleNumber())
                        .arg(n_up));

    //Gettine time base
    const qreal *t=this->getTimeData();

    //Getting params
    qreal _ampl=_gsp->amplitude();
    qreal _freq=_gsp->frequency();
    qreal _phase=M_RADIANS(_gsp->initPhase());
    for (quint64 n=n_dw; n < n_up; n++) {
        Q_ASSERT(insertSignalValue(n,_ampl*sin(2*M_PI*_freq*t[n]+_phase)));
    }
}


