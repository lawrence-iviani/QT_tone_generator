#include "generictimedataui.h"

GenericTimeDataUI::GenericTimeDataUI(QWidget *widget) :
    DataUiHandlerUI(widget)
{
    this->initControlWidget();
 //   m_envelopeUI=new DataEnvelopeUI();
 //   addWidget(m_envelopeUI);
}

GenericTimeDataUI::~GenericTimeDataUI() {
 //    delete m_envelopeUI;
}


void GenericTimeDataUI::initControlWidget() {
    QWidget * _widget=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget  layout
    QVBoxLayout * l=new QVBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    _widget->setLayout(l);
    _widget->setFont(f);

    //Name the curve
    QLabel * _nameLabel=new QLabel("Curve name",this);
    _nameLabel->setFont(f);
    _nameLabel->setAlignment(Qt::AlignCenter);

    m_baseControl.lineName= new QLineEdit("m_genericTimeData->name()",this);
    m_baseControl.lineName->setFont(f);
    connect( m_baseControl.lineName,SIGNAL(editingFinished()), this ,SLOT(nameUpdated()));

    //Show curve
    m_baseControl.checkBoxShowCurve=new QCheckBox("Show curve",this);
    m_baseControl.checkBoxShowCurve->setFont(f);
    connect(m_baseControl.checkBoxShowCurve,SIGNAL(toggled(bool)),this,SIGNAL(showCurveUIChanged(bool)));

    //Enable curve
    m_baseControl.checkBoxEnableCurve=new QCheckBox("Enable curve",this);
    m_baseControl.checkBoxEnableCurve->setFont(f);
    connect(m_baseControl.checkBoxEnableCurve,SIGNAL(toggled(bool)),this,SIGNAL(enableCurveUIChanged(bool)));

    //Curve color
    m_baseControl.comboColor=new ComboBoxWidgetColor();
    m_baseControl.comboColor->setFont(f);
    connect(m_baseControl.comboColor, SIGNAL(colorChanged(QColor)),this,SIGNAL(colorUIChanged(QColor)));

    //XML Button
    m_baseControl.exportXML=new QPushButton("Export ...");
    m_baseControl.exportXML->setFont(f);
//    connect(m_baseControl.exportXML ,SIGNAL(clicked()),m_genericTimeData,SLOT(exportXML()));

    m_baseControl.importXML=new QPushButton("Import");
    m_baseControl.importXML->setFont(f);
  //  connect(m_baseControl.importXML ,SIGNAL(clicked()),m_genericTimeData,SLOT(importXML()));

    m_baseControl.showXML=new QPushButton("Show data");
    m_baseControl.showXML->setFont(f);
  //  connect(m_baseControl.showXML ,SIGNAL(clicked()),this,SLOT(showXML()));

    //C&P Button
    m_baseControl.copy=new QPushButton("Copy");
    m_baseControl.copy->setFont(f);
    //connect(m_baseControl.copy ,SIGNAL(clicked()),m_genericTimeData,SLOT(copy()));

    m_baseControl.paste=new QPushButton("Paste");
    m_baseControl.paste->setFont(f);
    //connect(m_baseControl.paste ,SIGNAL(clicked()),m_genericTimeData,SLOT(paste()));


    //Layouting XML button & C&P button
    QGridLayout * _lButton=new QGridLayout();
    _lButton->addWidget(m_baseControl.exportXML,0,0,1,1,Qt::AlignLeft);
    _lButton->addWidget(m_baseControl.importXML,0,1,1,1,Qt::AlignLeft);
    _lButton->addWidget(m_baseControl.showXML,0,2,1,1,Qt::AlignLeft);
    _lButton->addWidget(m_baseControl.copy,1,0,1,1,Qt::AlignLeft);
    _lButton->addWidget(m_baseControl.paste,1,1,1,1,Qt::AlignLeft);
    _lButton->setSizeConstraint(QLayout::SetMaximumSize);
    QWidget *_buttonWidget=new QWidget(this);
    _buttonWidget->setLayout((QLayout*)_lButton);

    //Lay out all the controls
    l->addWidget(_nameLabel,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.lineName,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.checkBoxEnableCurve,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.checkBoxShowCurve,1,Qt::AlignLeft);
    l->addWidget(m_baseControl.comboColor,1,Qt::AlignLeft);
    l->addWidget(_buttonWidget,1,Qt::AlignLeft);

    //Add the local widget to the framework widget
    this->addWidget(_widget, "Generic time Controls");
}

void GenericTimeDataUI::nameUpdated() {
    emit (nameUIChanged(m_baseControl.lineName->text()));
}

void GenericTimeDataUI::nameUIUpdate(QString name) {
    if (name!=m_baseControl.lineName->text())
        m_baseControl.lineName->setText(name);
}

void GenericTimeDataUI::colorUIUpdate(QColor color) {
    if (m_baseControl.comboColor->color()!=color)
        m_baseControl.comboColor->setColor(color);
}

void GenericTimeDataUI::showCurveUIUpdate(bool show){
    if (m_baseControl.checkBoxShowCurve->isChecked()!=show)
        m_baseControl.checkBoxShowCurve->setChecked(show);
}

void GenericTimeDataUI::enableCurveUIUpdate(bool enable) {
    if (m_baseControl.checkBoxEnableCurve->isChecked()!=enable) {
        m_baseControl.checkBoxEnableCurve->setChecked(enable);
        if(!enable)
            m_baseControl.checkBoxShowCurve->setDisabled(true);
        else
            m_baseControl.checkBoxShowCurve->setDisabled(false);
    }
}

//void GenericTimeDataUI::updateXML() {
//    const QDomDocument *_d=m_genericTimeData->getDomDocument();
//    Q_ASSERT(_d!=NULL);
//    Q_ASSERT(_d->isDocument());

//    if (m_TreeWidgetshowXML==NULL) {
//        m_TreeWidgetshowXML=new QTreeWidget();
//        m_TreeWidgetshowXML->setGeometry(300,200,300,400);
//        Qt::WindowFlags flags = m_TreeWidgetshowXML->windowFlags();
//        m_TreeWidgetshowXML->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
//    }
//    Q_ASSERT(DomHelper::parseDOMToQTreeWidget((DomHelper*)m_genericTimeData,m_TreeWidgetshowXML));

//    //expand all the tree if was visible before update
//    if (!m_TreeWidgetshowXML->isHidden()) m_TreeWidgetshowXML->expandAll();
//}

//void GenericTimeDataUI::showXML() {
//    if (m_TreeWidgetshowXML==NULL) {
//        m_TreeWidgetshowXML=new QTreeWidget();
//        m_TreeWidgetshowXML->setGeometry(300,200,300,400);
//        Qt::WindowFlags flags = m_TreeWidgetshowXML->windowFlags();
//        m_TreeWidgetshowXML->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
//    }

//    if (m_TreeWidgetshowXML->isHidden() ) {
//        m_TreeWidgetshowXML->setWindowTitle( QString("XML dump of %1").arg(m_genericTimeData->name()) );
//        m_TreeWidgetshowXML->show();
//        m_TreeWidgetshowXML->expandAll();
//    }
//    m_TreeWidgetshowXML->expandAll();
//}

//void GenericTimeDataUI::updateControlUI() {
//    //qDebug() << "GenericTimeDataUI::updateControlUI called";

//    bool prevSig=m_baseControl.lineName->blockSignals(true);
//    m_baseControl.lineName->setText(m_genericTimeData->name());
//    m_baseControl.lineName->blockSignals(prevSig);

//    prevSig=m_baseControl.comboColor->blockSignals(true);
//    m_baseControl.comboColor->setColor(m_genericTimeData->color());
//    m_baseControl.comboColor->blockSignals(prevSig);

//    prevSig=m_baseControl.checkBoxEnableCurve->blockSignals(true);
//    m_baseControl.checkBoxEnableCurve->setChecked(m_genericTimeData->isEnabled());
//    m_baseControl.checkBoxEnableCurve->blockSignals(prevSig);

//    prevSig=m_baseControl.checkBoxShowCurve->blockSignals(true);
//    m_baseControl.checkBoxShowCurve->setDisabled(!m_genericTimeData->isEnabled());
//    m_baseControl.checkBoxShowCurve->setChecked(m_genericTimeData->isShowEnabled());
//    m_baseControl.checkBoxShowCurve->blockSignals(prevSig);

//    emit (controlUIUpdated());
//}
