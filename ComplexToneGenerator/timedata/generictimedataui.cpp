#include "generictimedataui.h"

GenericTimeDataUI::GenericTimeDataUI(GenericTimeData *gtd, QWidget *widget) :
    CustomCurveUI(widget),
    m_genericTimeData(gtd)
{
    this->createBaseControl();
}


void GenericTimeDataUI::createBaseControl() {

    //Setting layout, widget, etc. for the widget GenricTimeDataUI class
    this->setLayout(new QVBoxLayout());

    //init the controls related to this class
    this->initBaseControlWidget();

    //Adding to layout
    m_genericTimeData->getControlWidget()->addControlFrame(m_baseControl.controlFrame,"GTD control");

}

//void GenericTimeDataUI::setExtendedControl(CustomCurveUI *customCurveControl) {
//    QVBoxLayout * l=(QVBoxLayout *)this->layout();

//    //Adding extended control widget
//    m_customCurveControl=(CustomCurveUI*)customCurveControl;
//    if (m_customCurveControl!=NULL) {
//        //Add control
//        l->addWidget(m_customCurveControl,1);//,Qt::AlignLeft);
//        m_customCurveControl->show();
//    }
//}

void GenericTimeDataUI::updateControlUI() {

    bool prevSig=m_baseControl.lineName->blockSignals(true);
    m_baseControl.lineName->setText(m_genericTimeData->name());
    m_baseControl.lineName->blockSignals(prevSig);

    prevSig=m_baseControl.comboColor->blockSignals(true);
    m_baseControl.comboColor->setColor(m_genericTimeData->color());
    m_baseControl.comboColor->blockSignals(prevSig);

    prevSig=m_baseControl.checkBoxEnableCurve->blockSignals(true);
    m_baseControl.checkBoxEnableCurve->setChecked(m_genericTimeData->isEnabled());
    m_baseControl.checkBoxEnableCurve->blockSignals(prevSig);

    prevSig=m_baseControl.checkBoxShowCurve->blockSignals(true);
    m_baseControl.checkBoxShowCurve->setDisabled(!m_genericTimeData->isEnabled());
    m_baseControl.checkBoxShowCurve->setChecked(m_genericTimeData->isShowEnabled());
    m_baseControl.checkBoxShowCurve->blockSignals(prevSig);

    prevSig=m_baseControl.checkBoxEnableEnvelope->blockSignals(true);
    m_baseControl.checkBoxEnableEnvelope->setChecked(m_genericTimeData->isEnvelopeEnabled());
    m_genericTimeData->isEnvelopeEnabled() ? m_baseControl.envelopeWidget->show() : m_baseControl.envelopeWidget->hide();
    m_baseControl.checkBoxEnableEnvelope->blockSignals(prevSig);
}

void GenericTimeDataUI::nameUpdated() {
    m_genericTimeData->setName(m_baseControl.lineName->text());
}

//void GenericTimeDataUI::addControlFrame(QFrame *controlFrame,const QString name) {

//    QVBoxLayout * l=(QVBoxLayout *)this->layout();
//    //Adding base control
//    if (controlFrame!=NULL) {
//        //Add control
//        l->addWidget(controlFrame,1);//,Qt::AlignLeft);
//        controlFrame->show();
//    }
//}

void GenericTimeDataUI::initBaseControlWidget() {
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout
    m_baseControl.controlFrame=new CustomCurveUI();
    QVBoxLayout * l=new QVBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    m_baseControl.controlFrame->setLayout(l) ;
    m_baseControl.controlFrame->setFont(f);


    //Name the curve
    QLabel * _nameLabel=new QLabel("Curve name",this);
    _nameLabel->setFont(f);
    _nameLabel->setAlignment(Qt::AlignCenter);
    m_baseControl.lineName= new QLineEdit(m_genericTimeData->name(), this);
    m_baseControl.lineName->setFont(f);
    connect( m_baseControl.lineName,SIGNAL(editingFinished()), this ,SLOT(nameUpdated()) );

    //Show curve
    m_baseControl.checkBoxShowCurve=new QCheckBox("Show curve",this);
    m_baseControl.checkBoxShowCurve->setChecked(true);
    m_baseControl.checkBoxShowCurve->setFont(f);
    connect(m_baseControl.checkBoxShowCurve,SIGNAL(toggled(bool)),m_genericTimeData,SLOT(setShowCurve(bool)));

    //Enable curve
    m_baseControl.checkBoxEnableCurve=new QCheckBox("Enable curve",this);
    m_baseControl.checkBoxEnableCurve->setChecked(true);
    m_baseControl.checkBoxEnableCurve->setFont(f);
    connect(m_baseControl.checkBoxEnableCurve,SIGNAL(toggled(bool)),m_genericTimeData,SLOT(setEnableCurve(bool)) );

    //Curve color
    m_baseControl.comboColor=new ComboBoxWidgetColor(this);
    m_baseControl.comboColor->setColor(m_genericTimeData->color());
    m_baseControl.comboColor->setFont(f);
    connect(m_baseControl.comboColor, SIGNAL(colorChanged(QColor)),m_genericTimeData,SLOT(setColor(QColor)) );

    //Enable curve
    m_baseControl.checkBoxEnableEnvelope=new QCheckBox("Enable envelope",this);
    m_baseControl.checkBoxEnableEnvelope->setChecked(false);
    m_baseControl.checkBoxEnableEnvelope->setFont(f);
    connect(m_baseControl.checkBoxEnableEnvelope,SIGNAL(toggled(bool)),m_genericTimeData,SLOT(setEnableEnvelope(bool)) );

    //Getting the envelope control
    m_baseControl.envelopeWidget=m_genericTimeData->getEnvelopeData()->getEnvelopeUI();
    m_baseControl.envelopeWidget->setParent(this);
    m_baseControl.envelopeWidget->hide();


    //Lay out all the controls
    l->addWidget(_nameLabel,1,Qt::AlignCenter);
    l->addWidget(m_baseControl.lineName,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.checkBoxEnableCurve,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.checkBoxShowCurve,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.comboColor,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.checkBoxEnableEnvelope,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.envelopeWidget,1);
}


