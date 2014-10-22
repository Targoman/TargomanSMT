/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 * 
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_DEFINITIONS_LOGGER_H
#define TARGOMAN_DEFINITIONS_LOGGER_H

#include <QObject>
#include <QTime>
#include <QMap>
#include <QMetaType>

#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman {
namespace Common {

TARGOMAN_ADD_EXCEPTION_HANDLER(exLogger, Targoman::Common::exTargomanBase);


/** @brief below macro must be used to generate new UUID and register actor */
#define TARGOMAN_REGISTER_ACTOR(_actorName) \
  Targoman::Common::Logger::instance().registerActor(&ActorUUID, _actorName);

/** @brief These are helper macros to ease usage of Logger */
#define TargomanLogWarn(_level, _message) \
  Targoman::Common::Logger::instance().write(ActorUUID, Targoman::Common::enuLogType::Warning, _level, _message);

#define TargomanLogInfo( _level, _message) \
  Targoman::Common::Logger::instance().write(ActorUUID, Targoman::Common::enuLogType::Info, _level, _message);

#define TargomanLogError( _message) \
  Targoman::Common::Logger::instance().write(ActorUUID, Targoman::Common::enuLogType::Error, 9, _message);

#define TargomanLogDebug(_level, _message) \
  Targoman::Common::Logger::instance().write(ActorUUID, Targoman::Common::enuLogType::Debug, _level, _message);

#define TargomanLogHappy(_level, _message) \
  Targoman::Common::Logger::instance().write(ActorUUID, Targoman::Common::enuLogType::Happy, _level, _message);

namespace Private {
    class LoggerPrivate;
}

/**
 * @enum   enuLogType An enum defining all the valid Log Types
 * @value  Info
 * @value  Warning
 * @value Error
 * @value Debug
 **/
TARGOMAN_DEFINE_ENHANCED_ENUM_BEGIN(enuLogType)
    Info,
    Warning,
    Error,
    Debug,
    Happy
TARGOMAN_DEFINE_ENHANCED_ENUM_STRINGS
    "Info ",
    "Warn ",
    "Error",
    "Debug",
    "Happy"
TARGOMAN_DEFINE_ENHANCED_ENUM_END

class clsLogSettings
{
public:
    clsLogSettings(){
        this->Details = 0xFF;
    }

    inline bool canBeShown(quint8 _level){
        return (this->Details & 0x0F) >= _level;
    }

    inline void setLevel(quint8 _level){
        this->Details = (this->Details & 0xF0) + _level;
    }

private:
    quint8 Details;
};

/**
 * @brief The stuLogMessage struct is used to add new Log to Logger engine.
 */
struct stuLogMessage
{
  QDateTime DateTime;
  QString   ActorID;
  enuLogType::Type Type;
  quint8 Level;
  QString Message;

  /**
   * @brief stuLogMessage Helper method/constructor of stuLogMessage to be used to create new Log Struct
   * @param _actorID ID of the actor who has generated the log. This will be mapped to his name by different modules
   * @param _type Type of the Log baesed on \a enuLogType enumeration
   * @param _level Level of the log. used to ignore or filter some log levels
   * @param _message Message of the log
   * @param _dateTime Date and time of the generated Log. This will be stored to File/DB instead of the write time
   */
  stuLogMessage(QString   _actorID,
                enuLogType::Type _type,
                quint8 _level,
                QString _message,
                QDateTime _dateTime = QDateTime::currentDateTime())
  {
    this->DateTime = _dateTime;
    this->ActorID = _actorID;
    this->Type = _type;
    this->Level = _level;
    this->Message = _message;
  }
};

/**
 * @class Logger
 * @inmodule Targoman::Common
 * @brief The Logger class is a single instance class shared among all the modules. This class is mostly used by
 * macros instead of accesing its methods directly
 */
class Logger : public QObject
{
    Q_OBJECT
public:
    static inline Logger& instance(){return *(Q_LIKELY(Instance) ? Instance : (Instance = new Logger));}
    ~Logger();
    /**
     * @brief Initialization method. If Logger is not initialized then no other function will work
     *
     * @param _fileName Output file where save logs Defaults to "stderr".
     * @param _maxSize Maximum file size before rotate in MegaBytes Defaults to 10.
     * @return bool true if successful else false
     **/
    bool   init(const QString& _fileName,
                QMap<enuLogType::Type, clsLogSettings> _logSettings = QMap<enuLogType::Type, clsLogSettings>(),
                quint64 _maxSize = 10,
                bool _show = true);
    /**
     * @brief Base Log Write function. This function is better called via Macros. @see Macros
     *
     * @param _actorID UUID of the actor who is logging
     * @param _type Log Type from enuLogType
     * @param _level An integer below or equal 10 specifiying Log priority
     * @param _message Message to be written as Log
     * @return void in case of any error it will rais an exception
     **/
    void   write(const QString&     _actorID,
                 enuLogType::Type   _type,
                 quint8             _level,
                 const QString&     _message,
                 bool _newLine = true);
    /**
     * @brief A metho to register actors. this methos will create new UUID if actor has not been registered before
     *
     * @param _actorID UUID for registering actor. If it passes empty a new UUID will be generated
     * @param _actorName Name for the registering Actor
     * @return void in case of any error it will rais an exception
     **/
    void   registerActor(QString* _actorID, const QString& _actorName);
    /**
     * @brief List of all registered actors
     *
     * @return const QMap< QString, QString >&
     **/
    const  QHash<QString, QString>& actors();

    /**
     * @brief setActive Activates and deactivates logging
     * @param _state
     */
    void setActive(bool _state = true);

    bool isActive();

    /**
     * @brief setVisible if set to active logs will be shown in stdout/sterr while saving
     * @param _state
     */
    void setVisible(bool _state = true);

    bool isVisible();
  private:
    Logger(QObject *parent=0);

  signals:
    void sigLogAdded(const QDateTime& _time,
                     QString          _actorID,
                     enuLogType::Type _type,
                     quint8           _level,
                     const QString& _message);

  private:
      static Logger* Instance;
      QScopedPointer<Targoman::Common::Private::LoggerPrivate> pPrivate;

      friend class Targoman::Common::Private::LoggerPrivate;
};

}
}

/* Below line will register enuLogType as a MetaType to be used in signals and slots */
Q_DECLARE_METATYPE(Targoman::Common::enuLogType::Type)

#endif // TARGOMAN_DEFINITIONS_LOGGER_H
