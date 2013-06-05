#ifndef TIMEPLOTWIDGETUI_H
#define TIMEPLOTWIDGETUI_H

#include <DataUiHandler.h>
#include <QLayout>
#include <QPushButton>
#include <scaledsliderwidget.h>
#include <CTG_constants.h>

/**
 * @brief The TimePlotWidgetUI class contains not only the controls related to the property but also some general
 * tools for the drawing. These needs to be manual controlled.
 */
class TimePlotWidgetUI : public DataUiHandlerUI
{
    Q_OBJECT
public:
    explicit TimePlotWidgetUI(QWidget *widget = 0);
    void addZMPControlWidget(QWidget * zmpWidget) {this->addWidget(zmpWidget,"Zoom & C. controls");}
    virtual ~TimePlotWidgetUI();
signals:
    /**
     * @brief sampleRateChanged This signal is emitted whenever the duration change for some reason
     * @param SR The new sample rate
     */
    void sampleRateUIChanged(qreal);

    /**
     * @brief duartionChanged This signal is emitted whenever the duration change for some reason
     * @param duration The new duration
     */
    void maxDurationUIChanged(qreal);

    /**
     * @brief showAllCurvesToggled the signal is emitted when the check box is pressed.
     * The value is true if the relative selection is checked, false otherwise
     */
    void showAllCurvesToggled();

    /**
     * @brief enableAllCurvesToggled the signal is emitted when the check box is pressed.
     * The value is true if the relative selection is checked, false otherwise
     */
    void hideAllCurvesToggled();

public slots:
    void sampleRateUIUpdate(qreal sampleRate);
    void maxDurationUIUpdate(qreal maxduration);

private:
    void initControlWidget();
    struct {
      ScaledSliderWidget * sliderSampleRate;
      ScaledSliderWidget * sliderMaxDuration;
      QPushButton * showAllCurves;
      QPushButton * hideAllCurves;
    } m_baseControl;
};

#endif // TIMEPLOTWIDGETUI_H
