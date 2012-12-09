#ifndef CUSTOMCURVEUI_H
#define CUSTOMCURVEUI_H

#include <QFrame>
#include <QDebug>

/**
  * Custom curves UI should inherits from this class, implement control and signal for itself instance operations and implement the method updateUI
  * that performs UI update of the related custom curve class.
  */
class CustomCurveUI : public QFrame
{
    Q_OBJECT
public:
    explicit CustomCurveUI(QWidget *parent = 0) :
        QFrame(parent),
        m_enableUpdateUI(true)
    {
    }

    /**
     * @brief setEnableUpdateUI enable/disable the UI of this curves to be updated in TimeDataControlUI
     * @param enable
     * @return
     */
    bool setEnableUpdateUI(bool enable) {
        bool preValue=m_enableUpdateUI;
        m_enableUpdateUI=enable;
        return preValue;
    }

    /**
     * @brief enableUpdateUI
     * @return
     */
    bool enableUpdateUI() {return m_enableUpdateUI;}

    /**
     * @brief forceUpdateXML this method force an update of the internal DOM structure, assure in this cheap design the XML data rappresentation is up to date
     */
    void forceUpdateXML() {updateXML();}
signals:

    /**
     * @brief controlUIUpdated append this signal at the end of every update ui call, this grant the XML descriptor  is aligned with the UI.
     */
    void controlUIUpdated();
public slots:
    virtual void updateControlUI() {qDebug() << "CustomCurveUI::updateControlUI this method should never be called!!!";Q_ASSERT(false);}

protected slots:
    /**
     * @brief updateXML is the internal method the inerithed class must implement filling the XML data in their own structure.
     */
    virtual void updateXML() {}//qDebug() << "CustomCurveUI::updateXML this method should never be called!!!";}//Q_ASSERT(false);}

private:
    bool m_enableUpdateUI;
};

#endif // CUSTOMCURVEUI_H
