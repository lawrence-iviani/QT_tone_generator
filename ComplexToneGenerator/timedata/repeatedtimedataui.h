#ifndef REPEATEDTIMEDATAUI_H
#define REPEATEDTIMEDATAUI_H

#include <CustomCurves/customcurveui.h>
#include <timedata/repeatedtimedata.h>

class RepeatedTimeDataUI : public CustomCurveUI
{
    Q_OBJECT
public:
    explicit RepeatedTimeDataUI(RepeatedTimeData *rtd, QWidget *widget = 0);

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

    } m_repeatedDataControl;

    RepeatedTimeData *m_repeatedTimeData;
    
};

#endif // REPEATEDTIMEDATAUI_H
