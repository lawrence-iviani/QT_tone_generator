#include "generictimedata.h"

GenericTimeData::GenericTimeData()
{
    m_SR=TIMEDATA_DEFAULT_SR;
    m_MaxDuration=TIMEDATA_DEFAULT_MAX_TIME;
    m_Min_t0=TIMEDATA_DEFAULT_MIN_TIME;
    m_t=NULL;
    m_s=NULL;
    m_curve=NULL;
    m_data=NULL;
    m_sample=0;
    m_curveEnabled=true;
    m_name=QString("no name");
    m_curve=new QwtPlotCurve(m_name);
    m_curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve->setPaintAttribute(QwtPlotCurve::ClipPolygons);
    m_allControl=NULL;
    this->createBaseControl();
    //this->updateData();
}

GenericTimeData::GenericTimeData(double maxDuration, double SRGen)
{   
    m_SR=SRGen;
    m_MaxDuration=maxDuration;
    m_Min_t0=TIMEDATA_DEFAULT_MIN_TIME;
    m_t=NULL;
    m_s=NULL;
    m_curve=NULL;
    m_data=NULL;
    m_sample=0;
    m_curveEnabled=true;
    m_name=QString("no name");
    m_curve=new QwtPlotCurve(m_name);
    m_curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve->setPaintAttribute(QwtPlotCurve::ClipPolygons);
    m_allControl=NULL;
    this->createBaseControl();
    //this->updateData();
}

GenericTimeData::~GenericTimeData() {
    this->deleteAllData();
    if (m_curve!=NULL) {
        free(m_curve);
    }
    free(m_data);
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


    m_sample=floor(m_MaxDuration*m_SR);//number of sample
    double SL=1/m_SR; //Sample length

    m_t=new double[m_sample];
    m_s=new double[m_sample];

    for (int n=0; n < m_sample ; n++) {
        m_t[n]=this->minStartTime()+n*SL;
        m_s[n]=0;
    }
}

void GenericTimeData::createDataCurve() {
    Q_ASSERT(m_curve!=NULL);
    m_data=new QwtCPointerData(m_t,m_s,m_sample);
    m_curve->setData(m_data);//m_data is freed in the setData, see QWT documentation regarding QwtPlotCurve class beahvior
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

//void GenericTimeData::setMinStartTimeAndMaxDuration(double t0, double duration) {
//    m_Min_t0=t0;
//    if (duration < 0) {
//        m_MaxDuration=0;
//    } else {
//        m_MaxDuration=duration;
//    }
//    m_Max_t1=m_MaxDuration+m_Min_t0;
//    emit maxDurationUpdate(m_MaxDuration);
//    this->updateData();
//}

//void GenericTimeData::setMinStartTime(double t0) {
//    if (t0 > m_Max_t1 ) {
//        m_Min_t0=m_Max_t1;
//    } else {
//        m_Min_t0=t0;
//    }
//    m_Max_t1=m_MaxDuration+m_Min_t0;
//    emit minStartTimeUpdate(m_Min_t0);
//    this->updateData();
//}

void GenericTimeData::setMaxDurationAndUpdate(double  maxDuration, bool updateData) {
    if (maxDuration < 0) {
        m_MaxDuration=0;
    } else {
        m_MaxDuration=maxDuration;
    }
    // m_Max_t1=m_MaxDuration+m_Min_t0;
    if (updateData) this->updateData();
}

void GenericTimeData::setMaxDuration(double  maxDuration) {
    this->setMaxDurationAndUpdate(maxDuration,true);
}

void GenericTimeData::setSampleRate(double SR) {
    this->setSampleRateAndUpdate(SR,true);
}

void GenericTimeData::setSampleRateAndUpdate(double SR, bool updateData) {
    m_SR=SR;
    if (updateData) this->updateData();
}

//void GenericTimeData::setMaxEndTime(double  t1) {
//    if (t1 < m_Min_t0) {
//        m_Max_t1=m_Min_t0;
//    } else {
//        m_Max_t1=t1;
//    }
//     m_MaxDuration=m_Max_t1-m_Min_t0;
//     emit maxEndTimeUpdate(m_MaxDuration);
//     this->updateData();
// }

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
    this->m_curveEnabled=enable;
    bool prevSig=m_baseControl.checkBoxShowCurve->blockSignals(true);
    m_baseControl.checkBoxShowCurve->setDisabled(!enable);
    m_baseControl.checkBoxShowCurve->setChecked(enable);
    m_baseControl.checkBoxShowCurve->blockSignals(prevSig);
    this->setShowCurve(enable);
}

void GenericTimeData::setShowCurve(bool enable) {
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
    m_allControl=new QFrame();
    m_allControl->setFrameShape(QFrame::WinPanel);
    m_allControl->setFrameShadow(QFrame::Raised);
    m_allControl->setLayout(new QVBoxLayout());
    QVBoxLayout * l=(QVBoxLayout*) m_allControl->layout();

    m_baseControl.toggleButtonOptionControl=new QPushButton("Base control");
    m_baseControl.toggleButtonOptionControl->setCheckable(true);
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
    connect(m_extendedControl.toggleButtonOptionControl,SIGNAL(clicked(bool)),this,SLOT(showExtendedControl()));

    //Adding widget
    m_extendedControl.extendedWidget=(QFrame*)extendedWidget;
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
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    m_baseControl.baseWidget=new QFrame();
    m_baseControl.baseWidget->setFrameShape(QFrame::WinPanel);
    m_baseControl.baseWidget->setFrameShadow(QFrame::Raised);
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

    //Show curve
    m_baseControl.checkBoxShowCurve=new QCheckBox("Show curve",NULL);
    m_baseControl.checkBoxShowCurve->setChecked(true);
    m_baseControl.checkBoxShowCurve->setFont(f);
    connect(m_baseControl.checkBoxShowCurve,SIGNAL(toggled(bool)),this,SLOT(setShowCurve(bool)));

    //Enable curve
    m_baseControl.checkBoxEnableCurve=new QCheckBox("Enable curve",NULL);
    m_baseControl.checkBoxEnableCurve->setChecked(true);
    m_baseControl.checkBoxEnableCurve->setFont(f);
    connect(m_baseControl.checkBoxEnableCurve,SIGNAL(toggled(bool)),this,SLOT(setEnableCurve(bool)) );

    //Curve color
    m_baseControl.comboColor=new ComboBoxWidgetColor(NULL);
    m_baseControl.comboColor->setColor(this->color());
    m_baseControl.comboColor->setFont(f);
    connect(m_baseControl.comboColor, SIGNAL(colorChanged(QColor)),this,SLOT(setColor(QColor)) );

    //Lay out all the controls
    l->addWidget(_nameLabel,1,Qt::AlignCenter);
    l->addWidget(m_baseControl.lineName,1,Qt::AlignCenter);
    l->addWidget(m_baseControl.checkBoxEnableCurve,1,Qt::AlignCenter);
    l->addWidget(m_baseControl.checkBoxShowCurve,1,Qt::AlignCenter);
    l->addWidget(m_baseControl.comboColor,1,Qt::AlignCenter);
}


