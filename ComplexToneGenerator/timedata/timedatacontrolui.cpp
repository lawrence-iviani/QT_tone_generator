#include "timedatacontrolui.h"

TimeDataControlUI::TimeDataControlUI(QWidget *parent) :
    QWidget(parent),
    m_globalFrame(new QFrame(parent))
{
    //Setting layout, widget, etc. for the container class
    this->setLayout(new QVBoxLayout());
}

void TimeDataControlUI::addControlFrame(CustomCurveUI *controlFrame,const QString name) {
    if (controlFrame==NULL) return;
    s_frameListElement  _t;
    _t.frame=controlFrame;
    _t.name=name;
    m_controlFrameList.append(_t);
}


void TimeDataControlUI::updateUI() {
    foreach(const s_frameListElement f, m_controlFrameList )
        f.frame->updateControlUI();
}
