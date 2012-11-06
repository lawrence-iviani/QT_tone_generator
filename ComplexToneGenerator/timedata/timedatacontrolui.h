#ifndef TIMEDATACONTROLUI_H
#define TIMEDATACONTROLUI_H

#include <QObject>
#include <QVBoxLayout>
//#include <QFrame>
#include <QObject>
#include <QList>
#include "CustomCurves/customcurveui.h"



typedef struct  {
        CustomCurveUI *frame;
        QString name;
} s_frameListElement;


/**
  * Class container for all the control UI curves.
  * This class allows all curves to be added  with the  addControlFrame
  */

class TimeDataControlUI : public QWidget
{
    Q_OBJECT
public:
    explicit TimeDataControlUI(QWidget *widget = 0);

    void addControlFrame(CustomCurveUI *controlFrame,const QString name="");

signals:
    
public slots:
    /**
      * Force the UI to be update re-reading information from the referenced GenericTimeData class.
      * This slot doesn't propagate the signal/slot related to the UI, is intended to be called just to refresh the UI if something changes in the data class
      */
    virtual void updateUI();

private:

    QList<s_frameListElement> m_controlFrameList;
    QFrame *  m_globalFrame;
};

#endif // TIMEDATACONTROLUI_H
