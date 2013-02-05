#ifndef DATAUIHANDLERPROPERTY_H
#define DATAUIHANDLERPROPERTY_H

#include <QObject>
#include <QUuid>
#include "datauihandlercommon.h"
#include "datauihandlerdelegate.h"

class DataUiHandlerDelegate;

/**
 * @brief The GetterSetterProperty class provides data controls, extracting data and setting data.
 * Now its working is very simple but may be in the future can make more (ie DB extraction/insertion and also).
 * The basic class provides a name (stored in the meatobjcet className, description and an ID).
 *
 */
class DataUiHandlerProperty : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUuid uuid READ uuid NOTIFY uuidPropertyChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionPropertyChanged)

public:
    explicit DataUiHandlerProperty(QObject *parent=0);
    explicit DataUiHandlerProperty(DataUiHandlerProperty *copyProperty,QObject *parent=0);

    //getter
    const QUuid& uuid() {return m_uuid;}
    const QString& description() {return m_description;}

    /**
     *
     */
    friend class DataUiHandlerDelegate;

public slots:
    //setter, these method
    void setDescription(const QString &description) {descriptionPropertyUpdate(description);}

    //update without refresh
    void descriptionPropertyUpdate(const QString &description);

signals:
    void descriptionPropertyChanged(QString description);
    void uuidPropertyChanged(QUuid);

private:
    QUuid m_uuid;
    QString m_description;

    /**
     * @brief sendAllPropertiesSignal call this method when you want syncronize the UI, this is used by  DataUiHandlerDelegate in order
     * to syncronize the UI after init.
     */
    void sendAllPropertiesSignal();
};

#endif // DATAUIHANDLERPROPERTY_H
