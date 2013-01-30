#include "errormessage.h"


ErrorMessage::ErrorMessage(int type) :
    QString(),
    m_time(QTime::currentTime()),
    m_type(type),
    m_method(""),
    m_message(""),
    m_debugLevel(DEBUG_NOT_SO_IMPORTANT)
{
    generateMessage();
}

ErrorMessage::ErrorMessage(const QString &method, const QString &message, int type) :
    QString(),
    m_time(QTime::currentTime()),
    m_type(type),
    m_method(method),
    m_message(message),
    m_debugLevel(DEBUG_NOT_SO_IMPORTANT)
{
    generateMessage();
}

inline void ErrorMessage::generateMessage() {
    this->clear();
    this->append(QString("(%1)-").arg(m_time.toString("hh:mm:ss.zzz")));
    this->append(QString("[%1]-").arg(typeToString(m_type)));
    this->append(QString("%1 ").arg(m_method));
    this->append(QString("--%1--").arg(m_message));
}

inline void ErrorMessage::setErrorMessage(const QString &method, const QString &message, int type) {
    m_time=QTime::currentTime();
    m_type=type;
    m_method.clear();
    m_method.append(method);
    m_message.clear();
    m_message.append(message);
    generateMessage();
}

 void ErrorMessage::setMethod(const QString& method) {
    m_method.clear();
    m_method.append(method);
    generateMessage();
}

void ErrorMessage::setMessage(const QString& message) {
    m_message.clear();
    m_message.append(message);
    generateMessage();
}

void ErrorMessage::appendMessage(ErrorMessage message) {
    m_message.append("\n\t");
    m_message.append(message);
}

inline QString ErrorMessage::typeToString(int type) {
    QString retval="";
    switch(type) {
        case DEBUGMESSAGE:
            retval.append("DEBUG");
            break;
        case WARNINGMESSAGE:
            retval.append("WARNING");
            break;
        case ERRORMESSAGE:
            retval.append("ERROR");
            break;
        default:
            retval.append("UNKNOWN");
    }
    return retval;
}


//------------ STATIC METHOD ------------//
QString ErrorMessage::WARNING(ErrorMessage* errMessage,const QString& method,const QString& message) {
    QString retval="";
    if (errMessage) {
        errMessage->setErrorMessage(method,message,ErrorMessage::WARNINGMESSAGE);
        retval=QString(errMessage->toAscii());
    }
    return retval;
}

QString ErrorMessage::WARNING(const QString& method,const QString& message) {
    return ErrorMessage(method,message,ErrorMessage::WARNINGMESSAGE);
}


QString ErrorMessage::DEBUG(ErrorMessage* errMessage, const QString& method, const QString& message, uint level) {
    QString retval="";
    if (errMessage) {
        errMessage->setDebugLevel(level);
        errMessage->setErrorMessage(method,message,ErrorMessage::DEBUGMESSAGE);
        retval=QString(errMessage->toAscii());
    }
    return retval;
}

QString ErrorMessage::DEBUG(const QString& method, const QString& message, uint level) {
    ErrorMessage _m=ErrorMessage(method,message);
    _m.setDebugLevel(level);
    return _m;
}

QString ErrorMessage::ERROR(ErrorMessage* errMessage,const QString& method,const QString& message) {
    QString retval="";
    if (errMessage) {
        errMessage->setErrorMessage(method,message,ErrorMessage::ERRORMESSAGE);
        retval=QString(errMessage->toAscii());
    }
    return retval;
}

QString ErrorMessage::ERROR(const QString& method,const QString& message) {
    return ErrorMessage(method,message,ErrorMessage::ERRORMESSAGE);
}
