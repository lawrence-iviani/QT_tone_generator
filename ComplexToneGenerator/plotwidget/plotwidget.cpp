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
    this->setAutoReplot(false);
    this->setAutoFillBackground( true );
    this->setPalette( QPalette( QColor( 165, 193, 228 ) ) );
    this->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    this->setDimension(m_dimension);
    this->setTitle("PlotWidget NO TITLE");

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
    //this->canvas()->setLineWidth( 1 );
    this->canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    this->canvas()->setBorderRadius( 15 );
    QPalette canvasPalette( Qt::lightGray );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    this->canvas()->setPalette( canvasPalette );
    m_scrollRubberBand=new ScrollRubberBand(this->canvas());
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

void PlotWidget::setBothAxisScale(int xScaleType, qreal xmin, qreal xmax,int yScaleType,qreal ymin, qreal ymax) {
    this->setXScaleType(xScaleType);
    this->setYScaleType(yScaleType);
    this->setBothAxisScale(xmin,xmax,ymin,ymax);
}

void PlotWidget::setBothAxisScale(qreal xmin, qreal xmax,qreal ymin, qreal ymax) {
    this->setAxisScale(xBottom, xmin, 1.1*xmax);
    this->setAxisScale(yLeft, ymin, 1.1*ymax);
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
    connect(gtd,SIGNAL(dataUpdated()),this,SLOT(updatePlot()));
    connect(gtd,SIGNAL(curveAttributeUpdated()),this,SLOT(updatePlot()));
    this->updatePlot();
    emit curveListChanged();
    return (m_curveList.length()-1);
}

bool PlotWidget::removeTimeData(int index) {
    bool retval=false;
    if (  (0 <= index) && (index < m_curveList.length()) ) {
        GenericTimeData *  gtd=this->getTimeDataList(index);
        gtd->getCurve()->detach();
        disconnect(gtd,SIGNAL(dataUpdated()),this,SLOT(updatePlot()));
        disconnect(gtd,SIGNAL(curveUpdated()),this,SLOT(updatePlot()));
        Q_ASSERT(gtd!=NULL);
        delete gtd;
        m_curveList.removeAt(index);
        retval=true;
        this->updatePlot();
        emit curveListChanged();
    }
    return retval;
}

GenericTimeData *PlotWidget::getTimeDataList(int index) {
    GenericTimeData * retval=NULL;
    if (  (0 <= index) && (index < m_curveList.length()) ) {
        retval=m_curveList.at(index);
    }
    return retval;

}


QStringList  PlotWidget::getTimeDataStringList() {
    QStringList retval;
    foreach(GenericTimeData* p, m_curveList) {
        retval << p->name();
    }
    return retval;
}
