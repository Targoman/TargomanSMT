/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include <QDateTime>
#include <QProcess>
#include <QUuid>
#include <QFile>
#include <QHash>
#include <QMutex>
#include <QMetaType>

#include "Logger.h"
#include "Private/Logger_p.h"
#include "exTargomanBase.h"
#include "Configuration/tmplConfigurable.h"

namespace Targoman {
namespace Common {

using namespace Configuration;

tmplConfigurable<QString> LogFile(
        Logger::moduleName() + "/File",
        "Log File to store logs. If not set then no log file will be generated",
        "",
        [] (const intfConfigurable& _item, QString&){
            if (_item.toVariant().toString().size())
                Logger::instance().init(_item.toVariant().toString());
            return true;
        },
        "",
        "FILE_PATH",
        "log-file",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );


tmplConfigurable<bool> DontShow(
        Logger::moduleName() + "/DontShow",
        "Disable showing logs while saving",
        false,
        [] (const intfConfigurable& _item, QString&){
            Logger::instance().setVisible(_item.toVariant().toBool() == false);
            return true;
        },
        "",
        "",
        "log-dont-show",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<bool> Disable(
        Logger::moduleName() + "/Disable",
        "Disable logs",
        false,
        [] (const intfConfigurable& _item, QString&){
            Logger::instance().setActive(_item.toVariant().toBool() == false);
            return true;
        },
        "",
        "",
        "log-disable",
        enuConfigSource::Arg,
        false
        );

static clsLogSettings* LogSettings = new clsLogSettings[enuLogType::getCount()];

tmplConfigurable<QStringList> LogDebugDetail(
        Logger::moduleName() + "/DebugDetail",
        "Set Details of Normal Logs",
        QStringList()<<"true"<<"true",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            QStringList Params = _item.toVariant().toString().split(",");
            if (Params.size() < 2){
                _errorMessage = "Invalid count of arguments for " + _item.configPath();
                return false;
            }
            LogSettings[enuLogType::Debug].setDetails(QVariant(Params.at(0)).toBool(),
                                                      QVariant(Params.at(1)).toBool());
            return true;
        },
        "",
        "SHOW_TIME,SHOW_ACTOR",
        "log-debug-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<quint8> LogDebugLevel(
        Logger::moduleName() + "/DebugLevel",
        "Set Normal level",
        5,
        [] (const intfConfigurable& _item, QString&){
            LogSettings[enuLogType::Debug].setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "log-debug-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> LogWarningDetail(
        Logger::moduleName() + "/WarningDetail",
        "Set Details of Normal Logs",
        QStringList()<<"true"<<"true",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            QStringList Params = _item.toVariant().toString().split(",");
            if (Params.size() < 2){
                _errorMessage = "Invalid count of arguments for " + _item.configPath();
                return false;
            }
            LogSettings[enuLogType::Warning].setDetails(QVariant(Params.at(0)).toBool(),
                                                      QVariant(Params.at(1)).toBool());
            return true;
        },
        "",
        "SHOW_TIME,SHOW_ACTOR",
        "log-warning-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<quint8> LogWarningLevel(
        Logger::moduleName() + "/WarningLevel",
        "Set Normal level",
        5,
        [] (const intfConfigurable& _item, QString&){
            LogSettings[enuLogType::Warning].setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "log-warning-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> LogErrorDetail(
        Logger::moduleName() + "/ErrorDetail",
        "Set Details of Normal Logs",
        QStringList()<<"true"<<"true",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            QStringList Params = _item.toVariant().toString().split(",");
            if (Params.size() < 2){
                _errorMessage = "Invalid count of arguments for " + _item.configPath();
                return false;
            }
            LogSettings[enuLogType::Error].setDetails(QVariant(Params.at(0)).toBool(),
                                                      QVariant(Params.at(1)).toBool());
            return true;
        },
        "",
        "SHOW_TIME,SHOW_ACTOR",
        "log-error-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> LogInfoDetail(
        Logger::moduleName() + "/InfoDetail",
        "Set Details of Normal Logs",
        QStringList()<<"true"<<"true",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            QStringList Params = _item.toVariant().toString().split(",");
            if (Params.size() < 2){
                _errorMessage = "Invalid count of arguments for " + _item.configPath();
                return false;
            }
            LogSettings[enuLogType::Info].setDetails(QVariant(Params.at(0)).toBool(),
                                                      QVariant(Params.at(1)).toBool());
            return true;
        },
        "",
        "SHOW_TIME,SHOW_ACTOR",
        "log-info-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<quint8> LogInfoLevel(
        Logger::moduleName() + "/InfoLevel",
        "Set Normal level",
        5,
        [] (const intfConfigurable& _item, QString&){
            LogSettings[enuLogType::Info].setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "log-info-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> LogHappyDetail(
        Logger::moduleName() + "/HappyDetail",
        "Set Details of Normal Logs",
        QStringList()<<"true"<<"true",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            QStringList Params = _item.toVariant().toString().split(",");
            if (Params.size() < 2){
                _errorMessage = "Invalid count of arguments for " + _item.configPath();
                return false;
            }
            LogSettings[enuLogType::Happy].setDetails(QVariant(Params.at(0)).toBool(),
                                                      QVariant(Params.at(1)).toBool());
            return true;
        },
        "",
        "SHOW_TIME,SHOW_ACTOR",
        "log-happy-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<quint8> LogHappyLevel(
        Logger::moduleName() + "/HappyLevel",
        "Set Normal level",
        5,
        [] (const intfConfigurable& _item, QString&){
            LogSettings[enuLogType::Happy].setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "log-happy-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

/******************************************************************************************/
Logger::Logger(QObject *parent) :
    QObject(parent),pPrivate(new Targoman::Common::Private::LoggerPrivate)
{
    this->registerActor(&this->pPrivate->ActorUUID,"BaseLogger");
}

bool Logger::init(const QString &_fileName,
                  quint64 _maxSize,
                  bool _show)
{
    qRegisterMetaType<Targoman::Common::enuLogType::Type>("Targoman::Common::enuLogType::Type");
    this->pPrivate->LogFile.setFileName(_fileName.size() ? _fileName : "/dev/null");
    this->pPrivate->MaxFileSize = _maxSize * 1024 * 1024;
    this->setVisible(_show);
    this->setActive();
    return this->pPrivate->open();
}


void Logger::write(const QString &_actorID,
                   enuLogType::Type _type,
                   quint8 _level,
                   const QString &_message,
                   bool _newLine)
{
    if (this->isActive() == false || LogSettings[_type].canBeShown(_level) == false)
        return;

    QString Actor = this->pPrivate->Actors.value(_actorID);
    if (!_actorID.isEmpty() && Actor.isEmpty())
        throw exLogger(QString("Actor ID %1 Not registerd: message(%2)").arg(_actorID).arg(_message));

    QByteArray LogMessage= LogSettings[_type].details(Actor).toLatin1();

    LogMessage+= QString("[%1]").arg(enuLogType::toStr(_type));
    LogMessage += "[" + QString::number(_level) + "]: ";

    if (_newLine)
        LogMessage += _message+"\n";


    QMutexLocker Locker(&this->pPrivate->mxLog);
    if (this->pPrivate->LogFile.fileName().size()){
        if (!this->pPrivate->LogFile.isOpen() ||
                !this->pPrivate->LogFile.isWritable())
            this->pPrivate->open();

        if (this->pPrivate->LogFile.isWritable())
            this->pPrivate->LogFile.write(LogMessage);
    }

    if (this->isVisible()){
        switch(_type){
        case enuLogType::Debug:
            fprintf(stderr,"%s%s%s", TARGOMAN_COLOR_DEBUG, LogMessage.constData(), TARGOMAN_COLOR_NORMAL);
            break;
        case enuLogType::Info:
            fprintf(stderr,"%s%s%s", TARGOMAN_COLOR_INFO, LogMessage.constData(), TARGOMAN_COLOR_NORMAL);
            break;
        case enuLogType::Warning:
            fprintf(stderr,"%s%s%s", TARGOMAN_COLOR_WARNING, LogMessage.constData(), TARGOMAN_COLOR_NORMAL);
            break;
        case enuLogType::Happy:
            fprintf(stderr,"%s%s%s", TARGOMAN_COLOR_HAPPY, LogMessage.constData(), TARGOMAN_COLOR_NORMAL);
            break;
        case enuLogType::Error:
            fprintf(stderr,"%s%s%s", TARGOMAN_COLOR_ERROR, LogMessage.constData(), TARGOMAN_COLOR_NORMAL);
            break;
        default:
            break;
        }
    }

    if (this->pPrivate->LogFile.fileName().size()){
        this->pPrivate->LogFile.flush();
        this->pPrivate->rotateLog();
    }
    Locker.unlock();
    emit this->sigLogAdded(QDateTime().currentDateTime(), _actorID, _type, _level, _message);
}

void Logger::registerActor(QString *_actorUUID, const QString &_actorName)
{
    QMutexLocker Locker(&this->pPrivate->mxLog);

    if (_actorName.isEmpty())
        throw exLogger("Invalid Null Actor Name: " + *_actorUUID);

    if (_actorUUID->isEmpty())
        *_actorUUID = QUuid::createUuid().toString();
    else if (this->pPrivate->Actors.contains(*_actorUUID))
        throw exLogger(QString ("Invalid Actor UUID: %1 it is in use by %2").arg(
                           *_actorUUID).arg(this->pPrivate->Actors.value(*_actorUUID)));

    if (this->pPrivate->Actors.values().contains(_actorName) &&
            this->pPrivate->Actors.key(_actorName,0) != *_actorUUID)
        throw exLogger(QString ("Invalid Actor UUID/Name: %1/%2 it has been previously registered as: %3/%4").arg(
                           *_actorUUID).arg(_actorName).arg(this->pPrivate->Actors.key(_actorName)).arg(_actorName));

    this->pPrivate->Actors.insert(*_actorUUID, _actorName);
    //TargomanInfo(6, qPrintable(_actorName + " Registerd with UUID: " + *_actorUUID) );
}

void Logger::unregisterActor(const QString& _actorUUID)
{
    QMutexLocker Locker(&this->pPrivate->mxLog);
    QString ActorName = this->pPrivate->Actors.value(_actorUUID);

    this->pPrivate->Actors.remove(_actorUUID);

    //TargomanInfo(6, _actorUUID + " / " + ActorName + " Unregistered");
}

const QHash<QString, QString> &Logger::actors()
{
    return this->pPrivate->Actors;
}
Logger::~Logger()
{
    //It is defined to suppress error on QScoppedPointer
}

bool Logger::init()
{
    return true;
}

void Logger::setActive(bool _state)
{
    this->pPrivate->GlobalSettings = (this->pPrivate->GlobalSettings & 0xFE)  | (_state ? 1 : 0);
}

bool Logger::isActive()
{
    return this->pPrivate->GlobalSettings & 0x01 ? true : false;
}

void Logger::setVisible(bool _state)
{
    this->pPrivate->GlobalSettings = (this->pPrivate->GlobalSettings & 0xFD)  | (_state ? 2 : 0);
}

bool Logger::isVisible()
{
    return this->pPrivate->GlobalSettings & 0x02 ? true : false;
}


/***************************************************************************/

Targoman::Common::Private::LoggerPrivate::LoggerPrivate()
{
}

bool Targoman::Common::Private::LoggerPrivate::open()
{
    if (this->LogFile.isOpen())
        this->LogFile.close();

    this->LogFile.open(QIODevice::Append);

    if ( ! this->LogFile.isWritable()) {
        TargomanError("%s",QString("Unable to open <%1> for writing").arg(this->LogFile.fileName()).toUtf8().constData());
        return false;
    }
    return true;
}

void Targoman::Common::Private::LoggerPrivate::rotateLog()
{
    if (((quint64)this->LogFile.size() > this->MaxFileSize) ) {
        TargomanDebug(7, "Rotating Logs");
        this->LogFile.close();
        QString NewFileName = this->LogFile.fileName() + QDateTime().currentDateTime().toString("yyyy_MM_dd_hh:mm:ss");
        QFile::rename(this->LogFile.fileName(), NewFileName);
#ifdef Q_OS_UNIX
        QProcess::startDetached("gzip",QStringList(NewFileName));
#else
        throw exTargomanNotImplemented("Compressing files on OSs other than linux is not yet implemented");
#endif
    }
}

}
}

