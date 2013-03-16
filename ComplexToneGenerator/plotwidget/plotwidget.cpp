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

//void PlotWidget::connectSignal() {
//    Q_ASSERT(connect(this,SIGNAL(curveListChanged()),this,SLOT(recalcAndUpdatePlot())));
//}

void PlotWidget::plotSetup() {
    this->setAutoReplot(false);
    this->setAutoFillBackground( true );
    this->setPalette( QPalette( QColor(50,50,50)));//( 165, 193, 228 ) ) );
    this->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    this->setDimension(m_dimension);
    this->setTitle("PlotWidget NO TITLE");

    if (m_yScaleType==PlotWidget::Logarithmic) {
        this->setAxisScaleEngine(xBottom,new QwtLog10ScaleEngine());
    } else {
        this->setAxisScaleEngine(xBottom,new QwtLinearScaleEngine());
    }
    QPalette _palette=this->axisWidget(xBottom)->palette();
    _palette.setColor(QPalette::WindowText, Qt::gray);
    _palette.setColor(QPalette::Text, Qt::gray);
    this->axisWidget(xBottom)->setPalette((const QPalette)_palette);

    if (m_yScaleType==PlotWidget::Logarithmic) {
        this->setAxisScaleEngine(yLeft,new QwtLog10ScaleEngine());
    } else {
        this->setAxisScaleEngine(yLeft,new QwtLinearScaleEngine());
    }
    _palette=this->axisWidget(yLeft)->palette();
    _palette.setColor(QPalette::WindowText, Qt::gray);
    _palette.setColor(QPalette::Text, Qt::gray);
    this->axisWidget(yLeft)->setPalette((const QPalette)_palette);

    _palette=this->axisWidget(yRight)->palette();
    _palette.setColor(QPalette::WindowText, Qt::gray);
    _palette.setColor(QPalette::Text, Qt::gray);
    this->axisWidget(yRight)->setPalette((const QPalette)_palette);

    //canvas
    this->canvas()->setFrameStyle( QFrame::Box | QFrame::Raised );
    this->canvas()->setBorderRadius( 12 );
    PlotWidgetBackground *bg = new PlotWidgetBackground(QColor( 230, 230, 255 ));
    bg->attach(this);
    m_scrollRubberBand=new ScrollRubberBand(this->canvas());

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
    this->setAxisFont(yRight,f_ax);
    this->legend()->setFont(f_leg);

   //set font for the title
    QwtText t=this->title();
    f.setPointSize(pointDimension);
    t.setFont(f);
    this->setTitle(t);
    this->replot();
}


