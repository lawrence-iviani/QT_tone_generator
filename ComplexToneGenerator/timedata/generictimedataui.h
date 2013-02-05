#ifndef GENERICTIMEDATAUI_H
#define GENERICTIMEDATAUI_H


#include <QLayout>
#include <QPushButton>
#include <QCheckBox>
#include <scaledsliderwidget.h>
#include <comboboxwidgetcolor.h>
#include <DataUiHandler.h>
#include "timedata/generictimedata.h"

class GenericTimeDataUI : public DataUiHandlerUI
{
    Q_OBJECT
public:
    explicit GenericTimeDataUI( QWidget *widget = 0);
    virtual ~GenericTimeDataUI();
//    /**
//     * @brief isShowXML tell if the showXML widget is showed
//     * @return  true if is showed
//     */
//    bool isShowXML() {return !m_TreeWidgetshowXML->isHidden();}

signals:
    void nameUIChanged(QString);
    void colorUIChanged(QColor);
    void showCurveUIChanged(bool);
    void enableCurveUIChanged(bool);

public slots:
    void nameUIUpdate(QString name);
    void colorUIUpdate(QColor color);
    void showCurveUIUpdate(bool show);
    void enableCurveUIUpdate(bool enable);

//    /**
//      * Force the UI to be update re-reading information from the referenced GenericTimeData class.
//      * This slot doesn't propagate the signal/slot related to the UI, is intended to be called just to refresh the UI if something changes in the data class
//      */
//    virtual void updateControlUI();
    
//    /**
//     * @brief showXML create and show a qtreewidget with the data structure of this curves (this feature is
//     * more for debug purposes and it's very simple).
//     */
//    void showXML();


protected:

protected slots:
//    virtual void updateXML();
private:
    void initControlWidget();//Create only the base control

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

private slots:
    void nameUpdated();
};

#endif // GENERICTIMEDATAUI_H
