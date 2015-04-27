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
#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/JSONConversationProtocol.h"

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
class ConfigManager : public QObject
{
    Q_OBJECT
public:
    ~ConfigManager();
    static inline ConfigManager& instance(){
        static ConfigManager* Instance = NULL;
        return *(Q_LIKELY(Instance) ? Instance : (Instance = new ConfigManager));
    }

    void init(const QString &_license, const QStringList &_arguments = QStringList());
    void save2File(const QString&  _fileName, bool _backup);
    void addConfig(const QString _path, intfConfigurable* _item);
    void addModuleInstantiaor(const QString _name, const stuInstantiator& _instantiator);
    QVariant getConfig(const QString& _path, const QVariant &_default = QVariant()) const;
    void setValue(const QString& _path, const QVariant &_value) const;
    fpModuleInstantiator_t getInstantiator(const QString& _name) const;
    QString configFilePath();
    void updateRelativePaths(QString &_path);
    bool isNetworkManagable();

public slots:
    void startAdminServer();

private:
    ConfigManager();
    Q_DISABLE_COPY(ConfigManager)

signals:
    void sigValidateAgent(INOUT QString&        _user,
                          const QString&        _pass,
                          const QString&        _ip,
                          OUTPUT bool&          _canView,
                          OUTPUT bool&          _canChange);

    void sigRPC(const QString&            _funcName,
                INOUT  QVariantMap&       _arguments,
                OUTPUT QVariant&          _returnVal);
    void sigPing(JSONConversationProtocol::stuPong& _pong);

private:
    QScopedPointer<Private::clsConfigManagerPrivate> pPrivate;

    friend class Targoman::Common::Configuration::Private::intfConfigurablePrivate;
};

}
}
}
#endif // TARGOMAN_COMMON_CONFIGMANAGER_H
