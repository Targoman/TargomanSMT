/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
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

#define CLASS_MODULE Logger

tmplConfigurable<QString> LogFile(
        clsConfigPath(Logger::moduleName() + "/" + "File"),
        "Log File to store logs. If not set then no log file will be generated. Relative to execution path unless specified as absolute path.",
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
        clsConfigPath(Logger::moduleName() + "/" + "DontShow"),
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
        clsConfigPath(Logger::moduleName() + "/" + "Disable"),
        "Disable logging module",
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
        clsConfigPath(Logger::moduleName() + "/" + "DebugDetail"),
        "Set details of debugging logs. {SHOW_TIME,SHOW_ACTOR}",
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

tmplRangedConfigurable<quint8> LogDebugLevel(
        clsConfigPath(Logger::moduleName() + "/" + "DebugLevel"),
        "Set debug log level",
        0,9,
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
        clsConfigPath(Logger::moduleName() + "/" + "WarningDetail"),
        "Set details of warning logs. {SHOW_TIME,SHOW_ACTOR}",
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

tmplRangedConfigurable<quint8> LogWarningLevel(
        clsConfigPath(Logger::moduleName() + "/" + "WarningLevel"),
        "Set warning log level",
        0,9,
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
        clsConfigPath(Logger::moduleName() + "/" + "ErrorDetail"),
        "Set details of error logs. {SHOW_TIME,SHOW_ACTOR}",
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
        clsConfigPath(Logger::moduleName() + "/" + "InfoDetail"),
        "Set details of informative Logs. {SHOW_TIME,SHOW_ACTOR}",
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

tmplRangedConfigurable<quint8> LogInfoLevel(
        clsConfigPath(Logger::moduleName() + "/" + "InfoLevel"),
        "Set informative log level",
        0,9,
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
        clsConfigPath(Logger::moduleName() + "/" + "HappyDetail"),
        "Set details of happy logs. {SHOW_TIME,SHOW_ACTOR}",
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

tmplRangedConfigurable<quint8> LogHappyLevel(
        clsConfigPath(Logger::moduleName() + "/" + "HappyLevel"),
        "Set happy log level",
        0,9,
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
{}

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


void Logger::write(const QString &_callerFuncName,
                   enuLogType::Type _type,
                   quint8 _level,
                   const QString &_message,
                   bool _newLine)
{
    if (this->isActive() == false || LogSettings[_type].canBeShown(_level) == false)
        return;

    QByteArray LogMessage= LogSettings[_type].details(_callerFuncName).toLatin1();

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
    emit this->sigLogAdded(QDateTime().currentDateTime(), _callerFuncName, _type, _level, _message);
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

QString clsLogSettings::getPrettyModuleName(const QString &_callerFuncName)
{
    thread_local static QRegExp RxPrivate("::Private$");
    QString ModuleName = _callerFuncName;
    int ParenthesisLoc = ModuleName.indexOf('(');
    if (ParenthesisLoc >=0 )
        ModuleName.truncate(ParenthesisLoc);
    int SpaceLoc = ModuleName.lastIndexOf(" ");
    ModuleName = (SpaceLoc >= 0 ? ModuleName.mid(SpaceLoc + 1) : ModuleName);
    ModuleName = ModuleName.mid(0, ModuleName.lastIndexOf("::"));
    if(ModuleName.contains("cls"))
        ModuleName = ModuleName.mid(0, ModuleName.lastIndexOf("::"));
    if(ModuleName.contains("intf"))
        ModuleName = ModuleName.mid(0, ModuleName.lastIndexOf("::"));
    ModuleName.replace("::Private::", "::");
    ModuleName.replace(RxPrivate, "");
    return ModuleName;
}

}
}

