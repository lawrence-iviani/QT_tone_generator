#include "datauihandlerdelegate.h"

DataUiHandlerDelegate::DataUiHandlerDelegate(DataUiHandlerProperty * property, DataUiHandlerUI * ui , QObject *parent) :
    QObject(parent),
    DomHelper((QObject*)property),
    m_property(property),
    m_ui(ui),
    m_updateDataEnabled(true)
{
    initClass();
}

DataUiHandlerDelegate::DataUiHandlerDelegate(DataUiHandlerProperty * property,
                                             DataUiHandlerUI * ui ,
                                             QString docType,
                                             QString rootTag,
                                             uint version,
                                             QObject *parent) :
    QObject(parent),
    DomHelper((QObject*)property,docType,rootTag,version),
    m_property(property),
    m_ui(ui),
    m_updateDataEnabled(true)
{
    initClass();
}

DataUiHandlerDelegate::DataUiHandlerDelegate(DataUiHandlerProperty * property,
                                             DataUiHandlerUI * ui ,
                                             QString docType,
                                             QString rootTag,
                                             uint version,
                                             QString fileExtension,
                                             QObject *parent) :
    QObject(parent),
    DomHelper((QObject*)property,docType,rootTag,version,fileExtension),
    m_property(property),
    m_ui(ui),
    m_updateDataEnabled(true)
{
    initClass();
}

DataUiHandlerDelegate::~DataUiHandlerDelegate() {
    if (m_property) delete m_property;
    if (m_ui) delete m_ui;
}

void DataUiHandlerDelegate::initClass() {
    addUnhandleableNode( "uuid");
    PRINT_DEBUG_LEVEL (ErrorMessage::DEBUG_NOT_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"\t--------- Connecting signal ---------"));
    connectSignal(m_property,m_ui);
    PRINT_DEBUG_LEVEL (ErrorMessage::DEBUG_NOT_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"\t--------- Creating DOM ---------"));
    this->selfObjectData();
    PRINT_DEBUG_LEVEL (ErrorMessage::DEBUG_NOT_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"\t--------- Emit ALL Prop Signal ---------"));
    this->setEnableDataUpdate(false);
    m_property->sendAllPropertiesSignal();
    this->setEnableDataUpdate(true);
}

void DataUiHandlerDelegate::connectSignal(DataUiHandlerProperty *properties, DataUiHandlerUI *ui) {
    if (properties==NULL || ui==NULL) {
        PRINT_WARNING( ErrorMessage::WARNING(Q_FUNC_INFO,
                     QString("trying to connect unreferenced data, property@%1, UI@%2").
                                             arg(QString::number((qlonglong)properties,16)).
                                             arg(QString::number((qlonglong)ui,16))));
        return;
    }

    const QMetaObject* _propMetaObject = properties->metaObject();
    const QMetaObject* _uiMetaObject = ui->metaObject();

    //Collecting all the method from property and ui
    QStringList _propMethod=extractMethodSignatureList(_propMetaObject);
    QStringList _uiMethod=extractMethodSignatureList(_uiMetaObject);

//    PRINT_DEBUG( ErrorMessage::DEBUG(Q_FUNC_INFO,"Connecting PROPERTY to UI"));
//----------- 1. Connect the property class signal to the UI ---------------
    for(int i = 0; i < _propMetaObject->propertyCount(); i++) {
        //Looking if the property has slot&connection
        QMetaProperty _prop=_propMetaObject->property(i);//Getting property i
        QString _propName=_prop.name();//Getting properties name
        //continue if the property must be not handled
        if (getUnhandleableNode()->contains(_propName,Qt::CaseInsensitive) ) continue;
        //Looking
        QString _signalSignature="";
        if (_prop.hasNotifySignal()) {
            _signalSignature=_prop.notifySignal().signature();
        }
        if (_signalSignature=="") continue;

        //looking if the corrispondent UI slot exists!
        QString _slotName=QString("%1%2").arg(_propName).arg(POSTPEND_UPDATE_UI_SLOT);
        QString _uiMethodSignature=findMethodSignature(&_uiMethod,_slotName);
        if (_uiMethodSignature=="") {
            PRINT_WARNING( ErrorMessage::WARNING(Q_FUNC_INFO,
                         QString("found signal %1 but candidate method %2 can't be find").arg(_signalSignature).arg(_slotName)
                        ));
            continue;
        }
        QString _slotSignature=findSlotSignature(_uiMetaObject,_uiMethodSignature);
        if (_slotSignature=="") {
            PRINT_WARNING( ErrorMessage::WARNING(Q_FUNC_INFO,
                         QString("found signal %1 but method %2 is not a slot").arg(_signalSignature).arg(_uiMethodSignature)
                        ));
            continue;
        }

        //Connect property SIGNAL to ui SLOT
        _signalSignature.prepend("2");//2 is in front because in qobjectdefs.h the macro SIGNAL introduce this number
        _slotSignature.prepend("1");//1 is in front because in qobjectdefs.h the macro SLOT introduce this number
        if (!connect(properties,_signalSignature.toAscii().constData(),ui,_slotSignature.toAscii().data())) {
            PRINT_WARNING( ErrorMessage::WARNING(Q_FUNC_INFO,
                         QString("fail to connect signal %1 to slot %2").arg(_signalSignature).arg(_slotSignature)
                        ));
        } else {
            //Every time a property is changed this method is called
            Q_ASSERT(connect(properties,_signalSignature.toAscii().constData(),this,SLOT(dataChanged())));
            PRINT_DEBUG_LEVEL (ErrorMessage::DEBUG_NOT_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,
                         QString("connected signal %1 to slot %2").arg(_signalSignature).arg(_slotSignature)
                        ));
        }
    }

   // PRINT_DEBUG( ErrorMessage::DEBUG(Q_FUNC_INFO,"Connecting UI to PORPERTY"));
 //----------- 2. Connect the UI class signal to the property class ---------------
        for(int n = 0; n < _propMetaObject->propertyCount(); n++) {
            //Looking if the property has slot&connection
            QMetaProperty _prop=_propMetaObject->property(n);//Getting property i
            QString _propName=_prop.name();//Getting properties name

            //continue if the property must be not handled
            if (getUnhandleableNode()->contains(_propName,Qt::CaseInsensitive) ) continue;
            //Looking for the signal name from UI
            QString _signalName=QString("%1%2").arg(_propName).arg(POSTPEND_UI_CHANGED_SIGNAL);
            QString _uiMethodSignature=findMethodSignature(&_uiMethod,_signalName);
            QString _signalSignature=findSignalSignature(_uiMetaObject,_uiMethodSignature);
            if (_signalSignature=="") continue;

            //looking if the corrispondent property slot exists!
            QString _slotName=QString("%1%2").arg(PREPEND_UPDATE_PROPERTY_SLOT).arg(_propName);
            _slotName.replace(PREPEND_UPDATE_PROPERTY_SLOT.length(),1,_slotName.at(PREPEND_UPDATE_PROPERTY_SLOT.length()).toUpper());//Uppercase

            QString _propMethodSignature=findMethodSignature(&_propMethod,_slotName);
            if (_propMethodSignature=="") {
                PRINT_WARNING( ErrorMessage::WARNING(Q_FUNC_INFO,
                             QString("found signal %1 but candidate method %2 can't be find").arg(_signalSignature).arg(_slotName)
                            ));
                continue;
            }
            QString _slotSignature=findSlotSignature(_propMetaObject,_propMethodSignature);
            if (_slotSignature=="") {
                PRINT_WARNING( ErrorMessage::WARNING(Q_FUNC_INFO,
                             QString("found signal %1 but method %2 is not a slot").arg(_signalSignature).arg(_propMethodSignature)
                            ));
                continue;
            }

            //Connect property SIGNAL to ui SLOT
            _signalSignature.prepend("2");//2 is in front because in qobjectdefs.h the macro SIGNAL introduce this number
            _slotSignature.prepend("1");//1 is in front because in qobjectdefs.h the macro SLOT introduce this number
            if (!connect(ui,_signalSignature.toAscii().constData(),properties,_slotSignature.toAscii().data())) {
                PRINT_WARNING( ErrorMessage::WARNING(Q_FUNC_INFO,
                             QString("fail to connect signal %1 to signal %2").arg(_signalSignature).arg(_slotSignature)
                            ));
            } else {
                PRINT_DEBUG_LEVEL (ErrorMessage::DEBUG_NOT_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,
                             QString("connected signal %1 to signal %2").arg(_signalSignature).arg(_slotSignature)
                            ));
            }
        }
}

const QString DataUiHandlerDelegate::findMethodSignature(const QStringList* methodList, QString& methodName) {
    QStringList nameList=methodList->filter(methodName);
    if (nameList.length() > 1) {
        QString _methods="";
        for (int i=0; i < nameList.length() ; i++)
            _methods.append(QString("\n\t%1").arg(nameList.at(i)));
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,
                                            QString("Found too many (%1) methods %2: %3").arg(nameList.length()).arg(methodName).arg(_methods)
                                         ));
        return "";
    }
    if (nameList.length()==0) return "";
    return (const QString) nameList.at(0);
}

const QString DataUiHandlerDelegate::findSignalSignature(const QMetaObject * metaObj, QString& methodSignature) {
    if (methodSignature=="") return "";
    int _sigIndex=metaObj->indexOfSignal(methodSignature.toLocal8Bit().constData());
    if (_sigIndex<=-1) {
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,
                     QString("NOT FOUND signal |%1|").arg(methodSignature)
                    ));
        return "";
    } /*else {
        PRINT_DEBUG( ErrorMessage::DEBUG(Q_FUNC_INFO,
                     QString("Found signal %1 with index=%2").arg(methodSignature).arg(_sigIndex)
                    ));
    }*/
    QMetaMethod _signal=metaObj->method(_sigIndex);
    //qDebug() <<Q_FUNC_INFO<<"Property SIGNAL"<< _signal.signature() << _signal.parameterTypes() << _signal.parameterNames();
    return (const QString) _signal.signature();
}

const QString DataUiHandlerDelegate::findSlotSignature(const QMetaObject * metaObj, QString& methodSignature) {
    if (methodSignature=="") return "";
    int _slotIndex=metaObj->indexOfSlot(methodSignature.toLocal8Bit().constData());
    if (_slotIndex<=-1) {
        PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,
                     QString("NOT FOUND slot |%1|").arg(methodSignature)
                    ));
        return "";

    } /*else {
        PRINT_DEBUG( ErrorMessage::DEBUG(Q_FUNC_INFO,
                     QString("Found slot %1 with index=%2").arg(methodSignature).arg(_slotIndex)
                    ));
    }*/
    QMetaMethod _slot=metaObj->method(_slotIndex);
    return (const QString) _slot.signature();
}

const QStringList DataUiHandlerDelegate::extractMethodSignatureList(const QMetaObject * metaObj) {
    QStringList retval;
    for (int n=0; n < metaObj->methodCount() ; n++)
        retval << metaObj->method(n).signature();
    return retval;
}

void DataUiHandlerDelegate::dataChanged() {
    PRINT_DEBUG_LEVEL (ErrorMessage::DEBUG_NOT_IMPORTANT, ErrorMessage::DEBUG(Q_FUNC_INFO,"slot called!"));
    //updateDOM
    if (m_updateDataEnabled &&  !isImportingDomData()) {
        if (!this->selfObjectData()) {
            PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"Something was wrong when try to generate the self objcet DOM data"));
        } else  {
            PRINT_DEBUG_LEVEL (ErrorMessage::DEBUG_NOT_IMPORTANT, ErrorMessage::DEBUG(Q_FUNC_INFO,"doc is \n%1\n").arg(getDomDocument().toString(4)));
        }
    } else {
        PRINT_DEBUG_LEVEL (ErrorMessage::DEBUG_NOT_IMPORTANT, ErrorMessage::DEBUG(Q_FUNC_INFO,"Do nothing!"));
    }
}

bool DataUiHandlerDelegate::setEnableDataUpdate(bool enable) {
    bool retval=m_updateDataEnabled;
    if (enable!=m_updateDataEnabled) {
        m_updateDataEnabled=enable;
        if (enable) dataChanged();
    }
    return retval;
}

void DataUiHandlerDelegate::replacePropertiesAndUI(DataUiHandlerProperty *properties, DataUiHandlerUI *ui) {
    if (properties==m_property && m_ui==ui) return; //nothing to change!!

    //Should delete the signal connection?
    ErrorMessage _err1(Q_FUNC_INFO, QString("Prev Internal ref m_property@%1 m_ui@%2, new ref. properties@%3, ui@%4")
                       .arg(QString::number((qlonglong)m_property,16))
                       .arg(QString::number((qlonglong)m_ui,16))
                       .arg(QString::number((qlonglong)properties,16))
                       .arg(QString::number((qlonglong)ui,16)));

    if (properties==NULL || ui==NULL) {
        PRINT_WARNING( ErrorMessage::WARNING(Q_FUNC_INFO,
                     QString("trying to connect unreferenced data, property@%1, UI@%2").arg((qlonglong)properties).arg((qlonglong)ui)));
        return;
    }
    DataUiHandlerProperty *prevProperties=m_property;
    DataUiHandlerUI *prevUi=m_ui;

    QHash<QString, QVariant> _hashProperties;
    readProperties(m_property,_hashProperties);

    m_property=properties;
    m_ui=ui;

    ErrorMessage _err2(Q_FUNC_INFO, QString("Now  internal ref m_property@%1 m_ui@%2, new ref. properties@%3, ui@%4")
                        .arg(QString::number((qlonglong)m_property,16))
                        .arg(QString::number((qlonglong)m_ui,16))
                        .arg(QString::number((qlonglong)properties,16))
                        .arg(QString::number((qlonglong)ui,16)));
    PRINT_DEBUG_LEVEL (ErrorMessage::DEBUG_NOT_IMPORTANT,_err1);
    PRINT_DEBUG_LEVEL (ErrorMessage::DEBUG_NOT_IMPORTANT,_err2);

    //Delete previous property/UI if they are changed
    if (prevProperties!=m_property) {
        setHostObject(m_property);
        writeProperties(m_property,_hashProperties);
        //Before delete  i should disconnect the previous signal!!!
        delete(prevProperties);
    }

    if (prevUi!=m_ui) {
        //BE CAREFUL, THIS MAY LEAD TO SOME SEGFAULT IF SOME WIDGET IS CONNECTED AS A CHILD TO THE WIDGET POINTED
        //BY THIS POINTER. SOME MECHANISM WITH REMOVE THE CHILD WIDGET SHOULD BE IMPLEMENTED, LEAVING THE DELETE OF
        //ANY SINGLE WIDGET TO THE CREATOR ITSELF
        delete(prevUi);
    }

    connectSignal(m_property,m_ui);
    //updating properties
    this->setEnableDataUpdate(false);
    m_property->sendAllPropertiesSignal();
    this->selfObjectData();
    this->setEnableDataUpdate(true);
}

void DataUiHandlerDelegate::readProperties(DataUiHandlerProperty *properties, QHash<QString, QVariant>& hash) {
    const QMetaObject* _propMetaObject = properties->metaObject();
    for (int n=0; n < _propMetaObject->propertyCount() ; n++) {
        QMetaProperty _prop=_propMetaObject->property(n);
        QString _propName=_prop.name();
        if (getUnhandleableNode()->contains(_propName,Qt::CaseInsensitive) ) continue;
        QVariant _propValueVariant=_prop.read(properties);
        if (_propValueVariant.isValid())
            hash[_propName] = _propValueVariant;
    }
}

void DataUiHandlerDelegate::writeProperties(DataUiHandlerProperty *properties, QHash<QString, QVariant>& hash) {
    const QMetaObject* _propMetaObject = properties->metaObject();
    for (int n=0; n < _propMetaObject->propertyCount() ; n++) {
        QMetaProperty _prop=_propMetaObject->property(n);
        QString _propName=_prop.name();
        if (getUnhandleableNode()->contains(_propName,Qt::CaseInsensitive) ) continue;
        if (hash.contains(_propName)) {
             QVariant  _propValueVariant = hash.value(_propName);
             if (!_prop.write(properties,_propValueVariant))
                 Q_ASSERT(false);
        }
    }
}
