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

#ifndef TARGOMAN_APPS_TARGOMANLOADBALANACER_CONFIGS_H
#define TARGOMAN_APPS_TARGOMANLOADBALANACER_CONFIGS_H

#include <QtNetwork/QHostAddress>
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/tmplConfigurableArray.hpp"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman {
namespace Apps {

TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanLoadBalancer, Common::exTargomanBase);

extern QString ActorUUID;
class gConfigs{
public:
    struct stuServer{
        stuServer(const QString& _basePath);
        Common::Configuration::tmplConfigurable<QString> Host;
        Common::Configuration::tmplConfigurable<quint16> Port;
        Common::Configuration::tmplConfigurable<quint8>  AbsoluteScore;
        Common::Configuration::tmplConfigurable<bool>    Active;
        Common::Configuration::tmplConfigurable<QString> UserName;
        Common::Configuration::tmplConfigurable<QString> Password;
        struct stuStatistics{
            stuStatistics(const QString& _basePath);
            Common::Configuration::tmplConfigurable<qint8> Load1MinPercent;
            Common::Configuration::tmplConfigurable<qint8> Load15MinPercent;
            Common::Configuration::tmplConfigurable<qint8> FreeMemoryPercent;
            Common::Configuration::tmplConfigurable<qint8> TranslationQueuePercent;
        }Statistics;
    };

public:
    static Common::Configuration::tmplConfigurableArray<stuServer> TranslationServers;
};

}
}
#endif // TARGOMAN_APPS_TARGOMANLOADBALANACER_CONFIGS_H
