#ifndef DATAUIHANDLERUI_H
#define DATAUIHANDLERUI_H

#include <QWidget>
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

    //UIupdate
    //virtual void descriptionUIUpdate(QString description) {Q_UNUSED(description);}
};

#endif // GETTERSETTERUI_H
