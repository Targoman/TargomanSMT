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
#include "Configuration/tmplConfigurable.h"

#include "Macros.h"
#include "CmdIO.h"

typedef std::string (*delGetTargomanLibVersion)();

namespace Targoman {
namespace Common {

using namespace Configuration;

tmplConfigurable<bool> Silent(
        "IO/Silent",
        "Set output off",
        false,
        [] (const intfConfigurable&, QString&){
            TARGOMAN_IO_SETTINGS.setSilent();
            Logger::instance().setActive(false);
            return true;
        },
        "",
        "",
        "io-silent",
        enuConfigSource::Arg
        );

tmplConfigurable<bool> Full(
        "IO/Full",
        "Set output to full mode",
        false,
        [] (const intfConfigurable&, QString&){TARGOMAN_IO_SETTINGS.setFull(); return true;},
        "",
        "",
        "io-full",
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<QStringList> DebugDetail(
        "IO/DebugDetail",
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
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<quint8> DebugLevel(
        "IO/DebugLevel",
        "Set Debug level",
        0,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Debug.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "debug-level",
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<QStringList> InfoDetail(
        "IO/InfoDetail",
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
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<quint8> InfoLevel(
        "IO/InfoLevel",
        "Set Info level",
        5,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Info.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "info-level",
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<QStringList> WarningDetail(
        "IO/WarningDetail",
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
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<quint8> WarningLevel(
        "IO/WarningLevel",
        "Set Warning level",
        5,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Warning.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "warning-level",
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<QStringList> ErrorDetail(
        "IO/ErrorDetail",
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
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<QStringList> HappyDetail(
        "IO/HappyDetail",
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
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<quint8> HappyLevel(
        "IO/HappyLevel",
        "Set Happy level",
        5,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Happy.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "happy-level",
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<QStringList> NormalDetail(
        "IO/NormalDetail",
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
        enuConfigSource::Arg,
        false
        );

tmplConfigurable<quint8> NormalLevel(
        "IO/NormalLevel",
        "Set Normal level",
        5,
        [] (const intfConfigurable& _item, QString&){
            TARGOMAN_IO_SETTINGS.Normal.setLevel(_item.toVariant().toUInt());
            return true;
        },
        "",
        "LEVEL",
        "normal-level",
        enuConfigSource::Arg,
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


}
}


