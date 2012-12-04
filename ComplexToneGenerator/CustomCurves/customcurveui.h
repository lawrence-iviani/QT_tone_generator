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
        QFrame(parent)
    {
    }


    void forceUpdateXML() {updateXML();}
signals:

    /**
     * @brief controlUIUpdated append this signal at the end of every update ui call, this grant the XML descriptor  is aligned with the UI.
     */
    void controlUIUpdated();
public slots:
    virtual void updateControlUI() {qDebug() << "CustomCurveUI::updateControlUI this method should never be called!!!";Q_ASSERT(false);}

protected slots:
    virtual void updateXML() {}//qDebug() << "CustomCurveUI::updateXML this method should never be called!!!";}//Q_ASSERT(false);}
};

#endif // CUSTOMCURVEUI_H
