#include "generictimedata.h"

GenericTimeData::GenericTimeData()
{
    m_duration=0;
    m_t0=0;
    m_t1=m_t0+m_duration;
    m_SR=48000;
    m_t=NULL;
    m_s=NULL;
    m_curve=NULL;
    m_data=NULL;
    m_sample=0;
    m_name=QString("no name");
    m_curve=new QwtPlotCurve(m_name);
    m_curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve->setPaintAttribute(QwtPlotCurve::ClipPolygons);
    m_allControl=NULL;
    this->createBaseControl();
    this->updateData();
}

GenericTimeData::GenericTimeData(double t0, double duration, double SRGen)
{
    m_t0=t0;
    m_duration=duration;
    m_t1=m_t0+m_duration;
    m_SR=SRGen;
    m_t=NULL;
    m_s=NULL;
    m_curve=NULL;
    m_data=NULL;
    m_sample=0;
    m_name=QString("no name");
    m_curve=new QwtPlotCurve(m_name);
    m_curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve->setPaintAttribute(QwtPlotCurve::ClipPolygons);
    m_allControl=NULL;
    this->createBaseControl();
    this->updateData();
}

GenericTimeData::~GenericTimeData() {
    this->deleteAllData();
    if (m_curve!=NULL) {
        free(m_curve);
    }
    free(m_data);
}

void GenericTimeData::setSampleRate(double SR) {
    m_SR=SR;
    this->updateData();
}

void GenericTimeData::updateData() {
    this->resetAllData();
    recalc();
    this->createDataCurve();
    emit dataUpdated();
}

void GenericTimeData::resetAllData() {
    this->deleteAllData();
    Q_ASSERT(m_t==NULL);
    Q_ASSERT(m_s==NULL);


    m_sample=floor(m_duration*m_SR);//number of sample
    double SL=1/m_SR; //Sample length

    m_t=new double[m_sample];
    m_s=new double[m_sample];
    for (int n=0; n < m_sample ; n++) {
        m_t[n]=m_t0+n*SL;
        m_s[n]=0;
    }
}

void GenericTimeData::createDataCurve() {
    Q_ASSERT(m_curve!=NULL);
    m_data=new QwtCPointerData(m_t,m_s,m_sample);
    m_curve->setData(m_data);
}

void GenericTimeData::deleteAllData() {
    if (m_t!=NULL) {
        delete[] m_t;
        m_t=NULL;
    }
    if (m_s!=NULL) {
        delete[] m_s;
        m_s=NULL;
    }
}

void GenericTimeData::setStartTimeAndDuration(double t0, double duration) {
    m_t0=t0;
    if (duration < 0) {
        m_duration=0;
    } else {
        m_duration=duration;
    }
    m_t1=m_duration+m_t0;
    this->updateData();
}

void GenericTimeData::setStartTime(double t0) {
    m_t0=t0;
    m_t1=m_duration+m_t0;
    this->updateData();
}

void GenericTimeData::setDuration(double  duration) {
    if (duration < 0) {
        m_duration=0;
    } else {
        m_duration=duration;
    }
     m_t1=m_duration+m_t0;
     this->updateData();
 }

void GenericTimeData::setEndTime(double  t1) {
    if (t1 < m_t0) {
        m_t1=m_t0;
    } else {
        m_t1=t1;
    }
     m_duration=m_t1-m_t0;
     this->updateData();
 }

void GenericTimeData::setTimeData(double * t, long int len){
    Q_ASSERT(len==m_sample);
    if (m_t!=NULL) {
        delete[] m_t;
        m_t=NULL;
    }
    m_t=t;
}

void GenericTimeData::setSignalData(double * s, long int len){
    Q_ASSERT(len==m_sample);
    if (m_s!=NULL) {
        delete[] m_s;
        m_s=NULL;
    }
    m_s=s;
}

void GenericTimeData::nameUpdated() {
    this->setName( m_baseControl.lineName->text());

}

void GenericTimeData::setName(QString name) {
    if (m_name!=name) {
        m_name=name;
        m_curve->setTitle(m_name);
        m_baseControl.lineName->setText(m_name);
        emit(dataUpdated());
    }
}

void GenericTimeData::setEnableCurve(bool enable) {
    if (enable) {
        m_curve->show();
    } else {
        m_curve->hide();
    }
    emit(dataUpdated());
}

void GenericTimeData::setColor(QColor color) {
    if (ComboBoxWidgetColor::isSupportedColorName(color)) {
        QPen  p=m_curve->pen();
        p.setColor(color);
        this->m_baseControl.comboColor->setColor(color);
        m_curve->setPen(p);
        emit(dataUpdated());
    }
}

void GenericTimeData::showBaseControl() {
    Q_ASSERT(m_baseControl.baseWidget!=NULL);
    if (m_baseControl.toggleButtonOptionControl->isChecked()) {
        m_baseControl.baseWidget->show();
    } else {
        m_baseControl.baseWidget->hide();
    }
}

void GenericTimeData::showExtendedControl() {
    Q_ASSERT(m_extendedControl.extendedWidget!=NULL);
    if (m_extendedControl.toggleButtonOptionControl->isChecked()) {
        m_extendedControl.extendedWidget->show();
    } else {
        m_extendedControl.extendedWidget->hide();
    }
}

void GenericTimeData::createBaseControl() {
    m_allControl=new QWidget();
    m_allControl->setLayout(new QVBoxLayout());
    QVBoxLayout * l=(QVBoxLayout*) m_allControl->layout();

    m_baseControl.toggleButtonOptionControl=new QPushButton("Base control");
    m_baseControl.toggleButtonOptionControl->setCheckable(true);
   // m_baseControl.toggleButtonOptionEnable->setMaximumWidth( (2.0/3.0)*CONTROL_WIDTH);
   // m_baseControl.toggleButtonOptionEnable->setMinimumWidth( (2.0/3.0)*CONTROL_WIDTH);
   // m_baseControl.toggleButtonOptionEnable->setSizePolicy(QSizePolicy::Maximum ,QSizePolicy::Maximum);
    connect(m_baseControl.toggleButtonOptionControl,SIGNAL(clicked(bool)),this,SLOT(showBaseControl()));
    l->addWidget(m_baseControl.toggleButtonOptionControl,1);

    //Adding base control
    this->initBaseControlWidget();
    l->addWidget(m_baseControl.baseWidget,1);

    this->showBaseControl();
}

void GenericTimeData::setExtendedControl(QWidget * extendedWidget) {
    QVBoxLayout * l=(QVBoxLayout *)m_allControl->layout();

    //Create extended control button
    m_extendedControl.toggleButtonOptionControl=new QPushButton("Extended control");
    m_extendedControl.toggleButtonOptionControl->setCheckable(true);
   // m_extendedControl.toggleButtonOptionEnable->setMaximumWidth( (2.0/3.0)*CONTROL_WIDTH);
   // m_extendedControl.toggleButtonOptionEnable->setMinimumWidth( (2.0/3.0)*CONTROL_WIDTH);
   // m_extendedControl.toggleButtonOptionEnable->setSizePolicy(QSizePolicy::Maximum ,QSizePolicy::Maximum);
    connect(m_extendedControl.toggleButtonOptionControl,SIGNAL(clicked(bool)),this,SLOT(showExtendedControl()));

    //Adding widget
    m_extendedControl.extendedWidget=extendedWidget;
    if (m_extendedControl.extendedWidget!=NULL) {
        //Add button
        l->addWidget(m_extendedControl.toggleButtonOptionControl,1);//,Qt::AlignLeft);
        //Add control
        l->addWidget(m_extendedControl.extendedWidget,1);//,Qt::AlignLeft);
        this->showExtendedControl();
    }

}

void GenericTimeData::initBaseControlWidget() {

    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(BASE_SIZE);

    //Widget container and layout
    m_baseControl.baseWidget=new QWidget();
    QVBoxLayout * l=new QVBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    m_baseControl.baseWidget->setLayout(l) ;
    m_baseControl.baseWidget->hide();
    m_baseControl.baseWidget->setFont(f);

    //Name the curve
    QLabel * _nameLabel=new QLabel("Curve name",NULL);
    _nameLabel->setFont(f);
    _nameLabel->setAlignment(Qt::AlignCenter);
    m_baseControl.lineName= new QLineEdit(m_name, NULL);
    m_baseControl.lineName->setFont(f);
    connect( m_baseControl.lineName,SIGNAL(editingFinished()) ,this,SLOT(nameUpdated()) );

    //Enable curve
    m_baseControl.checkBoxEnableCurve=new QCheckBox("Enable curve",NULL);
    m_baseControl.checkBoxEnableCurve->setChecked(true);
    m_baseControl.checkBoxEnableCurve->setFont(f);
    //m_baseControl.checkBoxEnableCurve->setMaximumWidth( CONTROL_WIDTH);
    //m_baseControl.checkBoxEnableCurve->setMinimumWidth( CONTROL_WIDTH/2);
    //m_baseControl.checkBoxEnableCurve->setSizePolicy(QSizePolicy::Maximum ,QSizePolicy::Maximum);
    connect(m_baseControl.checkBoxEnableCurve,SIGNAL(toggled(bool)),this,SLOT(setEnableCurve(bool)));

    //Curve color
    m_baseControl.comboColor=new ComboBoxWidgetColor(NULL);
    m_baseControl.comboColor->setColor(this->color());
    m_baseControl.comboColor->setFont(f);
    //m_baseControl.comboColor->setMaximumWidth( CONTROL_WIDTH);
    //m_baseControl.comboColor->setMinimumWidth( CONTROL_WIDTH/2);
    //m_baseControl.comboColor->setSizePolicy(QSizePolicy::Maximum ,QSizePolicy::Maximum);
    connect(m_baseControl.comboColor, SIGNAL(colorChanged(QColor)),this,SLOT(setColor(QColor)) );

    //set duration
    m_baseControl.slider_duration = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_baseControl.slider_duration->setScale(0,10,0.1);
    m_baseControl.slider_duration->setValue(m_duration);
    m_baseControl.slider_duration->setName("Duration");
    m_baseControl.slider_duration->setMeasureUnit("Sec.");
    m_baseControl.slider_duration->setFont(f);
    //m_baseControl.slider_duration->setMaximumWidth( CONTROL_WIDTH);
    //m_baseControl.slider_duration->setMinimumWidth( CONTROL_WIDTH/2);
    //m_baseControl.slider_duration->setSizePolicy(QSizePolicy::Maximum ,QSizePolicy::Maximum);
    //m_baseControl.slider_duration->setSizePolicy(QSizePolicy::Minimum ,QSizePolicy::Minimum);
    connect(m_baseControl.slider_duration,SIGNAL(valueChanged(double)),this,SLOT(setDuration(double)));

    //set t0
    m_baseControl.slider_t0 = new ScaledSliderWidget(NULL, Qt::Vertical,ScaledSlider::Linear) ;
    m_baseControl.slider_t0->setScale(-10,+10,0.1);
    m_baseControl.slider_t0->setValue(m_t0);
    m_baseControl.slider_t0->setName("Start Time");
    m_baseControl.slider_t0->setMeasureUnit("Sec.");
    m_baseControl.slider_t0->setFont(f);
    //m_baseControl.slider_t0->setMaximumWidth( CONTROL_WIDTH);
    //m_baseControl.slider_t0->setMinimumWidth( CONTROL_WIDTH/2);
    //m_baseControl.slider_t0->setSizePolicy(QSizePolicy::Maximum ,QSizePolicy::Maximum);
    connect(m_baseControl.slider_t0,SIGNAL(valueChanged(double)),this,SLOT(setStartTime(double)));

    //Lay out all the controls
    l->addWidget(_nameLabel,1,Qt::AlignCenter);
    l->addWidget(m_baseControl.lineName,1,Qt::AlignCenter);
    l->addWidget(m_baseControl.checkBoxEnableCurve,1,Qt::AlignCenter);
    l->addWidget(m_baseControl.comboColor,1,Qt::AlignCenter);
    l->addWidget(m_baseControl.slider_duration,1,Qt::AlignCenter);
    l->addWidget(m_baseControl.slider_t0,1,Qt::AlignCenter);

}


