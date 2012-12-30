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
    virtual ~GenericTimeDataUI();
    /**
     * @brief isShowXML tell if the showXML widget is showed
     * @return  true if is showed
     */
    bool isShowXML() {return !m_TreeWidgetshowXML->isHidden();}


signals:


public slots:
    /**
      * Force the UI to be update re-reading information from the referenced GenericTimeData class.
      * This slot doesn't propagate the signal/slot related to the UI, is intended to be called just to refresh the UI if something changes in the data class
      */
    virtual void updateControlUI();
    
    /**
     * @brief showXML create and show a qtreewidget with the data structure of this curves (this feature is
     * more for debug purposes and it's very simple).
     */
    void showXML();


protected:

protected slots:
    virtual void updateXML();
private:
    void initControlWidget();//Create only the base control

    GenericTimeData * m_genericTimeData;
    struct {
      QLineEdit * lineName;
      ComboBoxWidgetColor *comboColor;
      QCheckBox *checkBoxEnableCurve;
      QCheckBox *checkBoxShowCurve;
      QPushButton *exportXML;
      QPushButton *importXML;
      QPushButton *showXML;
      QPushButton *copy;
      QPushButton *paste;
    } m_baseControl; //This class control.
    QTreeWidget * m_TreeWidgetshowXML;
private slots:
    void nameUpdated();
};

#endif // GENERICTIMEDATAUI_H
