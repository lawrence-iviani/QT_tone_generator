#ifndef DATAUIHANDLERUI_H
#define DATAUIHANDLERUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "datauihandlercommon.h"
#include "datauihandlerdelegate.h"

class DataUiHandlerUI : public QWidget
{
    Q_OBJECT
public:
    explicit DataUiHandlerUI(QWidget *parent = 0);
    
    friend class DataUiHandlerDelegate;

signals:
    void descriptionUIChanged(QString description);

public slots:

protected:

    /**
     * @brief addWidget Add a widget to this control. This will layout and store all the widgets together (now it's a vertical layout).
     * Every  widget is added with a label and a priority to move the widget up ord down (this feature is not yet implemented).
     * @param widget
     * @param label
     * @param priority Higher number, higher position (from top to bottom). (NOT YET IMPLEMENTED).
     */
    void addWidget(QWidget * widget, const QString& label="Widget", uint priority=0);

private:

};

#endif // GETTERSETTERUI_H
