#ifndef REPEATEDTIMEDATAUI_H
#define REPEATEDTIMEDATAUI_H

#include "partialtimedataui.h"
#include <scaledsliderwidget.h>


class RepeatedTimeDataUI : public PartialTimeDataUI
{
    Q_OBJECT
public:
    explicit RepeatedTimeDataUI(QWidget *widget = 0);
    virtual ~RepeatedTimeDataUI();

signals:
    void blankTimeUIChanged(qreal);

public slots:
    void blankTimeUIUpdate(qreal blanktime);
    void setBlankTimeScale(qreal maxDuration);

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

private slots:
//maxduration change????
};

#endif // REPEATEDTIMEDATAUI_H
