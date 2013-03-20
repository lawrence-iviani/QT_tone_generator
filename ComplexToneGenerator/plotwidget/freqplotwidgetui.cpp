#include "freqplotwidgetui.h"

FreqPlotWidgetUI::FreqPlotWidgetUI(QWidget *widget) :
    DataUiHandlerUI(widget)
{
    initControlWidget();
}

FreqPlotWidgetUI::~FreqPlotWidgetUI() { }

void FreqPlotWidgetUI::initControlWidget() {

    QWidget * _widget=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont _font;//=*(new QFont());
    _font.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget  layout
    QVBoxLayout * l=new QVBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    _widget->setLayout(l);
    _widget->setFont(_font);

    //Adding the ZMP button control
//    m_baseControl.ZMP_RadioButton.groupBox = new QGroupBox("Mouse Selection",NULL);
//    m_baseControl.ZMP_RadioButton.PICKER =  new QRadioButton("Picker",(QWidget*)m_baseControl.ZMP_RadioButton.groupBox);
//    m_baseControl.ZMP_RadioButton.PAN = new QRadioButton("Pan",(QWidget*)m_baseControl.ZMP_RadioButton.groupBox);
//    m_baseControl.ZMP_RadioButton.ZOOM = new QRadioButton("Zoom",(QWidget*)m_baseControl.ZMP_RadioButton.groupBox);
//    QVBoxLayout * radioButtonLayout = new QVBoxLayout();
//    radioButtonLayout->addWidget(m_baseControl.ZMP_RadioButton.PICKER,1);
//    radioButtonLayout->addWidget(m_baseControl.ZMP_RadioButton.PAN,1);
//    radioButtonLayout->addWidget(m_baseControl.ZMP_RadioButton.ZOOM,1);
//    m_baseControl.ZMP_RadioButton.groupBox->setLayout(radioButtonLayout);
//    connect(m_baseControl.ZMP_RadioButton.PICKER,SIGNAL(clicked()),this,SLOT(ZMP_statusChanged()));
//    connect(m_baseControl.ZMP_RadioButton.PAN,SIGNAL(clicked()),this,SLOT(ZMP_statusChanged()));
//    connect(m_baseControl.ZMP_RadioButton.ZOOM,SIGNAL(clicked()),this,SLOT(ZMP_statusChanged()));
//    m_baseControl.ZMP_RadioButton.PICKER->setChecked(true);
//    l->addWidget( m_baseControl.ZMP_RadioButton.groupBox,1,Qt::AlignLeft|Qt::AlignTop);

    //set bins
    m_baseControl.binsNuber = new QComboBox(NULL);
    QStringList binsItems;
    m_baseControl.binsNuber->addItems(FREQPLOTWIDGET_BINS(binsItems));
    l->addWidget( createCombo("Bins\nper window",m_baseControl.binsNuber,_font),1,Qt::AlignCenter|Qt::AlignTop);
    Q_ASSERT(connect(m_baseControl.binsNuber,SIGNAL(currentIndexChanged(QString)),this,SLOT(binsHasChanged(QString))));

    //OVERLAP
    m_baseControl.overlap = new QComboBox(NULL);
    QStringList overlapItems;
    m_baseControl.overlap->addItems(FREQPLOTWIDGET_OVERLAP(overlapItems));
    l->addWidget(createCombo("Window\noverlap",m_baseControl.overlap,_font),1,Qt::AlignCenter|Qt::AlignTop);
    Q_ASSERT(connect(m_baseControl.overlap,SIGNAL(currentIndexChanged(QString)),this,SLOT(overlapHasChanged(QString))));

    //set window
    m_baseControl.windowType = new QComboBox(NULL);
    QStringList windowItems;
    m_baseControl.windowType->addItems(FREQPLOTWIDGET_WINDOWTYPE(windowItems));
    l->addWidget( createCombo("Windows",m_baseControl.windowType,_font),1,Qt::AlignCenter|Qt::AlignTop);
    Q_ASSERT(connect(m_baseControl.windowType,SIGNAL(currentIndexChanged(QString)),this,SIGNAL(windowTypeUIChanged(QString))));

    //Add the local widget to the framework widget
    this->addWidget(_widget, "Spectrogram Plot controls");
}

void FreqPlotWidgetUI::binsHasChanged(QString nbins) {
    bool ok;
    uint n=nbins.toUInt(&ok);
    if (ok) emit binsNumberUIChanged(n);
}

void FreqPlotWidgetUI::overlapHasChanged(QString overlap){
    float value;
    sscanf(overlap.toUtf8().data(),"%f %%",&value);
    emit overlapUIChanged(((qreal)value));
}

void FreqPlotWidgetUI::binsNumberUIUpdate(uint nBins) {
    QString _snBins=QString(QString::number(nBins));
    if (_snBins!=m_baseControl.binsNuber->currentText())
        m_baseControl.binsNuber->setCurrentIndex(m_baseControl.binsNuber->findText(_snBins));
}

void FreqPlotWidgetUI::overlapUIUpdate(qreal overlap) {
    QString _sOverlap=QString(QString::number(overlap)).append(" %");
    if (_sOverlap!=m_baseControl.overlap->currentText())
        m_baseControl.overlap->setCurrentIndex( m_baseControl.overlap->findText(_sOverlap));
}

void FreqPlotWidgetUI::windowTypeUIUpdate(QString windowName) {
    if (windowName!=m_baseControl.windowType->currentText())
        m_baseControl.windowType->setCurrentIndex( m_baseControl.windowType->findText(windowName));
}

QWidget* FreqPlotWidgetUI::createCombo(QString name,QComboBox *combo, QFont& font) {
    QWidget* _retval=new QWidget();
    QHBoxLayout* _l=new QHBoxLayout(_retval);
    _l->addWidget(combo,1,Qt::AlignLeft);
    QLabel*  _label=new QLabel(name);
    _l->addWidget(_label,1,Qt::AlignLeft);

    combo->setMinimumContentsLength(10);
    combo->setFont(font);
    _label->setFont(font);
    return _retval;
}
