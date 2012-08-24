#include "freqplotwidget.h"

FreqPlotWidget::FreqPlotWidget(QWidget *parent, int xScaleType, int yScaleType) :
    PlotWidget(parent,  xScaleType,  yScaleType)
{
    this->createControlWidget();
}

void FreqPlotWidget::createControlWidget() {
    m_allControl=new QFrame();
    m_allControl->setFrameShape(QFrame::WinPanel);
    m_allControl->setFrameShadow(QFrame::Raised);
    m_allControl->setLayout(new QVBoxLayout());
    QVBoxLayout * l=(QVBoxLayout*)m_allControl->layout();

    //Adding Option button control, this remains always visible
    m_baseControl.toggleButtonOptionEnable=new QPushButton("Freq. plot option");
    m_baseControl.toggleButtonOptionEnable->setCheckable(true);
    m_baseControl.toggleButtonOptionEnable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    l->addWidget(m_baseControl.toggleButtonOptionEnable ,1,Qt::AlignHCenter|Qt::AlignTop);
    connect(m_baseControl.toggleButtonOptionEnable,SIGNAL(clicked()),this,SLOT(freqOptionPressed()) );

    this->initBaseControlWidget();
    l->addWidget(m_baseControl.baseControlWidget,1,Qt::AlignHCenter|Qt::AlignTop);
}

void FreqPlotWidget::initBaseControlWidget() {
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout for the real control option
    m_baseControl.baseControlWidget=new QFrame();
    m_baseControl.baseControlWidget->setFrameShape(QFrame::WinPanel);
    m_baseControl.baseControlWidget->setFrameShadow(QFrame::Raised);
    QVBoxLayout * lBase=new QVBoxLayout();
    m_baseControl.baseControlWidget->setLayout(lBase) ;
    m_baseControl.baseControlWidget->hide();
    m_baseControl.baseControlWidget->setFont(f);
}

void FreqPlotWidget::freqOptionPressed() {
    Q_ASSERT(m_baseControl.baseControlWidget!=NULL);
    if (m_baseControl.toggleButtonOptionEnable->isChecked()) {
        m_baseControl.baseControlWidget->show();
    } else {
        m_baseControl.baseControlWidget->hide();
    }
}
