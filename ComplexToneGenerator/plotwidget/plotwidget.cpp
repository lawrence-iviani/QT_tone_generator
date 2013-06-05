#include "plotwidget.h"

PlotWidget::PlotWidget(QWidget *parent, int xScaleType, int yScaleType) :
    QwtPlot(parent),
    m_zmp(NULL)
{
    m_referenceDimension=PLOTWIDGET_DEFAULT_PLOT_DIMENSION;
    this->plotSetup();
    this->setXScaleType(xScaleType);
    this->setYScaleType(yScaleType);
    m_zmp=new ZMP_helper(this);
    m_zmp->enablePicker();
    this->replot();
}

void PlotWidget::plotSetup() {
    this->setAutoReplot(false);
    this->setAutoFillBackground( true );
    this->setPalette( QPalette( QColor(50,50,50)));
    this->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    this->setDimension(m_referenceDimension);

    setPlotTitle("Plot Widget Title");

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
    } else {
        this->setAxisScaleEngine(xBottom,new QwtLinearScaleEngine());
    }
    this->replot();
}

QwtInterval PlotWidget::getXAxisInterval() {
   return axisInterval(xBottom);
}

QwtInterval PlotWidget::getYAxisInterval() {
   return axisInterval(yLeft);
}

void PlotWidget::setPlotTitle(const QString& title) {
    QwtText _title= this->title();
    _title.setText(title);
    _title.setColor(Qt::lightGray);
    //set font for the title
    QFont _f=_title.font();
    _f.setPointSize(m_referenceDimension+2);
    _f.setBold(true);
    _title.setFont(_f);
    this->setTitle(_title);

}

void PlotWidget::setYScaleType(int yScaleType) {
    m_yScaleType=yScaleType;
    if (m_yScaleType==PlotWidget::Logarithmic) {
        this->setAxisScaleEngine(yLeft,new QwtLog10ScaleEngine());
    } else {
        this->setAxisScaleEngine(yLeft,new QwtLinearScaleEngine());
    }
    this->replot();
}

void PlotWidget::setAxesName(QString xName,QString yName) {
    this->setAxisTitle(xBottom, xName );
    this->setAxisTitle(yLeft, yName );
}

void PlotWidget::setBothAxesScale(int xScaleType, qreal xmin, qreal xmax,int yScaleType,qreal ymin, qreal ymax) {
    this->setXScaleType(xScaleType);
    this->setYScaleType(yScaleType);
    this->setBothAxesScale(xmin,xmax,ymin,ymax);
}

void PlotWidget::setBothAxesScale(qreal xmin, qreal xmax,qreal ymin, qreal ymax) {
    this->setAxisScale(xBottom, xmin, 1.1*xmax);
    this->setAxisScale(yLeft, ymin, 1.2*ymax);

    //Also set zoom
    if (m_zmp) {
        QRectF _rect(xmin,1.1*ymin,1.1*xmax-xmin,1.2*ymax-ymin);
        m_zmp->changeZoomBase(_rect);
    }
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
    this->replot();
}


