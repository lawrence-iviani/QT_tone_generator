#include "zmp_helper.h"


ZMP_helper::ZMP_helper(PlotWidget *parentPlot) :
    QObject((QObject*)parentPlot),
    m_parentPlot(parentPlot),
    ZMP_controlWidget(NULL)
{
    Q_ASSERT(m_parentPlot);
    this->init();
    this->initZMPControlWdiget();

}

void ZMP_helper::init() {
    QwtPlotCanvas* _canvas=  m_parentPlot->canvas();

    // zoom in/out with the wheel
    m_magnifier=new QwtPlotMagnifier(_canvas);

    // panning with the left mouse button
    m_pan=new QwtPlotPanner(_canvas);

    // zoom in/out with the wheel
    m_zoom=new QwtPlotZoomer (_canvas);//QwtPlot::xTop , QwtPlot::yLeft,canvas);
    m_zoom->setRubberBand(QwtPicker::RectRubberBand);
   // changeZoomBase(m_parentPlot->getXAxisInterval(),m_parentPlot->getYAxisInterval());
  // QRectF _rect(0.0,-1.0,TIMEDATA_DEFAULT_PROJECT_TIME-TIMEDATA_DEFAULT_MIN_TIME,2.0);
  //  changeZoomBase(_rect);
    changeZoomBase(m_parentPlot->getXAxisInterval(),m_parentPlot->getYAxisInterval());
    //Setting Picker cross with a click
    m_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, _canvas);
    m_picker->setStateMachine(new QwtPickerDragPointMachine());
    m_picker->setRubberBand(QwtPicker::CrossRubberBand);

    //set color
    this->setBaseColor(QColor(Qt::black));

    this->disableAll();
}

void ZMP_helper::changeZoomBase(const QwtInterval& x, const QwtInterval& y) {
    qDebug() << "x.minValue()="<<x.minValue() <<" -y.maxValue()=" << -y.maxValue()<<" x.maxValue()-x.minValue()=" << x.maxValue()-x.minValue()<< " y.maxValue()-y.minValue()=" <<y.maxValue()-y.minValue();
    QRectF _rect(x.minValue(),-y.maxValue(),x.maxValue()-x.minValue(),(y.maxValue()-y.minValue()));
    changeZoomBase(_rect);
}

//Found this function here
//http://comments.gmane.org/gmane.comp.graphics.qwt.general/3501
void ZMP_helper::changeZoomBase(const QRectF& rect)
{
    if(m_zoom->zoomBase() == rect) return ;
    QStack<QRectF> stack = m_zoom->zoomStack() ; // get stack
    stack.remove(0) ; // remove old base
    // check stack items if they are within new base:
    for(QStack<QRectF>::size_type i = 0 ; i < stack.size() ; i++)
        if (! rect.contains(stack[i]))
    // and remove if they are not:
            stack.remove(i--) ;
    stack.prepend(rect) ;
    // set new base
    // put stack in place and try to find current zoomRect in it
    // (set index to current's index in new stack or top of stack
    // if the current rectangle is not in the new stack)
    m_zoom->setZoomStack(stack, stack.indexOf(m_zoom->zoomRect())) ;
}

void ZMP_helper::setBaseColor(QColor color) {
    m_picker->setRubberBandPen(color);
    m_picker->setTrackerPen(color);
    m_zoom->setRubberBandPen(color);
    m_zoom->setTrackerPen(color);
}

void ZMP_helper::disableAll() {
    m_zoom->setEnabled(false);
    m_pan->setEnabled(false);
    m_magnifier->setEnabled(false);
    m_picker->setEnabled(false);
}

void ZMP_helper::enableZoomer() {
    this->disableAll();
   // changeZoomBase(m_parentPlot->getXAxisInterval(),m_parentPlot->getYAxisInterval());
    m_zoom->setEnabled(true);
}

void ZMP_helper::enablePanner() {
    this->disableAll();
    m_pan->setEnabled(true);
    m_magnifier->setEnabled(true);
}

void ZMP_helper::enablePicker() {
    this->disableAll();
    m_picker->setEnabled(true);
    m_magnifier->setEnabled(true);
}

void ZMP_helper::initZMPControlWdiget() {

    ZMP_controlWidget=new QWidget();//Create the widget for these controls
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget  layout
    QVBoxLayout * l=new QVBoxLayout();
    l->setSizeConstraint(QLayout::SetMinimumSize);
    ZMP_controlWidget->setLayout(l);
    ZMP_controlWidget->setFont(f);

    //Adding the ZMP button control
    ZMP_RadioButton.groupBox = new QGroupBox("Mouse Selection",NULL);
    ZMP_RadioButton.PICKER =  new QRadioButton("Picker",(QWidget*)ZMP_RadioButton.groupBox);
    ZMP_RadioButton.PAN = new QRadioButton("Pan",(QWidget*)ZMP_RadioButton.groupBox);
    ZMP_RadioButton.ZOOM = new QRadioButton("Zoom",(QWidget*)ZMP_RadioButton.groupBox);
    QVBoxLayout * radioButtonLayout = new QVBoxLayout();
    radioButtonLayout->addWidget(ZMP_RadioButton.PICKER,1);
    radioButtonLayout->addWidget(ZMP_RadioButton.PAN,1);
    radioButtonLayout->addWidget(ZMP_RadioButton.ZOOM,1);
    ZMP_RadioButton.groupBox->setLayout(radioButtonLayout);
    connect(ZMP_RadioButton.PICKER,SIGNAL(clicked()),this,SLOT(enablePicker()));
    connect(ZMP_RadioButton.PAN,SIGNAL(clicked()),this,SLOT(enablePanner()));
    connect(ZMP_RadioButton.ZOOM,SIGNAL(clicked()),this,SLOT(enableZoomer()));
    ZMP_RadioButton.PICKER->setChecked(true);
    l->addWidget( ZMP_RadioButton.groupBox,1,Qt::AlignLeft|Qt::AlignTop);
}
