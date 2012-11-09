#ifndef PARTIALTIMEDATAUI_H
#define PARTIALTIMEDATAUI_H

#include <QFrame>
#include "generictimedataui.h"
#include "partialtimedata.h"
#include "CustomCurves/customcurveui.h"

class PartialTimeData;

class PartialTimeDataUI : public CustomCurveUI
{
    Q_OBJECT
public:
    explicit PartialTimeDataUI(PartialTimeData *ptd, QWidget *widget = 0);
    
signals:
    
public slots:
    /**
      * Force the UI to be update re-reading information from the referenced GenericTimeData class.
      * This slot doesn't propagate the signal/slot related to the UI, is intended to be called just to refresh the UI if something changes in the data class
      */
    virtual void updateControlUI();

protected:
    /**
      * This method is called when the extended class set itself widget to add more control
      */
   // void setExtendedControl(CustomCurveUI * customCurveControl);

private:
    void initControlWidget();

    /**
      * This struct contains controls useful to be integrated in the inerhit class.
      * This means that this struct should be inserted in the widget that will be associated to the method
      * of an instance of GenericTimeData::setControlWidget(QWidget *). In this case will possible add the control implemented in this struct
      */
    struct {
        ScaledSliderWidget *widgetDuration;
        ScaledSliderWidget *widget_t0;
    } m_partialDataControl;

    PartialTimeData *m_partiaTimeData;
};

#endif // PARTIALTIMEDATAUI_H
