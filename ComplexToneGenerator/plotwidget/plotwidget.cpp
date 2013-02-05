#include "plotwidget.h"

PlotWidget::PlotWidget(QWidget *parent, int xScaleType, int yScaleType) :
    QwtPlot(parent),
    m_enablePlotUpdate(true)
{
    m_dimension=PLOTWIDGET_DEFAULT_PLOT_DIMENSION;
    this->plotSetup();
    this->setXScaleType(xScaleType);
    this->setYScaleType(yScaleType);
    this->replot();
    connectSignal();
}

void PlotWidget::connectSignal() {
    Q_ASSERT(connect(this,SIGNAL(curveListChanged()),this,SLOT(recalcAndUpdatePlot())));
}

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


    //canvas
    //this->canvas()->setLineWidth( 1 );
    this->canvas()->setFrameStyle( QFrame::Box | QFrame::Raised );
    this->canvas()->setBorderRadius( 12 );
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
    if (!gtd) return -1;
    m_curveList.append(gtd);
    gtd->getCurve()->attach(this);
    Q_ASSERT(connect(gtd,SIGNAL(dataChanged()),this,SLOT(recalcAndUpdatePlot())));
    Q_ASSERT(connect(gtd,SIGNAL(curveAttributeChanged()),this,SLOT(updatePlot())));
    emit curveListChanged();
    return (m_curveList.length()-1);
}

bool PlotWidget::removeTimeData(int index) {
    bool retval=false;
    if (  (0 <= index) && (index < m_curveList.length()) ) {
        GenericTimeData *  gtd=this->getTimeData(index);
        gtd->getCurve()->detach();
        disconnect(gtd,SIGNAL(dataChanged()),this,SLOT(recalcAndUpdatePlot()));
        disconnect(gtd,SIGNAL(curveAttributeChanged()),this,SLOT(updatePlot()));
        Q_ASSERT(gtd!=NULL);
        delete gtd;
        m_curveList.removeAt(index);
        retval=true;
        emit curveListChanged();
    } else {
        qWarning() << "PlotWidget::removeTimeData trying remove curve index "<< index <<", is out of range " << " lenCurveList=" << m_curveList.length();
    }
    return retval;
}

GenericTimeData *PlotWidget::getTimeData(int index) {
    GenericTimeData * retval=NULL;
    if (  (0 <= index) && (index < m_curveList.length()) ) {
        retval=m_curveList.at(index);
    }
    return retval;
}

//void PlotWidget::forceRecreateAll() {
//    if (!m_enableUpdate) return;
//    foreach(GenericTimeData* p, m_curveList) {
//       // p->createData();
//        //p->getControlWidget()->updateUI();
//        //p->forceRegenerateDomDocument();
//    }
//}

//void PlotWidget::forceUpdateUI() {
//    if (!m_enableUpdate) return;
//    foreach(GenericTimeData* p, m_curveList) {
//        //p->getControlWidget()->updateUI();
//        //p->forceRegenerateDomDocument();
//    }
//}


//void PlotWidget::forceUpdateAll() {
//    if (!m_enableUpdate) return;
//    foreach(GenericTimeData* p, m_curveList) {
//      //  p->updateData();
//        //p->getControlWidget()->updateUI();
//        //p->forceRegenerateDomDocument();
//    }
//}

bool PlotWidget::setEnablePlot(bool enable) {
    bool retval=m_enablePlotUpdate;
    if (enable!=m_enablePlotUpdate) {
        m_enablePlotUpdate=enable;
    }
    return retval;
}

QStringList  PlotWidget::getTimeDataStringList() {
    QStringList retval;
    foreach(GenericTimeData* p, m_curveList) {
        GenericTimeDataParams* _params=dynamic_cast<GenericTimeDataParams*> (p->getDataParameters());
        retval << _params->name();
    }
    return retval;
}
