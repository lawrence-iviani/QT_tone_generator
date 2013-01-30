#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include "ErrorMessage_global.h"
#include <QString>
#include <QTime>

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 2
#endif // DEBUG_LEVEL

//#define DEBUG(errMessage, method, message) if (errMessage) errMessage->setErrorMessage(method,message)
#define PRINT_WARNING(message) qWarning() << message;
#define PRINT_DEBUG_LEVEL(level,message)  if (level <= DEBUG_LEVEL ) qDebug() << message;
#define PRINT_DEBUG(message) PRINT_DEBUG_LEVEL(message.debugLevel() ,message);
#define PRINT_ERROR(message) qFatal() << message;
//#define ERROR(errMessage, method, message) if (errMessage) errMessage->setErrorMessage(method,message,ErrorMessage::ERRORMESSAGE)

/**
 * @brief The ErrorMessage class ineriths from QString and add some simple functionality to handle in an uniform way message string like error, debug ant so on
 * by every single piece of program.
 */
class ERRORMESSAGESHARED_EXPORT ErrorMessage : public QString
{

public:
    enum MessageType {DEBUGMESSAGE, WARNINGMESSAGE, ERRORMESSAGE};
    enum DebugLevel {DEBUG_IMPORTANT=1, DEBUG_NOT_SO_IMPORTANT=2, DEBUG_NOT_IMPORTANT=3};

    ErrorMessage(int type=DEBUGMESSAGE);
    ErrorMessage(const QString& method,const QString& message, int type=DEBUGMESSAGE);

    bool isDebugMessage()   {return (m_type==DEBUGMESSAGE ? true : false);}
    bool isWarningMessage() {return (m_type==WARNINGMESSAGE ? true : false);}
    bool isErrorMessage()   {return (m_type==ERRORMESSAGE ? true : false);}

    const QString method()  {return m_method;}
    const QString message() {return m_message;}

    void setMethod(const QString& method);
    void setMessage(const QString& message);
    void setDebugLevel(uint level) { if (m_type==DEBUGMESSAGE) m_debugLevel=level;}
    uint debugLevel() {return m_debugLevel;}

    void setErrorMessage(const QString& method,const QString& message, int type=DEBUGMESSAGE);

    /**
     * @brief appendMessage append a message to this message, useful when there is multiple error in nidified call
     * @param message The message to append
     */
    void appendMessage(ErrorMessage message);

    /**
     * @brief WARNING create a warning message in the pointer errMessage
     * @param errMessage
     * @param method
     * @param message
     * @return
     */
    static QString WARNING(ErrorMessage* errMessage,const QString& method,const QString& message);

    /**
     * @brief WARNING construct a warning message and return as string
     * @param method
     * @param message
     * @return
     */
    static QString WARNING(const QString& method,const QString& message);

    /**
     * @brief DEBUG create a debug message in the pointer errMessage
     * @param errMessage
     * @param method
     * @param message
     * @param level The level of debug, set the priority and change at compile time
     * @return
     */
    static QString DEBUG(ErrorMessage* errMessage,const QString& method,const QString& message, uint level=DEBUG_NOT_IMPORTANT);

    /**
     * @brief DEBUG construct a debug message and return as string
     * @param method
     * @param message
     * @param level The level of debug, set the priority and change at compile time
     * @return
     */
    static QString DEBUG(const QString& method,const QString& message, uint level=DEBUG_NOT_IMPORTANT);

    /**
     * @brief ERROR create an error message in the pointer errMessage
     * @param errMessage
     * @param method
     * @param message
     * @return
     */
    static QString ERROR(ErrorMessage* errMessage,const QString& method,const QString& message);

    /**
     * @brief ERROR construct an error message and return as string
     * @param method
     * @param message
     * @return
     */
    static QString ERROR(const QString& method,const QString& message);

private:
    QString typeToString(int type);
    QTime m_time;
    int m_type;
    QString m_method;
    QString m_message;
    uint m_debugLevel;

    void generateMessage();
};

#endif // ERRORMESSAGE_H
