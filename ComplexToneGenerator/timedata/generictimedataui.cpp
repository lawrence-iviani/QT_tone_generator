#include "generictimedataui.h"

GenericTimeDataUI::GenericTimeDataUI(GenericTimeData *gtd, QWidget *widget) :
    CustomCurveUI(widget),
    m_genericTimeData(gtd)
{
    this->initControlWidget();
    m_genericTimeData->getControlWidget()->addControlFrame(this,"GTD control");
}

void GenericTimeDataUI::updateControlUI() {
     qDebug() << "GenericTimeDataUI::updateControlUI called";

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


}

void GenericTimeDataUI::nameUpdated() {
    m_genericTimeData->setName(m_baseControl.lineName->text());
}

void GenericTimeDataUI::initControlWidget() {
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget  layout
    QVBoxLayout * l=new QVBoxLayout();
   // l->setSizeConstraint(QLayout::SetMinimumSize);
    l->setSizeConstraint(QLayout::SetMaximumSize);
    this->setLayout(l) ;
    this->setFont(f);

    //Name the curve
    QLabel * _nameLabel=new QLabel("Curve name",this);
    _nameLabel->setFont(f);
    _nameLabel->setAlignment(Qt::AlignCenter);

    m_baseControl.lineName= new QLineEdit(m_genericTimeData->name(),this);
    m_baseControl.lineName->setFont(f);


    connect( m_baseControl.lineName,SIGNAL(editingFinished()), this ,SLOT(nameUpdated()) );

    //Show curve
    m_baseControl.checkBoxShowCurve=new QCheckBox("Show curve",this);
    m_baseControl.checkBoxShowCurve->setFont(f);
    connect(m_baseControl.checkBoxShowCurve,SIGNAL(toggled(bool)),m_genericTimeData,SLOT(setShowCurve(bool)));

    //Enable curve
    m_baseControl.checkBoxEnableCurve=new QCheckBox("Enable curve",this);
    m_baseControl.checkBoxEnableCurve->setFont(f);
    connect(m_baseControl.checkBoxEnableCurve,SIGNAL(toggled(bool)),m_genericTimeData,SLOT(setEnableCurve(bool)) );

    //Curve color
    m_baseControl.comboColor=new ComboBoxWidgetColor();
    m_baseControl.comboColor->setFont(f);
    connect(m_baseControl.comboColor, SIGNAL(colorChanged(QColor)),m_genericTimeData,SLOT(setColor(QColor)) );

    //Lay out all the controls
    l->addWidget(_nameLabel,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.lineName,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.checkBoxEnableCurve,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.checkBoxShowCurve,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.comboColor,1,Qt::AlignLeft);

    //update UI
    m_baseControl.lineName->setText(m_genericTimeData->name());
    m_baseControl.checkBoxShowCurve->setChecked(m_genericTimeData->isShowEnabled());
    m_baseControl.checkBoxEnableCurve->setChecked(m_genericTimeData->isEnabled());
    m_baseControl.comboColor->setColor(m_genericTimeData->color());

}


