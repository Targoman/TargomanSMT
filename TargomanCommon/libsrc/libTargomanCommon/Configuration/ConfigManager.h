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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#ifndef TARGOMAN_COMMON_CONFIGMANAGER_H
#define TARGOMAN_COMMON_CONFIGMANAGER_H

#include <cfloat>
#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"

namespace Targoman {
namespace Common {
/**
 * @brief Configuration module with many interfaces and templates to ease application configuration
 */
namespace Configuration {

namespace Private {
class clsConfigManagerPrivate;
class intfConfigurablePrivate;
}

TARGOMAN_ADD_EXCEPTION_HANDLER(exConfiguration, exTargomanBase);

/**
 * @brief The ConfigManager class is the manager class for configurables data.
 * Currently it will just manage Arguments and config file
 */
class ConfigManager
{
public:
    ~ConfigManager();
    static inline ConfigManager& instance(){
        return *(Q_LIKELY(Instance) ? Instance : (Instance = new ConfigManager));}

    void init(const QString &_license, const QStringList &_arguments = QStringList());
    void save2File(const QString&  _fileName, bool _backup);
    void addConfig(const QString _path, intfConfigurable* _item);
    void addModuleInstantiaor(const QString _name, const stuInstantiator& _instantiator);
    QVariant getConfig(const QString& _path, const QVariant &_default = QVariant()) const;
    void setValue(const QString& _path, const QVariant &_value) const;
    fpModuleInstantiator_t getInstantiator(const QString& _name) const;
    QString configFilePath();

private:
    ConfigManager();
    Q_DISABLE_COPY(ConfigManager)

private:
    static ConfigManager* Instance;
    QScopedPointer<Private::clsConfigManagerPrivate> pPrivate;

    friend class Targoman::Common::Configuration::Private::intfConfigurablePrivate;
};

}
}
}
#endif // TARGOMAN_COMMON_CONFIGMANAGER_H
