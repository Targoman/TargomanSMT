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

#include <QtGlobal>
#include <QString>
#include <link.h>
#include <iostream>
#include "Configuration/tmplConfigurable.h"

#include "Macros.h"
#include "CmdIO.h"

typedef std::string (*delGetTargomanLibVersion)();

namespace Targoman {
namespace Common {

using namespace Configuration;

tmplConfigurable<bool> Silent(
        CmdIO::moduleName() + "/Silent",
        "Set output off",
        false,
        [] (const intfConfigurable& _item, QString&){
            if(_item.toVariant().toBool()){
                TARGOMAN_IO_SETTINGS.setSilent();
                Logger::instance().setActive(false);
            }
            return true;
        },
        "",
        "",
        "io-silent",
        enuConfigSource::Arg
        );

tmplConfigurable<bool> Full(
        CmdIO::moduleName() + "/Full",
        "Set output to full mode",
        false,
        [] (const intfConfigurable& _item, QString&){
            if(_item.toVariant().toBool())
                TARGOMAN_IO_SETTINGS.setFull();
            return true;
        },
        "",
        "",
        "io-full",
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<QStringList> DebugDetail(
        CmdIO::moduleName() + "/DebugDetail",
        "Set Details to be shown for debug",
        QStringList()<<"true"<<"true"<<"true"<<"false",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            QStringList Params = _item.toVariant().toString().split(",");
            if (Params.size() < 4){
                _errorMessage = "Invalid count of arguments for " + _item.configPath();
                return false;
            }
            TARGOMAN_IO_SETTINGS.Debug.setDetails(QVariant(Params.at(0)).toBool(),
                                                  QVariant(Params.at(1)).toBool(),
                                                  QVariant(Params.at(2)).toBool(),
                                                  QVariant(Params.at(3)).toBool());
            return true;
        },
        "",
        "SHOW_TIME,SHOW_FUNC,SHOW_LINE,SHOW_FILE",
        "debug-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<quint8> DebugLevel(
        CmdIO::moduleName() + "/DebugLevel",
        "Set Debug level",
        0,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Debug.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "debug-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> InfoDetail(
        CmdIO::moduleName() + "/InfoDetail",
        "Set Details to be shown for Info",
        QStringList()<<"true"<<"true"<<"true"<<"false",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            QStringList Params = _item.toVariant().toString().split(",");
            if (Params.size() < 4){
                _errorMessage = "Invalid count of arguments for " + _item.configPath();
                return false;
            }
            TARGOMAN_IO_SETTINGS.Info.setDetails(QVariant(Params.at(0)).toBool(),
                                                 QVariant(Params.at(1)).toBool(),
                                                 QVariant(Params.at(2)).toBool(),
                                                 QVariant(Params.at(3)).toBool());
            return true;
        },
        "",
        "SHOW_TIME,SHOW_FUNC,SHOW_LINE,SHOW_FILE",
        "info-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<quint8> InfoLevel(
        CmdIO::moduleName() + "/InfoLevel",
        "Set Info level",
        5,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Info.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "info-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> WarningDetail(
        CmdIO::moduleName() + "/WarningDetail",
        "Set Details to be shown for Warning",
        QStringList()<<"true"<<"true"<<"true"<<"false",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            QStringList Params = _item.toVariant().toString().split(",");
            if (Params.size() < 4){
                _errorMessage = "Invalid count of arguments for " + _item.configPath();
                return false;
            }
            TARGOMAN_IO_SETTINGS.Warning.setDetails(QVariant(Params.at(0)).toBool(),
                                                    QVariant(Params.at(1)).toBool(),
                                                    QVariant(Params.at(2)).toBool(),
                                                    QVariant(Params.at(3)).toBool());
            return true;
        },
        "",
        "SHOW_TIME,SHOW_FUNC,SHOW_LINE,SHOW_FILE",
        "warning-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<quint8> WarningLevel(
        CmdIO::moduleName() + "/WarningLevel",
        "Set Warning level",
        5,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Warning.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "warning-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> ErrorDetail(
        CmdIO::moduleName() + "/ErrorDetail",
        "Set Details to be shown for Error",
        QStringList()<<"true"<<"true"<<"true"<<"false",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            QStringList Params = _item.toVariant().toString().split(",");
            if (Params.size() < 4){
                _errorMessage = "Invalid count of arguments for " + _item.configPath();
                return false;
            }
            TARGOMAN_IO_SETTINGS.Error.setDetails(QVariant(Params.at(0)).toBool(),
                                                  QVariant(Params.at(1)).toBool(),
                                                  QVariant(Params.at(2)).toBool(),
                                                  QVariant(Params.at(3)).toBool());
            return true;
        },
        "",
        "SHOW_TIME,SHOW_FUNC,SHOW_LINE,SHOW_FILE",
        "error-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> HappyDetail(
        CmdIO::moduleName() + "/HappyDetail",
        "Set Details to be shown for Happy",
        QStringList()<<"true"<<"true"<<"true"<<"false",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            QStringList Params = _item.toVariant().toString().split(",");
            if (Params.size() < 4){
                _errorMessage = "Invalid count of arguments for " + _item.configPath();
                return false;
            }
            TARGOMAN_IO_SETTINGS.Happy.setDetails(QVariant(Params.at(0)).toBool(),
                                                  QVariant(Params.at(1)).toBool(),
                                                  QVariant(Params.at(2)).toBool(),
                                                  QVariant(Params.at(3)).toBool());
            return true;
        },
        "",
        "SHOW_TIME,SHOW_FUNC,SHOW_LINE,SHOW_FILE",
        "happy-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<quint8> HappyLevel(
        CmdIO::moduleName() + "/HappyLevel",
        "Set Happy level",
        5,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Happy.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "happy-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> NormalDetail(
        CmdIO::moduleName() + "/NormalDetail",
        "Set Details to be shown for Normal",
        QStringList()<<"true"<<"true"<<"true"<<"false",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            QStringList Params = _item.toVariant().toString().split(",");
            if (Params.size() < 4){
                _errorMessage = "Invalid count of arguments for " + _item.configPath();
                return false;
            }
            TARGOMAN_IO_SETTINGS.Normal.setDetails(QVariant(Params.at(0)).toBool(),
                                                   QVariant(Params.at(1)).toBool(),
                                                   QVariant(Params.at(2)).toBool(),
                                                   QVariant(Params.at(3)).toBool());
            return true;
        },
        "",
        "SHOW_TIME,SHOW_FUNC,SHOW_LINE,SHOW_FILE",
        "normal-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<quint8> NormalLevel(
        CmdIO::moduleName() + "/NormalLevel",
        "Set Normal level",
        5,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Normal.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "normal-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

/******************************************************************************************/
int targomanLinkedLibrariesCallback(struct dl_phdr_info *_info, size_t _size, void *_data)
{
    Q_UNUSED(_size)
    Q_UNUSED(_data)

    delGetTargomanLibVersion getTargomanLibVersion;

    QString LibName = _info->dlpi_name;
    if (LibName.isEmpty())
        return 0;

    void* Handle = dlopen (_info->dlpi_name, RTLD_LAZY);
    dlerror();    /* Clear any existing error */
    *(void**) (&getTargomanLibVersion) = dlsym (Handle, "getTargomanLibVersion");

    if (dlerror() != NULL)
        qDebug("System Lib: %s",_info->dlpi_name);
    else if (getTargomanLibVersion)
        qDebug("  Targoman  Lib: %s : %s",_info->dlpi_name, getTargomanLibVersion().c_str());
    else
        qDebug("Loaded System Lib: %s",_info->dlpi_name);

    dlclose(Handle);
    return 0;
}

void dummyPrintLoadedLibs()
{
}

/**
 * @brief prints a list of loaded system and Targoman libraries.
 */
void printLoadedLibs()
{
    if (TARGOMAN_IO_SETTINGS.Debug.canBeShown(5) == false)
        return;

    dl_iterate_phdr(targomanLinkedLibrariesCallback, NULL);
}

clsIOSettings TARGOMAN_IO_SETTINGS;

QString CmdIO::getPassword(const QString& _message, char _replacingChar)
{
    std::cout<<_message.toUtf8().constData()<<":"<<std::flush;
    QByteArray Data;
    char Ch = getch();
    while(Ch != '\n'){
        Data.append(Ch);
        if (_replacingChar != '\0')
            std::cout<<_replacingChar<<std::flush;
        Ch = getch();
    }
    std::cout<<std::endl;
    return QString::fromUtf8(Data);
}



}
}

#ifdef TargomanLinuxGetCH
//Following code was copied from http://stackoverflow.com/questions/7469139/what-is-equivalent-to-getch-getche-in-linux
static struct termios Old, New;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
  tcgetattr(0, &Old); /* grab old terminal i/o settings */
  New = Old; /* make new settings same as old settings */
  New.c_lflag &= ~ICANON; /* disable buffered i/o */
  New.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &New); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &Old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void)
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void)
{
  return getch_(1);
}
#endif

