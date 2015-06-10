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

#ifndef TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSCONFIGURATION_P_H
#define TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSCONFIGURATION_P_H

#include <QHash>
#include <QVariant>
#include "Configuration/ConfigManager.h"

namespace Targoman {
namespace Common {
namespace Configuration {
/**
 *  @brief Private section of Targoman::Common module where internal functionalities needed to be obfuscated
 *  are defined
 */
namespace Private {

class clsConfigNetworkServer;

class clsConfigManagerPrivate : public QObject
{
    Q_OBJECT
public:
    clsConfigManagerPrivate(ConfigManager& _parent);
    ~clsConfigManagerPrivate();
    void printHelp(const QString &_license, bool _minimal);
    QList<intfConfigurable*> configItems(const QString& _parent, bool _isRegEX, bool _reportRemote);
    void prepareServer();
    bool isNetworkBased();
    void startNetworkListening();

public:
    /**
     * @brief This is a registry (Map) for all configs and arguments of all programs such as normalizer, the key of
     * this Map, specifies the program and option of that program, and value of this Map, specifies the value of that
     * option.
     */
    QHash<QString, Configuration::intfConfigurable*>    Configs;

    /**
     * @brief ModuleInstantiators
     */
    QHash<QString, stuInstantiator>                     ModuleInstantiators;

    /**
     * @brief In case that we have a config file (.ini file) for arguments of our programs, we put the address of
     * that config file in #ConfigFilePath
     */
    QString                              ConfigFilePath;
    QString                              ConfigFileDir;
    /**
     * @brief This variable will be true if init() function of Configuration class works without any exceptions.
     */
    bool Initialized;

    bool SetPathsRelativeToConfigPath;

    QString ActorUUID;

    ConfigManager& Parent;
    QScopedPointer<clsConfigNetworkServer> ConfigNetServer;
};

}
}
}
}

#endif // TARGOMAN_COMMON_CONFIGURATION_PRIVATE_CLSCONFIGURATION_P_H
