#include "plotwidget.h"



PlotWidget::PlotWidget(QWidget *parent, int xScaleType, int yScaleType) :
    QwtPlot(parent)
{
    m_dimension=PLOTWIDGET_DEFAULT_PLOT_DIMENSION;
    this->plotSetup();
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
