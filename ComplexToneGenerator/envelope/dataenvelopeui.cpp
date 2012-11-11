#include "dataenvelopeui.h"
#include "ui_dataenvelopeui.h"

DataEnvelopeUI::DataEnvelopeUI(QWidget *parent) :
    CustomCurveUI(parent),
    ui(new Ui::DataEnvelopeUI)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    m_parameters=NULL;
    this->initAmplitudeWidget();
    this->initTimeWidget();
}

DataEnvelopeUI::DataEnvelopeUI(DataEnvelopeParameters *params , QWidget *parent) :
    CustomCurveUI(parent),
    ui(new Ui::DataEnvelopeUI),
    m_parameters(params)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    this->initEnvelopeWidget();
    this->initAmplitudeWidget();
    this->initTimeWidget();
}

DataEnvelopeUI::~DataEnvelopeUI()
{
    delete ui;
}

void DataEnvelopeUI::initAmplitudeWidget() {
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    ui->widgetAmplitude->setLayout(ui->hlayoutAmplitude);

    //Setting up hold control widget
    m_structAmplitude.hold=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_structAmplitude.hold->setScale(DataEnvelopeParameters::DATAENVELOPE_LOWERBOUND_AMPLITUDE,
                                     DataEnvelopeParameters::DATAENVELOPE_UPPERBOUND_AMPLITUDE,
                                     (DataEnvelopeParameters::DATAENVELOPE_LOWERBOUND_AMPLITUDE-DataEnvelopeParameters::DATAENVELOPE_UPPERBOUND_AMPLITUDE)/100.0);
    m_structAmplitude.hold->setName("Hold amplitude");
    m_structAmplitude.hold->setMeasureUnit("");
    m_structAmplitude.hold->setFont(f);
    connect(m_structAmplitude.hold,SIGNAL(valueChanged(qreal)),this,SLOT(setHoldAmplitude(qreal)));

    //Setting up sustain control widget
    m_structAmplitude.sustain=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_structAmplitude.sustain->setScale(DataEnvelopeParameters::DATAENVELOPE_LOWERBOUND_AMPLITUDE,
                                     DataEnvelopeParameters::DATAENVELOPE_UPPERBOUND_AMPLITUDE,
                                     (DataEnvelopeParameters::DATAENVELOPE_LOWERBOUND_AMPLITUDE-DataEnvelopeParameters::DATAENVELOPE_UPPERBOUND_AMPLITUDE)/100.0);
    m_structAmplitude.sustain->setName("Sustain amplitude");
    m_structAmplitude.sustain->setMeasureUnit("");
    m_structAmplitude.sustain->setFont(f);
    connect(m_structAmplitude.sustain,SIGNAL(valueChanged(qreal)),this,SLOT(setSustainAmplitude(qreal)));

    //Layouting
    ui->hlayoutAmplitude->addWidget(m_structAmplitude.hold,1,Qt::AlignLeft);
    ui->hlayoutAmplitude->addWidget(m_structAmplitude.sustain,1,Qt::AlignLeft);

    if (m_parameters!=NULL) {
        setSliderValue(m_structAmplitude.hold,m_parameters->holdLevel());
        setSliderValue(m_structAmplitude.sustain,m_parameters->sustainLevel());
    }
}

void DataEnvelopeUI::initTimeWidget() {
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    ui->widgetTime->setLayout(ui->hlayoutTime );

    //Setting up attack time control widget
    m_structTime.attack=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_structTime.attack->setName("Attack time");
    m_structTime.attack->setMeasureUnit("msec");
    m_structTime.attack->setFont(f);
    connect(m_structTime.attack,SIGNAL(valueChanged(qreal)),this,SLOT(setAttackTime(qreal)) );


    //Setting up attack time control widget
    m_structTime.hold=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_structTime.hold->setName("Hold time");
    m_structTime.hold->setMeasureUnit("msec");
    m_structTime.hold->setFont(f);
    connect(m_structTime.hold,SIGNAL(valueChanged(qreal)),this,SLOT(setHoldTime(qreal)) );

    //Setting up Decay time control widget
    m_structTime.decay=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_structTime.decay->setName("Decay time");
    m_structTime.decay->setMeasureUnit("msec");
    m_structTime.decay->setFont(f);
    connect(m_structTime.decay,SIGNAL(valueChanged(qreal)),this,SLOT(setDecayTime(qreal)) );

    //Setting up Sustain time control widget
    m_structTime.sustain=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_structTime.sustain->setName("Sustain time");
    m_structTime.sustain->setMeasureUnit("msec");
    m_structTime.sustain->setFont(f);
    connect(m_structTime.sustain,SIGNAL(valueChanged(qreal)),this,SLOT(setSustainTime(qreal)) );

    //Setting up Sustain time control widget
    m_structTime.release=new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_structTime.release->setName("Release time");
    m_structTime.release->setMeasureUnit("msec");
    m_structTime.release->setFont(f);
    connect(m_structTime.release,SIGNAL(valueChanged(qreal)),this,SLOT(setReleaseTime(qreal)) );

    //Layouting
    ui->hlayoutTime->addWidget(m_structTime.attack,1,Qt::AlignLeft);
    ui->hlayoutTime->addWidget(m_structTime.hold,1,Qt::AlignLeft);
    ui->hlayoutTime->addWidget(m_structTime.decay,1,Qt::AlignLeft);
    ui->hlayoutTime->addWidget(m_structTime.sustain,1,Qt::AlignLeft);
    ui->hlayoutTime->addWidget(m_structTime.release,1,Qt::AlignLeft);

    //setting init values
    if (m_parameters!=NULL) {
        setTimeSlider(m_structTime.attack, m_parameters->attack());
        setTimeSlider(m_structTime.hold, m_parameters->hold());
        setTimeSlider(m_structTime.decay, m_parameters->decay());
        setTimeSlider(m_structTime.sustain, m_parameters->sustain());
        setTimeSlider(m_structTime.release, m_parameters->release());
    }
}

void DataEnvelopeUI::initEnvelopeWidget() {
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Enable curve
    ui->checkBoxEnableEnvelope->setFont(f);
    connect(ui->checkBoxEnableEnvelope ,SIGNAL(toggled(bool)), m_parameters,SLOT(setEnableEnvelope(bool)));

    if (m_parameters!=NULL) {
        this->setEnableEnvelopeUI(m_parameters->isEnableEnvelope());
    }

}

void DataEnvelopeUI::setEnableEnvelopeUI(bool enable) {
    ui->checkBoxEnableEnvelope->setChecked(enable);
    if (enable) {
        ui->widgetTime->show();
        ui->widgetAmplitude->show();
    } else {
        ui->widgetTime->hide();
        ui->widgetAmplitude->hide();
    }
}

void DataEnvelopeUI::setHoldAmplitude(qreal holdAmplitude) {
    if (holdAmplitude==m_structAmplitude.hold->value() && m_parameters==NULL) return;
    m_parameters->setLevelParameters(holdAmplitude, m_parameters->sustainLevel() );
}

void DataEnvelopeUI::setSustainAmplitude(qreal sustainAmplitude) {
    if (sustainAmplitude==m_structAmplitude.sustain->value() && m_parameters==NULL) return;
    m_parameters->setLevelParameters(m_parameters->holdLevel(), sustainAmplitude );
}

void DataEnvelopeUI::setAttackTime(qreal attackTime) {
    if (attackTime==m_structTime.attack->value() && m_parameters==NULL) return;
    m_parameters->setTimeParameters(m_structTime.attack->value(),m_structTime.hold->value(),m_structTime.decay->value(),m_structTime.sustain->value(),m_structTime.release->value());
}

void DataEnvelopeUI::setHoldTime(qreal holdTime){
    if (holdTime==m_structTime.hold->value() && m_parameters==NULL) return;
    m_parameters->setTimeParameters(m_structTime.attack->value(),m_structTime.hold->value(),m_structTime.decay->value(),m_structTime.sustain->value(),m_structTime.release->value());
}

void DataEnvelopeUI::setDecayTime(qreal decayTime) {
    if (decayTime==m_structTime.decay->value() && m_parameters==NULL) return;
    m_parameters->setTimeParameters(m_structTime.attack->value(),m_structTime.hold->value(),m_structTime.decay->value(),m_structTime.sustain->value(),m_structTime.release->value());
}

void DataEnvelopeUI::setSustainTime(qreal sustainTime) {
    if (sustainTime==m_structTime.sustain->value() && m_parameters==NULL) return;
    m_parameters->setTimeParameters(m_structTime.attack->value(),m_structTime.hold->value(),m_structTime.decay->value(),m_structTime.sustain->value(),m_structTime.release->value());
}

void DataEnvelopeUI::setReleaseTime(qreal releaseTime) {
    if (releaseTime==m_structTime.release->value() && m_parameters==NULL) return;
    m_parameters->setTimeParameters(m_structTime.attack->value(),m_structTime.hold->value(),m_structTime.decay->value(),m_structTime.sustain->value(),m_structTime.release->value());
}

void DataEnvelopeUI::updateUI() {
    updateControlUI();
}

void DataEnvelopeUI::updateControlUI() {
    qDebug() << "DataEnvelopeUI::updateControlUI called";

    //Enable
    bool sig=ui->checkBoxEnableEnvelope->blockSignals(true);
    this->setEnableEnvelopeUI(m_parameters->isEnableEnvelope());
    ui->checkBoxEnableEnvelope->blockSignals(sig);

    //Set amplitude
    setSliderValue(m_structAmplitude.hold,m_parameters->holdLevel());
    setSliderValue(m_structAmplitude.sustain,m_parameters->sustainLevel());

    //qreal debugAttack= m_parameters->attack();
    //Second, set the percentile
    setTimeSlider(m_structTime.attack,m_parameters->attack());
    setTimeSlider(m_structTime.hold,m_parameters->hold());
    setTimeSlider(m_structTime.decay,m_parameters->decay());
    setTimeSlider(m_structTime.sustain,m_parameters->sustain());
    setTimeSlider(m_structTime.release,m_parameters->release());
}

void DataEnvelopeUI::setTimeSlider(ScaledSliderWidget *slider, qreal val) {
    //Setting scale
    qreal settedTime=m_parameters->attack() + m_parameters->hold() + m_parameters->decay() + m_parameters->sustain() + m_parameters->release();
    qreal remainingTime=m_parameters->total()-settedTime;
//    qDebug() << "DataEnvelopeUI::setTimeSlider slider@" <<slider << "setTime="<<settedTime<<"remainingTime=" << remainingTime << " totalTime="<< m_parameters->total() ;

    Q_ASSERT( remainingTime>=0.0);
    Q_ASSERT( remainingTime<=m_parameters->total());
    m_structTime.attack->setScale(0,m_parameters->attack()+remainingTime,DATAENVELOPEUI_TIME_STEP);
    m_structTime.hold->setScale(0,m_parameters->hold()+remainingTime,DATAENVELOPEUI_TIME_STEP);
    m_structTime.decay->setScale(0,m_parameters->decay() +remainingTime,DATAENVELOPEUI_TIME_STEP);
    m_structTime.sustain->setScale(0,m_parameters->sustain()+remainingTime,DATAENVELOPEUI_TIME_STEP);
    m_structTime.release->setScale(0,m_parameters->release()+remainingTime,DATAENVELOPEUI_TIME_STEP);

    setSliderValue(slider,val);
}

void DataEnvelopeUI::setSliderValue(ScaledSliderWidget *slider, qreal val) {
    if (slider==NULL ) return;
//    qDebug() << "DataEnvelopeUI::setSliderValue slider@" <<slider << " value=" <<val;
    bool sig=slider->blockSignals(true);
    slider->setValue(((qreal)qFloor(val*100.0))/100.0);
    slider->blockSignals(sig);
}

bool DataEnvelopeUI::isEnabledEnvelopeUI()
{return ui->checkBoxEnableEnvelope->isChecked();}
