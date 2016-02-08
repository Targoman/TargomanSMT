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

int targomanLinkedLibrariesCallback(struct dl_phdr_info *_info, size_t _size, void *_data);

using namespace Configuration;

tmplConfigurable<bool> Silent(
        clsConfigPath(CmdIO::moduleName() + "/" + "Silent"),
        "Set output off",
        false,
        ReturnTrueCrossValidator,
        "",
        "",
        "out-silent",
        enuConfigSource::Arg,
        false,
        [] (const intfConfigurable& _item){
            if(_item.toVariant().toBool()){
                TARGOMAN_IO_SETTINGS.setSilent();
                Logger::instance().setVisible(false);
            }
            return true;
        }
        );

tmplConfigurable<bool> Full(
        clsConfigPath(CmdIO::moduleName() + "/" + "Full"),
        "Set output to full mode",
        false,
        ReturnTrueCrossValidator,
        "",
        "",
        "out-full",
        enuConfigSource::Arg,
        false,
        [] (const intfConfigurable& _item){
            if(_item.toVariant().toBool())
                TARGOMAN_IO_SETTINGS.setFull();
            return true;
        }
        );

tmplConfigurable<QStringList> DebugDetail(
        clsConfigPath(CmdIO::moduleName() + "/" + "DebugDetail"),
        "Set details to be shown for debug. {SHOW_TIME,SHOW_FUNC,SHOW_LINE,SHOW_FILE}",
        QStringList()<<"true"<<"false"<<"false"<<"false",
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
        "out-debug-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplRangedConfigurable<quint8> DebugLevel(
        clsConfigPath(CmdIO::moduleName() + "/" + "DebugLevel"),
        "Set Debug level",
        0,9,
        0,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Debug.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "out-debug-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> InfoDetail(
        clsConfigPath(CmdIO::moduleName() + "/" + "InfoDetail"),
        "Set details to be shown for Info. {SHOW_TIME,SHOW_FUNC,SHOW_LINE,SHOW_FILE}",
        QStringList()<<"true"<<"false"<<"false"<<"false",
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
        "out-info-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplRangedConfigurable<quint8> InfoLevel(
        clsConfigPath(CmdIO::moduleName() + "/" + "InfoLevel"),
        "Set Info level",
        0,9,
        5,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Info.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "out-info-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> WarningDetail(
        clsConfigPath(CmdIO::moduleName() + "/" + "WarningDetail"),
        "Set details to be shown for Warning. {SHOW_TIME,SHOW_FUNC,SHOW_LINE,SHOW_FILE}",
        QStringList()<<"true"<<"false"<<"false"<<"false",
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
        "out-warning-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplRangedConfigurable<quint8> WarningLevel(
        clsConfigPath(CmdIO::moduleName() + "/" + "WarningLevel"),
        "Set Warning level",
        0,9,
        5,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Warning.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "out-warning-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> ErrorDetail(
        clsConfigPath(CmdIO::moduleName() + "/" + "ErrorDetail"),
        "Set details to be shown for Error. {SHOW_TIME,SHOW_FUNC,SHOW_LINE,SHOW_FILE}",
        QStringList()<<"true"<<"false"<<"false"<<"false",
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
        "out-error-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> HappyDetail(
        clsConfigPath(CmdIO::moduleName() + "/" + "HappyDetail"),
        "Set details to be shown for Happy. {SHOW_TIME,SHOW_FUNC,SHOW_LINE,SHOW_FILE}",
        QStringList()<<"true"<<"false"<<"false"<<"false",
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
        "out-happy-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplRangedConfigurable<quint8> HappyLevel(
        clsConfigPath(CmdIO::moduleName() + "/" + "HappyLevel"),
        "Set Happy level",
        0,9,
        5,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Happy.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "out-happy-level",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplConfigurable<QStringList> NormalDetail(
        clsConfigPath(CmdIO::moduleName() + "/" + "NormalDetail"),
        "Set details to be shown for Normal. {SHOW_TIME,SHOW_FUNC,SHOW_LINE,SHOW_FILE}",
        QStringList()<<"true"<<"false"<<"false"<<"false",
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
        "out-normal-details",
        (enuConfigSource::Type)(enuConfigSource::Arg | enuConfigSource::File),
        false
        );

tmplRangedConfigurable<quint8> NormalLevel(
        clsConfigPath(CmdIO::moduleName() + "/" + "NormalLevel"),
        "Set Normal level",
        0,9,
        5,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Normal.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "out-normal-level",
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
        fprintf(stderr,"System Lib: %s\n",_info->dlpi_name);
    else if (getTargomanLibVersion)
        fprintf(stderr,"  Targoman Lib: %s : %s\n",_info->dlpi_name, getTargomanLibVersion().c_str());
    else
        fprintf(stderr,"Loaded System Lib: %s\n",_info->dlpi_name);

    dlclose(Handle);
    return 0;
}

/**
 * @brief prints a list of loaded system and Targoman libraries.
 */
void printLoadedLibs()
{
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

