#ifndef DATAUIHANDLERDELEGATE_H
#define DATAUIHANDLERDELEGATE_H

#include <QObject>
#include <QDebug>
#include <QMetaProperty>
#include <errormessage.h>
#include "datauihandlercommon.h"
#include "datauihandlerproperty.h"
#include "datauihandlerui.h"
#include "XML_utils/domhelper.h"
#include "XML_utils/domhelperutility.h"

class DataUiHandlerProperty;
class DataUiHandlerUI;

/**
 * @brief The GetterSetterDelegate class
 * This class provide glue funcionality between data and UI in both sense.
 */
class DataUiHandlerDelegate : public QObject, public DomHelper
{
    Q_OBJECT
public:
    /**
     * @brief DataUiHandlerDelegate Create a delegate that connect
     * @param property
     * @param ui
     * @param parent
     */
    explicit DataUiHandlerDelegate(DataUiHandlerProperty * property, DataUiHandlerUI * ui , QObject *parent = 0);

    /**
     * @brief DataUiHandlerDelegate
     * @param property
     * @param ui
     * @param docType
     * @param rootTag
     * @param parent
     */
    explicit DataUiHandlerDelegate(DataUiHandlerProperty * property, DataUiHandlerUI * ui ,QString docType=DOMHELPER_OBJECTTYPE_TAG, QString rootTag=DOMHELPER_DEFAULT_ROOT_TAG,  QObject *parent = 0);

    /**
     * @brief DataUiHandlerDelegate
     * @param property
     * @param ui
     * @param docType
     * @param rootTag
     * @param fileExtension
     * @param parent
     */
    explicit DataUiHandlerDelegate(DataUiHandlerProperty * property, DataUiHandlerUI * ui ,QString docType=DOMHELPER_OBJECTTYPE_TAG, QString rootTag=DOMHELPER_DEFAULT_ROOT_TAG, QString fileExtension=DOMHELPER_DEFAULT_FILE_SUFFIX,  QObject *parent = 0);


signals:
    
public slots:
    
private:
    DataUiHandlerProperty * m_property;
    DataUiHandlerUI * m_ui;
    bool m_updateDataEnabled;

    void initClass();

    /**
     * @brief connectSignal Connect and verify connection between the signal/slot
     * Look in the property and find if exist, and thus connect, the relative widget
     */
    void connectSignal();

    /**
     * @brief findMethod find a method with a well know syntax and return the complete name as a signature (refer to QMetaMethod)
     * @param methodList
     * @param methodName
     * @return the complete method name, the method is returned as a signature
     */
    const QString findMethodSignature(const QStringList* methodList, QString& methodName);

    /**
     * @brief findSignalSignature Given a certain QMetaObject this method look in and find if exists a method with
     * a certain methodSignature a returns the complete SignalSignature
     * @param metaObj
     * @param methodSignature
     * @return return an empty string if the slot isn't found
     */
    const QString findSignalSignature(const QMetaObject * metaObj, QString& methodSignature);

    /**
     * @brief findSlotSignature Given a certain QMetaObject this method look in and find if exists a method with
     * a certain methodSignature a returns the complete SlotSignature
     * @param metaObj
     * @param methodSignature
     * @return return an empty string if the slot isn't found
     */
    const QString findSlotSignature(const QMetaObject * metaObj, QString& methodSignature);

    /**
     * @brief extractMethodSignatureList
     * @param metaObj
     * @return
     */
    const QStringList extractMethodSignatureList(const QMetaObject * metaObj);

    /**
     * @brief enableDataUpdate Update enabled means that the data are updated every time the  slot dataChanged is called.
     * Setting to false inhibit the update, reccaling with enable=true force the data changed to be called and reupdated all the UI.
     * @param enable
     * @return the previous value;
     */
    bool setEnableDataUpdate(bool enable);

    /**
     * @brief isEnabledData Return if the data update is enabled
     * @return
     */
    bool isEnabledData() {return m_updateDataEnabled;}

private slots:
    /**
     * @brief dataChanged This slot is called everytime a property change,
     * The role of this member is update the DomDocument (may be other features in the future)
     * It also can be called to force an update of the DOM data.
     */
    void dataChanged();
};

#endif // DATAUIHANDLERDELEGATE_H
