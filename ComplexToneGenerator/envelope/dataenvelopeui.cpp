#include "dataenvelopeui.h"


DataEnvelopeUI::DataEnvelopeUI(QWidget *parent) :
    DataUiHandlerUI(parent)
{
    this->initEnableWidget();
    this->initAmplitudeWidget();
    this->initTimeWidget();
    this->initEnvelopeWidget();
}

DataEnvelopeUI::~DataEnvelopeUI()
{
    delete m_widgetAmplitude;
    delete m_widgetTime;
    delete m_widgetEnable;
}

void DataEnvelopeUI::initEnableWidget() {
    m_widgetEnable=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget  layout
    QHBoxLayout * _l=new QHBoxLayout();
    _l->setSizeConstraint(QLayout::SetMinimumSize);
    m_widgetEnable->setLayout(_l);
    m_widgetEnable->setFont(f);

    //Setting up hold control widget
    m_structEnable.enableCB=new QCheckBox("Enable/disable envelope");
    m_structEnable.enableCB->setFont(f);
    connect(m_structEnable.enableCB,SIGNAL(clicked()),this,SLOT(enableCheckBoxToggled()));

    _l->addWidget(m_structEnable.enableCB,1,Qt::AlignLeft);
}

void DataEnvelopeUI::initAmplitudeWidget() {
    m_widgetAmplitude=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget  layout
    QHBoxLayout * _l=new QHBoxLayout();
    _l->setSizeConstraint(QLayout::SetMinimumSize);
    m_widgetAmplitude->setLayout(_l);
    m_widgetAmplitude->setFont(f);

    //Setting up hold control widget
    m_structAmplitude.hold=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_structAmplitude.hold->setScale(DATAENVELOPE_LOWERBOUND_AMPLITUDE,
                                     DATAENVELOPE_UPPERBOUND_AMPLITUDE,
                                     (DATAENVELOPE_LOWERBOUND_AMPLITUDE-DATAENVELOPE_UPPERBOUND_AMPLITUDE)/100.0);
    m_structAmplitude.hold->setName("Hold amplitude");
    m_structAmplitude.hold->setMeasureUnit("");
    m_structAmplitude.hold->setFont(f);
    Q_ASSERT(connect(m_structAmplitude.hold,SIGNAL(valueChanged(qreal)),this,SIGNAL(holdLevelUIChanged(qreal))));

    //Setting up sustain control widget
    m_structAmplitude.sustain=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_structAmplitude.sustain->setScale(DATAENVELOPE_LOWERBOUND_AMPLITUDE,
                                     DATAENVELOPE_UPPERBOUND_AMPLITUDE,
                                     (DATAENVELOPE_LOWERBOUND_AMPLITUDE-DATAENVELOPE_UPPERBOUND_AMPLITUDE)/100.0);
    m_structAmplitude.sustain->setName("Sustain amplitude");
    m_structAmplitude.sustain->setMeasureUnit("");
    m_structAmplitude.sustain->setFont(f);
    Q_ASSERT(connect(m_structAmplitude.sustain,SIGNAL(valueChanged(qreal)),this,SIGNAL(sustainLevelUIChanged(qreal))));

    //Layouting
    _l->addWidget(m_structAmplitude.hold,1,Qt::AlignLeft);
    _l->addWidget(m_structAmplitude.sustain,1,Qt::AlignLeft);

    //Disable to start
    m_widgetAmplitude->setEnabled(false);
}


void DataEnvelopeUI::initTimeWidget() {
    m_widgetTime=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget  layout
    QHBoxLayout * l=new QHBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    m_widgetTime->setLayout(l);
    m_widgetTime->setFont(f);

    //Setting up attack time control widget
    m_structTime.attack=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_structTime.attack->setDigitAccuracy(DATAENVELOPE_DIGIT_TIME_ACCURACY);
    m_structTime.attack->setName("Attack time");
    m_structTime.attack->setMeasureUnit("sec.");
    m_structTime.attack->setFont(f);
    Q_ASSERT(connect(m_structTime.attack,SIGNAL(valueChanged(qreal)),this,SIGNAL(attackDurationUIChanged(qreal)) ));


    //Setting up attack time control widget
    m_structTime.hold=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_structTime.hold->setDigitAccuracy(DATAENVELOPE_DIGIT_TIME_ACCURACY);
    m_structTime.hold->setName("Hold time");
    m_structTime.hold->setMeasureUnit("sec.");
    m_structTime.hold->setFont(f);
    Q_ASSERT(connect(m_structTime.hold,SIGNAL(valueChanged(qreal)),this,SIGNAL(holdDurationUIChanged(qreal)) ));

    //Setting up Decay time control widget
    m_structTime.decay=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_structTime.decay->setDigitAccuracy(DATAENVELOPE_DIGIT_TIME_ACCURACY);
    m_structTime.decay->setName("Decay time");
    m_structTime.decay->setMeasureUnit("sec.");
    m_structTime.decay->setFont(f);
    Q_ASSERT(connect(m_structTime.decay,SIGNAL(valueChanged(qreal)),this,SIGNAL(decayDurationUIChanged(qreal)) ));

    //Setting up Sustain time control widget
    m_structTime.sustain=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_structTime.sustain->setDigitAccuracy(DATAENVELOPE_DIGIT_TIME_ACCURACY);
    m_structTime.sustain->setName("Sustain time");
    m_structTime.sustain->setMeasureUnit("sec.");
    m_structTime.sustain->setFont(f);
    Q_ASSERT(connect(m_structTime.sustain,SIGNAL(valueChanged(qreal)),this,SIGNAL(sustainDurationUIChanged(qreal)) ));

    //Setting up Sustain time control widget
    m_structTime.release=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_structTime.release->setDigitAccuracy(DATAENVELOPE_DIGIT_TIME_ACCURACY);
    m_structTime.release->setName("Release time");
    m_structTime.release->setMeasureUnit("sec.");
    m_structTime.release->setFont(f);
    Q_ASSERT(connect(m_structTime.release,SIGNAL(valueChanged(qreal)),this,SIGNAL(releaseDurationUIChanged(qreal)) ));

    //Layouting
    l->addWidget(m_structTime.attack,1,Qt::AlignLeft);
    l->addWidget(m_structTime.hold,1,Qt::AlignLeft);
    l->addWidget(m_structTime.decay,1,Qt::AlignLeft);
    l->addWidget(m_structTime.sustain,1,Qt::AlignLeft);
    l->addWidget(m_structTime.release,1,Qt::AlignLeft);

    //disable to start
    m_widgetTime->setEnabled(false);
}

void DataEnvelopeUI::initEnvelopeWidget() {
    QWidget * _widget=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget  layout
    QVBoxLayout * _l=new QVBoxLayout();
    _l->setSizeConstraint(QLayout::SetMinimumSize);
    _widget->setLayout(_l);
    _widget->setFont(f);

    _l->addWidget(m_widgetEnable);
    _l->addWidget(m_widgetAmplitude);
    _l->addWidget(m_widgetTime);

    this->addWidget(_widget, "Envelope Controls");
}

void DataEnvelopeUI::enableUIUpdate(bool enable) {
    if (enable!=m_structEnable.enableCB->isChecked())
            m_structEnable.enableCB->setChecked(enable);
    else return;

    m_widgetAmplitude->setEnabled(enable);
    m_widgetTime->setEnabled(enable);
}

void DataEnvelopeUI::holdLevelUIUpdate(qreal holdAmplitude) {
    if (holdAmplitude==m_structAmplitude.hold->value()) return;
    m_structAmplitude.hold->setValue(holdAmplitude);
}

void DataEnvelopeUI::sustainLevelUIUpdate(qreal sustainAmplitude) {
    if (sustainAmplitude==m_structAmplitude.sustain->value()) return;
    m_structAmplitude.sustain->setValue(sustainAmplitude);
}

void DataEnvelopeUI::attackDurationUIUpdate(qreal attackTime) {
    if (attackTime==m_structTime.attack->value()) return;
    m_structTime.attack->setValue(attackTime);
}

void DataEnvelopeUI::holdDurationUIUpdate(qreal holdTime){
    if (holdTime==m_structTime.hold->value()) return;
    m_structTime.hold->setValue(holdTime);
}

void DataEnvelopeUI::decayDurationUIUpdate(qreal decayTime) {
    if (decayTime==m_structTime.decay->value()) return;
    m_structTime.decay->setValue(decayTime);
}

void DataEnvelopeUI::sustainDurationUIUpdate(qreal sustainTime) {
    if (sustainTime==m_structTime.sustain->value()) return;
    m_structTime.sustain->setValue(sustainTime);
}

void DataEnvelopeUI::releaseDurationUIUpdate(qreal releaseTime) {
    if (releaseTime==m_structTime.release->value()) return;
    m_structTime.release->setValue(releaseTime);
}

void DataEnvelopeUI::setTimeSlider(ScaledSliderWidget *slider, qreal val) {
    setSliderValue(slider,val);
}

void DataEnvelopeUI::setSliderValue(ScaledSliderWidget *slider, qreal val) {
    if (slider==NULL ) return;
//    qDebug() << "DataEnvelopeUI::setSliderValue slider@" <<slider << " value=" <<val;
    bool sig=slider->blockSignals(true);
    slider->setValue(((qreal)qFloor(val*(1.0/DATAENVELOPEUI_TIME_STEP)))/(1.0/DATAENVELOPEUI_TIME_STEP));
    slider->blockSignals(sig);
}

void DataEnvelopeUI::enableCheckBoxToggled() {
    bool enable=m_structEnable.enableCB->isChecked();
    m_widgetAmplitude->setEnabled(enable);
    m_widgetTime->setEnabled(enable);
    emit (enableUIChanged(enable));
}

void DataEnvelopeUI::setParamsTime(DataEnvelopeParameters *params) {
    qreal _spareTime=params->spareTimeAvailable();

    //Setting value
    m_structTime.attack->setValue(params->attack());
    m_structTime.hold->setValue(params->hold());
    m_structTime.decay->setValue(params->decay());
    m_structTime.sustain->setValue(params->sustain());
    m_structTime.release->setValue(params->release());

    //setting scale
    m_structTime.attack->setScale(0,params->attack()+_spareTime,TIMEDATA_DEFAULT_TIMESTEP);
    m_structTime.hold->setScale(0,params->hold()+_spareTime,TIMEDATA_DEFAULT_TIMESTEP);
    m_structTime.decay->setScale(0,params->decay()+_spareTime,TIMEDATA_DEFAULT_TIMESTEP);
    m_structTime.sustain->setScale(0,params->sustain()+_spareTime,TIMEDATA_DEFAULT_TIMESTEP);
    m_structTime.release->setScale(0,params->release()+_spareTime,TIMEDATA_DEFAULT_TIMESTEP);

}
