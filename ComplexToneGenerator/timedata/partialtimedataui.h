#ifndef PARTIALTIMEDATAUI_H
#define PARTIALTIMEDATAUI_H

#include "generictimedataui.h"
#include <scaledsliderwidget.h>


class PartialTimeDataUI : public GenericTimeDataUI
{
    Q_OBJECT
public:
    explicit PartialTimeDataUI(QWidget *widget = 0);
    virtual ~PartialTimeDataUI();

signals:
    void durationUIChanged(qreal);
    void t0UIChanged(qreal);

public slots:

    void durationUIUpdate(qreal duration);
    void t0UIUpdate(qreal t0);
//    /**
//      * Force the UI to be update re-reading information from the referenced GenericTimeData class.
//      * This slot doesn't propagate the signal/slot related to the UI, is intended to be called just to refresh the UI if something changes in the data class
//      */
//    virtual void updateControlUI();

//    /**
//     * @brief durationChange tell to this class to update the duration when is changed, this class update the duration widget and the start time
//     * @param duration
//     */
//    virtual void durationChange(qreal duration);
protected:

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

};

#endif // PARTIALTIMEDATAUI_H
