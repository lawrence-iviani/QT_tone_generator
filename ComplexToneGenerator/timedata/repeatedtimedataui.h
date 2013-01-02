#ifndef REPEATEDTIMEDATAUI_H
#define REPEATEDTIMEDATAUI_H

#include <CustomCurves/customcurveui.h>
#include <timedata/repeatedtimedata.h>

class RepeatedTimeData;

class RepeatedTimeDataUI : public CustomCurveUI
{
    Q_OBJECT
public:
    explicit RepeatedTimeDataUI(RepeatedTimeData *rtd, QWidget *widget = 0);
    virtual ~RepeatedTimeDataUI();

signals:

public slots:

    /**
     * @brief durationChange tell to this class to update the duration when is changed, this class update the number of repetitions
     * @param duration
     */
    virtual void durationChange(qreal duration);

    /**
      * Force the UI to be update re-reading information from the referenced GenericTimeData class.
      * This slot doesn't propagate the signal/slot related to the UI, is intended to be called just to refresh the UI if something changes in the data class
      */
    virtual void updateControlUI();

protected:

private:
    void initControlWidget();

    /**
      * This struct contains controls useful to be integrated in the inerhit class.
      * This means that this struct should be inserted in the widget that will be associated to the method
      * of an instance of GenericTimeData::setControlWidget(QWidget *). In this case will possible add the control implemented in this struct
      */
    struct {
        ScaledSliderWidget *widgetBlankTime;
    } m_repeatedDataControl;

    RepeatedTimeData *m_repeatedTimeData;

private slots:

};

#endif // REPEATEDTIMEDATAUI_H
