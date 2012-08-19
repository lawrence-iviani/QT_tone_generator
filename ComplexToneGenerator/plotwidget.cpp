#include "plotwidget.h"



PlotWidget::PlotWidget(QWidget *parent, int xScaleType, int yScaleType) :
    QwtPlot(parent)
{
    m_dimension=DEFAULT_PLOT_DIMENSION;
    this->plotSetup();
    this->createControlWidget();
    this->setXScaleType(xScaleType);
    this->setYScaleType(yScaleType);
    this->replot();
}

void PlotWidget::plotSetup() {
    this->setAutoFillBackground( true );
    this->setPalette( QPalette( QColor( 165, 193, 228 ) ) );
    this->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    this->setDimension(m_dimension);
    this->setTitle("No title");

    if (m_yScaleType==PlotWidget::Logarithmic) {
        this->setAxisScaleEngine(xBottom,new QwtLog10ScaleEngine());
    } else {
        this->setAxisScaleEngine(xBottom,new QwtLinearScaleEngine());
    }

    if (m_yScaleType==PlotWidget::Logarithmic) {
        this->setAxisScaleEngine(yLeft,new QwtLog10ScaleEngine());
    } else {
        this->setAxisScaleEngine(yLeft,new QwtLinearScaleEngine());
    }

    //canvas
    this->canvas()->setLineWidth( 1 );
    this->canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    this->canvas()->setBorderRadius( 15 );
    QPalette canvasPalette( Qt::lightGray );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    this->canvas()->setPalette( canvasPalette );
    //this->setDimension(m_dimension);
}

void PlotWidget::createControlWidget() {
    //Init the control, ZMP. Other??
    m_baseControl.m_zmp=new ZMP_Handler(this->canvas());

    m_allControl=new QWidget();
    m_allControl->setLayout(new QVBoxLayout());
    QVBoxLayout * l=(QVBoxLayout*)m_allControl->layout();

    //Adding Option button control, this remains always visible
    m_baseControl.toggleButtonOptionEnable =new QPushButton("Time plot");
    m_baseControl.toggleButtonOptionEnable->setCheckable(true);
    m_baseControl.toggleButtonOptionEnable->setMaximumWidth( (2.0/3.0)*CONTROL_WIDTH);
    m_baseControl.toggleButtonOptionEnable->setMinimumWidth( (2.0/3.0)*CONTROL_WIDTH);
    //m_baseControl.toggleButtonOptionEnable->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    l->addWidget(m_baseControl.toggleButtonOptionEnable ,1,Qt::AlignHCenter|Qt::AlignTop);
    connect(m_baseControl.toggleButtonOptionEnable,SIGNAL(clicked()),this,SLOT(timeOptionPressed()) );

    this->initBaseControlWidget();
    l->addWidget(m_baseControl.baseControlWidget,1,Qt::AlignHCenter|Qt::AlignTop);
}

void PlotWidget::initBaseControlWidget() {
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(BASE_SIZE);

    //Widget container and layout for the real control option
    m_baseControl.baseControlWidget=new QWidget();
    QVBoxLayout * lBase=new QVBoxLayout();
    m_baseControl.baseControlWidget->setLayout(lBase) ;
    m_baseControl.baseControlWidget->hide();
    m_baseControl.baseControlWidget->setFont(f);

//    //Adding the ZMP button control
    m_baseControl.toggleButtonZMP = new QPushButton( "Pan" );
    m_baseControl.toggleButtonZMP->setCheckable(true);
   // m_baseControl.toggleButtonZMP->setMaximumWidth( (1.0/3.0)*CONTROL_WIDTH);
   // m_baseControl.toggleButtonZMP->setMinimumWidth( (1.0/3.0)*CONTROL_WIDTH);
   // m_baseControl.toggleButtonZMP->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    lBase->addWidget( m_baseControl.toggleButtonZMP,1,Qt::AlignHCenter|Qt::AlignTop);
    connect(m_baseControl.toggleButtonZMP,SIGNAL(pressed()),this,SLOT(zoomPanButtonPressed()));

    this->timeOptionPressed();
}

void PlotWidget::timeOptionPressed() {
    Q_ASSERT(m_baseControl.baseControlWidget!=NULL);
    if (m_baseControl.toggleButtonOptionEnable->isChecked()) {
        m_baseControl.baseControlWidget->show();
    } else {
        m_baseControl.baseControlWidget->hide();
    }
}

void PlotWidget::zoomPanButtonPressed() {
    if (m_baseControl.toggleButtonZMP->isChecked()) {
        m_baseControl.toggleButtonZMP->setText("Pan");
        m_baseControl.m_zmp->enablePanner();
    } else {
        m_baseControl.toggleButtonZMP->setText("Zoom");
        m_baseControl.m_zmp->enableZoomer();
    }
}

void PlotWidget::setXScaleType(int xScaleType) {
    m_xScaleType=xScaleType;
    if (m_xScaleType==PlotWidget::Logarithmic) {
        this->setAxisScaleEngine(xBottom,new QwtLog10ScaleEngine());
        this->replot();
    } else {
        this->setAxisScaleEngine(xBottom,new QwtLinearScaleEngine());
    }
}

void PlotWidget::setYScaleType(int yScaleType) {
    m_yScaleType=yScaleType;
    if (m_yScaleType==PlotWidget::Logarithmic) {
        this->setAxisScaleEngine(yLeft,new QwtLog10ScaleEngine());
        this->replot();
    } else {
        this->setAxisScaleEngine(yLeft,new QwtLinearScaleEngine());
    }
}


void PlotWidget::setAxisName(QString xName,QString yName) {
    this->setAxisTitle(xBottom, xName );
    this->setAxisTitle(yLeft, yName );
}

void PlotWidget::setBothAxisScale(int xScaleType, double xmin, double xmax,int yScaleType,double ymin, double ymax) {
    this->setXScaleType(xScaleType);
    this->setYScaleType(yScaleType);
    this->setBothAxisScale(xmin,xmax,ymin,ymax);
}

void PlotWidget::setBothAxisScale(double xmin, double xmax,double ymin, double ymax) {
    this->setAxisScale(xBottom, xmin, xmax);
    this->setAxisScale(yLeft, ymin, ymax);
}

void PlotWidget::setDimension(int pointDimension) {
    QFont f=this->font();
    f.setPointSize(pointDimension);
    QFont f_ax=this->font();
    f_ax.setPointSize(f.pointSize()-3);
    QFont f_leg=this->font();
    f_leg.setPointSize(f.pointSize()-2);

    this->setFont(f);
    this->setAxisFont(xBottom,f_ax);
    this->setAxisFont(yLeft,f_ax);
    this->legend()->setFont(f_leg);

   //set font for the title
    QwtText t=this->title();
    f.setPointSize(pointDimension);
    t.setFont(f);
    this->setTitle(t);
    this->replot();
}

int PlotWidget::addTimeData(GenericTimeData * gtd) {
    m_curveList.append(gtd);
    gtd->getCurve()->attach(this);
    this->replot();
    return (m_curveList.length()-1);
}

bool PlotWidget::removeTimeData(int index) {
    bool retval=false;
    if (  (0 <= index) && (index < m_curveList.length()) ) {
        GenericTimeData *  gtd=this->getTimeData(index);
        gtd->getCurve()->detach();
        Q_ASSERT(gtd!=NULL);
        delete gtd;
        m_curveList.removeAt(index);
        retval=true;
        this->replot();
    }
    return retval;
}

GenericTimeData * PlotWidget::getTimeData(int index) {
    GenericTimeData * retval=NULL;
    if (  (0 <= index) && (index < m_curveList.length()) ) {
        retval=m_curveList.at(index);
    }
    return retval;
}
