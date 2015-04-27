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
        "Log/File",
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
        "Log/DontShow",
        "Disable showing logs while saving",
        false,
        [] (const intfConfigurable&, QString&){
            Logger::instance().setVisible(false);
            return true;
        },
        "",
        "",
        "log-dont-show",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<bool> Disable(
        "Log/Disable",
        "Disable logs",
        false,
        [] (const intfConfigurable&, QString&){
            Logger::instance().setActive(false);
            return true;
        },
        "",
        "",
        "log-disable",
        enuConfigSource::Arg,
        false
        );

/******************************************************************************************/
Logger::Logger(QObject *parent) :
    QObject(parent),pPrivate(new Targoman::Common::Private::LoggerPrivate)
{
    this->pPrivate->LogSettings[enuLogType::Debug].setLevel(9);
    this->pPrivate->LogSettings[enuLogType::Error].setLevel(9);
    this->pPrivate->LogSettings[enuLogType::Warning].setLevel(9);
    this->pPrivate->LogSettings[enuLogType::Info].setLevel(9);
    this->pPrivate->LogSettings[enuLogType::Happy].setLevel(9);
    this->registerActor(&this->pPrivate->ActorUUID,"BaseLogger");
}

bool Logger::init(const QString &_fileName,
                  QMap<enuLogType::Type, clsLogSettings> _logSettings,
                  quint64 _maxSize,
                  bool _show)
{
    qRegisterMetaType<Targoman::Common::enuLogType::Type>("Targoman::Common::enuLogType::Type");
    this->pPrivate->LogFile.setFileName(_fileName.size() ? _fileName : "/dev/null");
    this->pPrivate->LogSettings[enuLogType::Debug] = _logSettings.value(enuLogType::Debug);
    this->pPrivate->LogSettings[enuLogType::Error] = _logSettings.value(enuLogType::Error);
    this->pPrivate->LogSettings[enuLogType::Warning] = _logSettings.value(enuLogType::Warning);
    this->pPrivate->LogSettings[enuLogType::Info] = _logSettings.value(enuLogType::Info);
    this->pPrivate->LogSettings[enuLogType::Happy] = _logSettings.value(enuLogType::Happy);
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
    if (this->isActive() == false || this->pPrivate->LogSettings[_type].canBeShown(_level) == false)
        return;

    QString Actor = this->pPrivate->Actors.value(_actorID);
    if (!_actorID.isEmpty() && Actor.isEmpty())
        throw exLogger(QString("Actor ID %1 Not registerd: message(%2)").arg(_actorID).arg(_message));

    QByteArray LogMessage=
            QString("[" + QDateTime().currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz") + "]").toUtf8();

    LogMessage+= QString("[%1]").arg(enuLogType::toStr(_type));
    LogMessage += "[" + QString::number(_level) + "]";

    LogMessage += QString("[%1] ").arg(Actor.isEmpty() ? "UNREG" : Actor);

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
    this->LogSettings = new clsLogSettings[enuLogType::getCount()];
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

