#include "timeplotwidget.h"

TimePlotWidget::TimePlotWidget(QWidget *parent, int xScaleType, int yScaleType) :
    PlotWidget(parent,  xScaleType,  yScaleType)
{
    this->createControlWidget();
}

void TimePlotWidget::createControlWidget() {
    //Init the control, ZMP. Other??
    m_baseControl.m_zmp=new ZMP_Handler(this->canvas());

    m_allControl=new QWidget();
    m_allControl->setLayout(new QVBoxLayout());
    QVBoxLayout * l=(QVBoxLayout*)m_allControl->layout();

    //Adding Option button control, this remains always visible
    m_baseControl.toggleButtonOptionEnable=new QPushButton("Time plot option");
    m_baseControl.toggleButtonOptionEnable->setCheckable(true);
    //m_baseControl.toggleButtonOptionEnable->setMaximumWidth( (2.0/3.0)*CONTROL_WIDTH);
    //m_baseControl.toggleButtonOptionEnable->setMinimumWidth( (2.0/3.0)*CONTROL_WIDTH);
    m_baseControl.toggleButtonOptionEnable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    l->addWidget(m_baseControl.toggleButtonOptionEnable ,1,Qt::AlignHCenter|Qt::AlignTop);
    connect(m_baseControl.toggleButtonOptionEnable,SIGNAL(clicked()),this,SLOT(timeOptionPressed()) );

    this->initBaseControlWidget();
    l->addWidget(m_baseControl.baseControlWidget,1,Qt::AlignHCenter|Qt::AlignTop);
}

void TimePlotWidget::initBaseControlWidget() {
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
   // m_baseControl.toggleButtonZMP->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    lBase->addWidget( m_baseControl.toggleButtonZMP,1,Qt::AlignHCenter|Qt::AlignTop);
    connect(m_baseControl.toggleButtonZMP,SIGNAL(pressed()),this,SLOT(zoomPanButtonPressed()));

    this->timeOptionPressed();
}

void TimePlotWidget::timeOptionPressed() {
    Q_ASSERT(m_baseControl.baseControlWidget!=NULL);
    if (m_baseControl.toggleButtonOptionEnable->isChecked()) {
        m_baseControl.baseControlWidget->show();
    } else {
        m_baseControl.baseControlWidget->hide();
    }
}

void TimePlotWidget::zoomPanButtonPressed() {
    if (m_baseControl.toggleButtonZMP->isChecked()) {
        m_baseControl.toggleButtonZMP->setText("Pan");
        m_baseControl.m_zmp->enablePanner();
    } else {
        m_baseControl.toggleButtonZMP->setText("Zoom");
        m_baseControl.m_zmp->enableZoomer();
    }
}
