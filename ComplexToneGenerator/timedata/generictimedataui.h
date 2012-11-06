#ifndef GENERICTIMEDATAUI_H
#define GENERICTIMEDATAUI_H


#include <QLayout>
#include <QPushButton>
#include <QCheckBox>
#include <scaledsliderwidget.h>
#include <comboboxwidgetcolor.h>
#include "CustomCurves/customcurveui.h"
#include "timedata/generictimedata.h"

class GenericTimeData;

class GenericTimeDataUI : public CustomCurveUI
{
    Q_OBJECT
public:
    explicit GenericTimeDataUI( GenericTimeData * gtd, QWidget *widget = 0);

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
   // void setExtendedControl(CustomCurveUI *customCurveControl);


private:
    void createBaseControl();//Create all the control, of the base class and of any eventual extended class. It llayouts all together.
    void initBaseControlWidget();//Create only the base control
    GenericTimeData * m_genericTimeData;
    struct {
      CustomCurveUI *controlFrame;
      QLineEdit * lineName;
      ComboBoxWidgetColor *comboColor;
      QCheckBox *checkBoxEnableCurve;
      QCheckBox *checkBoxShowCurve;
      QCheckBox *checkBoxEnableEnvelope;
      QWidget *envelopeWidget;
    } m_baseControl; //This class control.

private slots:
    void nameUpdated();
};

#endif // GENERICTIMEDATAUI_H
