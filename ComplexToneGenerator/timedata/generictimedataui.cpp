#include "generictimedataui.h"

GenericTimeDataUI::GenericTimeDataUI(GenericTimeData *gtd, QWidget *widget) :
    CustomCurveUI(widget),
    m_genericTimeData(gtd),
    m_TreeWidgetshowXML(0)
{
    this->initControlWidget();
    m_genericTimeData->getControlWidget()->addControlFrame(this,"GTD control");
}

void GenericTimeDataUI::updateXML() {
    const QDomDocument *_d=m_genericTimeData->getDomDocument();
    Q_ASSERT(_d!=NULL);
    Q_ASSERT(_d->isDocument());
    ReadAndWriteXML rwx;
    if (m_TreeWidgetshowXML==NULL) m_TreeWidgetshowXML=new QTreeWidget();
    m_TreeWidgetshowXML=rwx.parseXMLToQTreeWidget(_d,m_TreeWidgetshowXML);

}

void GenericTimeDataUI::showXML() {
    if (m_TreeWidgetshowXML==NULL) m_TreeWidgetshowXML=new QTreeWidget();

    if (m_TreeWidgetshowXML->isHidden() ) {
        m_TreeWidgetshowXML->show();
        m_TreeWidgetshowXML->setWindowTitle( QString("XML dump of %1").arg(m_genericTimeData->name()) );
        m_TreeWidgetshowXML->setGeometry(300,200,250,500);
        Qt::WindowFlags flags = m_TreeWidgetshowXML->windowFlags();
        m_TreeWidgetshowXML->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        m_TreeWidgetshowXML->show();
    }
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

    if (m_TreeWidgetshowXML) {
        updateXML();
    }
}

void GenericTimeDataUI::nameUpdated() {
    m_genericTimeData->setName(m_baseControl.lineName->text());
    if (!m_TreeWidgetshowXML->isHidden() ) {
        m_TreeWidgetshowXML->setWindowTitle( QString("XML dump of %1").arg(m_genericTimeData->name()) );
    }
}

void GenericTimeDataUI::initControlWidget() {
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget  layout
    QVBoxLayout * l=new QVBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
   // l->setSizeConstraint(QLayout::SetMaximumSize);
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

    m_baseControl.exportXML=new QPushButton("Export XML");
    m_baseControl.exportXML->setFont(f);
    connect(m_baseControl.exportXML ,SIGNAL(clicked()),m_genericTimeData,SLOT(exportXML()));

    m_baseControl.showXML=new QPushButton("Show XML");
    m_baseControl.showXML->setFont(f);
    connect(m_baseControl.showXML ,SIGNAL(clicked()),this,SLOT(showXML()));

    //Lay out all the controls
    l->addWidget(_nameLabel,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.lineName,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.checkBoxEnableCurve,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.checkBoxShowCurve,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.comboColor,1,Qt::AlignLeft);

    //Adding XML button
    QHBoxLayout * lh=new QHBoxLayout();
    lh->addWidget(m_baseControl.exportXML,1,Qt::AlignLeft);
    lh->addWidget(m_baseControl.showXML,1,Qt::AlignLeft);
    QWidget *buttonWidget=new QWidget(this);
    buttonWidget->setLayout((QLayout*)lh);
    l->addWidget(buttonWidget,1,Qt::AlignLeft);

    //update UI
    m_baseControl.lineName->setText(m_genericTimeData->name());
    m_baseControl.checkBoxShowCurve->setChecked(m_genericTimeData->isShowEnabled());
    m_baseControl.checkBoxEnableCurve->setChecked(m_genericTimeData->isEnabled());
    m_baseControl.comboColor->setColor(m_genericTimeData->color());

}


