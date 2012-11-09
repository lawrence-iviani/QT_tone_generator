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


    
signals:
    
public slots:
    virtual void updateControlUI() {qDebug() << "CustomCurveUI::updateControlUI this method should never be called!!!";}
};

#endif // CUSTOMCURVEUI_H
