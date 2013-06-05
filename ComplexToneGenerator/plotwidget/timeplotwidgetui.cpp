#include "timeplotwidgetui.h"

TimePlotWidgetUI::TimePlotWidgetUI(QWidget *widget) :
    DataUiHandlerUI(widget)
{
    this->initControlWidget();
}

TimePlotWidgetUI::~TimePlotWidgetUI() { }

void TimePlotWidgetUI::sampleRateUIUpdate(qreal sampleRate) {
    if (sampleRate!=m_baseControl.sliderSampleRate->value())
        m_baseControl.sliderSampleRate->setValue(sampleRate);
}

void TimePlotWidgetUI::maxDurationUIUpdate(qreal maxduration) {
    if (maxduration!=m_baseControl.sliderMaxDuration->value())
        m_baseControl.sliderMaxDuration->setValue(maxduration);
}

void TimePlotWidgetUI::initControlWidget() {

    QWidget * _widget=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget  layout
    QVBoxLayout * l=new QVBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    _widget->setLayout(l);
    _widget->setFont(f);

    //set Sample rate
    m_baseControl.sliderSampleRate = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_baseControl.sliderSampleRate->setDigitAccuracy(0);
    m_baseControl.sliderSampleRate->setScale(TIMEDATA_DEFAULT_MIN_SR,TIMEDATA_DEFAULT_MAX_SR,TIMEDATA_DEFAULT_STEP_SR);
    m_baseControl.sliderSampleRate->setName("SR Generation");
    m_baseControl.sliderSampleRate->setMeasureUnit("Hz");
    m_baseControl.sliderSampleRate->setFont(f);
    l->addWidget( m_baseControl.sliderSampleRate,1,Qt::AlignCenter|Qt::AlignTop);
    connect(m_baseControl.sliderSampleRate,SIGNAL(valueChanged(qreal)),this,SIGNAL(sampleRateUIChanged(qreal)));

    //set duration
    m_baseControl.sliderMaxDuration = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear);
    m_baseControl.sliderMaxDuration->setDigitAccuracy(3);
    m_baseControl.sliderMaxDuration->setScale(0,TIMEDATA_DEFAULT_MAX_TIME,TIMEDATA_DEFAULT_TIMESTEP);
    m_baseControl.sliderMaxDuration->setName("Duration Generated file");
    m_baseControl.sliderMaxDuration->setMeasureUnit("Sec.");
    m_baseControl.sliderMaxDuration->setFont(f);
    l->addWidget( m_baseControl.sliderMaxDuration,1, Qt::AlignCenter|Qt::AlignTop);
    connect(m_baseControl.sliderMaxDuration,SIGNAL(valueChanged(qreal)),this,SIGNAL(maxDurationUIChanged(qreal)));

    //add button show/hide all
    m_baseControl.showAllCurves=new QPushButton("Show all curves");
    m_baseControl.hideAllCurves=new QPushButton("Hide all curves");
    QHBoxLayout * _lh=new QHBoxLayout();
    _lh->addWidget(m_baseControl.showAllCurves,1,Qt::AlignCenter);
    _lh->addWidget(m_baseControl.hideAllCurves,1,Qt::AlignCenter);
    QWidget *_showHideWidget=new QWidget();
    _showHideWidget->setLayout((QLayout*)_lh);
    l->addWidget( _showHideWidget,1, Qt::AlignCenter|Qt::AlignTop);
    connect(m_baseControl.showAllCurves,SIGNAL(clicked()),this,SIGNAL(showAllCurvesToggled()));
    connect(m_baseControl.hideAllCurves,SIGNAL(clicked()),this,SIGNAL(hideAllCurvesToggled()));


    //Add the local widget to the framework widget
    this->addWidget(_widget, "Time Plot controls");
}

