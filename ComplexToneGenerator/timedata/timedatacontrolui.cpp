#include "timedatacontrolui.h"

TimeDataControlUI::TimeDataControlUI(QWidget *parent) :
    QWidget(parent)
{
    //Setting layout, widget, etc. for the container class
    this->setLayout(new QVBoxLayout());
}

void TimeDataControlUI::addControlFrame(CustomCurveUI *controlFrame,const QString name) {
    if (controlFrame==NULL) return;
    controlFrame->setParent(this);
    //qDebug() << "TimeDataControlUI::addControlFrame "<< this <<"adding " << controlFrame << " " << name << " to container ";
    s_frameListElement  _t;
    _t.frame=controlFrame;
    _t.name=name;
    m_controlFrameList.append(_t);

    //Preparing name label
    QWidget * _w=new QWidget(this);
    QVBoxLayout * _l=new QVBoxLayout();
    controlFrame->setFrameStyle(QFrame::QFrame::Raised);
    controlFrame->setFrameShape(QFrame::QFrame::Panel);
    controlFrame->setLineWidth(1);
    controlFrame->setMidLineWidth(1);
    _l->addWidget(new QLabel(name),1 );
    _l->addWidget(controlFrame);
    _w->setLayout(_l);
    this->layout()->addWidget(_w);
}

void TimeDataControlUI::updateUI() {
    qDebug() << "TimeDataControlUI::updateUI  m_controlFrameList size=" <<  m_controlFrameList.size();

    foreach(const s_frameListElement f, m_controlFrameList ) {
        if (f.frame->enableUpdateUI()) {
           // qDebug() << "TimeDataControlUI::updateControlUI & forceUpdateXML update for " << f.frame << " name is " << f.name;
            f.frame->updateControlUI();
            f.frame->forceUpdateXML();
        }
    }
}
