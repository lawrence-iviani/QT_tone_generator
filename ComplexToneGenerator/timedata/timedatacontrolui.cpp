#include "timedatacontrolui.h"

TimeDataControlUI::TimeDataControlUI(QWidget *parent) :
    QWidget(parent)
{
    //Setting layout, widget, etc. for the container class
    this->setLayout(new QVBoxLayout());
}

void TimeDataControlUI::addControlFrame(CustomCurveUI *controlFrame,const QString name) {
    if (controlFrame==NULL) return;


    controlFrame->setFrameShape(QFrame::Box);
    controlFrame->setFrameStyle(QFrame::Raised);
    controlFrame->setLineWidth(1);
    controlFrame->setParent(this);

    qDebug() << "TimeDataControlUI::addControlFrame "<< this <<"adding " << controlFrame << " " << name << " to container ";
    s_frameListElement  _t;
    _t.frame=controlFrame;
    _t.name=name;
    m_controlFrameList.append(_t);
    this->layout()->addWidget(controlFrame);
}

void TimeDataControlUI::updateUI() {
    qDebug() << "TimeDataControlUI::updateUI this is  "<<this<<" m_controlFrameList size=" <<  m_controlFrameList.size();

    foreach(const s_frameListElement f, m_controlFrameList ) {
        qDebug() << "Calling update for " << f.frame << " name is " << f.name;
        f.frame->updateControlUI();
    }
}
