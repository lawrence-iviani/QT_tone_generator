#include "datauihandlerproperty.h"

DataUiHandlerProperty::DataUiHandlerProperty(QObject *parent) :
    QObject(parent),
    m_description("no description")
{
    this->setObjectName("DataUiHandlerProperty");
    m_uuid=QUuid::createUuid();
}

DataUiHandlerProperty::DataUiHandlerProperty(DataUiHandlerProperty *copyProperty,QObject* parent) :
    QObject(parent)
{
    if (copyProperty) {
        this->setObjectName(copyProperty->objectName());
        m_description=copyProperty->description();
        m_uuid=QUuid::createUuid();
    } else {
        this->setObjectName("DataUiHandlerProperty");
        m_uuid=QUuid::createUuid();
        m_description="copy object null";
    }
}

void DataUiHandlerProperty::descriptionPropertyUpdate(const QString& description) {
    if (description=="") return;
    if (description==m_description) return;
    m_description=description;
    emit (descriptionPropertyChanged(m_description));
}

void DataUiHandlerProperty::sendAllPropertiesSignal() {
    const QMetaObject *_metaObject=this->metaObject();
    for(int i = 0; i < _metaObject->propertyCount(); i++) {
        //Looking if the property has slot&connection
        QMetaProperty _prop=_metaObject->property(i);//Getting property i
        QString _propName=_prop.name();//Getting properties name
        if (_prop.hasNotifySignal()) {
            //getting signal, property type
            QString _signalSignature=_prop.notifySignal().signature();
            QMetaMethod _method=_metaObject->method(_prop.notifySignalIndex());
            QVariant _value=_prop.read(this);
            if (!_method.invoke(this,Q_ARG(QVariant,_value))) {
                PRINT_WARNING( ErrorMessage::WARNING(Q_FUNC_INFO,
                             QString("Error invoking signal %1 on property %2, value type %3 NOT UPDATED should be |%4|")
                             .arg( _signalSignature).arg(_propName).arg(_prop.typeName()).arg(_value.toString()) ));
            } else {
                PRINT_DEBUG_LEVEL (ErrorMessage::DEBUG_NOT_IMPORTANT,
                             ErrorMessage::DEBUG(Q_FUNC_INFO,
                             QString("Correct invoked signal %1 on property %2, value type %3 now should be |%4|")
                             .arg( _signalSignature).arg(_propName).arg(_prop.typeName()).arg(_value.toString()) ));
            }
        }
    }
}
