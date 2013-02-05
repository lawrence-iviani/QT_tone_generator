#ifndef TIMEPLOTWIDGETUI_H
#define TIMEPLOTWIDGETUI_H

#include <DataUiHandler.h>
#include <QLayout>
#include <QPushButton>
#include <scaledsliderwidget.h>
#include <CTG_constants.h>
#include <QRadioButton>
#include <QGroupBox>
#include "zmp_handler.h"



/**
 * @brief The TimePlotWidgetUI class contains not only the controls related to the property but also some general
 * tools for the drawing. These needs to be manual controlled.
 */
class TimePlotWidgetUI : public DataUiHandlerUI
{
    Q_OBJECT
public:
    explicit TimePlotWidgetUI(QWidget *widget = 0);
    TimePlotWidgetUI(ZMP_Handler * zmp, QWidget *widget = 0);
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

    void ZMP_statusChanged();
private:
    void initControlWidget();
    ZMP_Handler * m_zmp;//Handle zoom, panel etc
    struct {
      ScaledSliderWidget * sliderSampleRate;
      ScaledSliderWidget * sliderMaxDuration;
      struct {
          QGroupBox * groupBox;
          QRadioButton * PICKER;
          QRadioButton * PAN;
          QRadioButton * ZOOM;
      } ZMP_RadioButton;//Zoom Move Pan Pick enable radio button
      QPushButton * showAllCurves;
      QPushButton * hideAllCurves;
    } m_baseControl;
};

#endif // TIMEPLOTWIDGETUI_H
