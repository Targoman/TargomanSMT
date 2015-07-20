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

#include "Configs.h"
#include "libTargomanCommon/Configuration/Validators.hpp"

namespace Targoman {
namespace Apps {

using namespace Common;
using namespace Common::Configuration;

gConfigs::stuServer::stuServer(const QString &_basePath) :
    Host(_basePath + "Host",
         "Host or IP Address of Server",
         "",
         Common::Configuration::ReturnTrueCrossValidator,
         "","","",
         Common::Configuration::enuConfigSource::File
         ),
    Port(_basePath + "Port",
         "Server port to connect to",
         1,
         Common::Configuration::ReturnTrueCrossValidator,
         "","","",
         Common::Configuration::enuConfigSource::File
         ),
    AbsoluteScore(_basePath + "AbsoluteScore",
         "Absolute score of Server used in priority computations",
        0,
         Validators::tmplNumericValidator<quint8,0,100>,
         "","","",
         Common::Configuration::enuConfigSource::File
         ),
    Active(_basePath + "Active",
         "Host or IP Address of Server",
         false,
         Common::Configuration::ReturnTrueCrossValidator,
         "","","",
         Common::Configuration::enuConfigSource::File
         ),
    UserName(_basePath + "Username",
         "Administration user",
         "TargomanLB",
         Common::Configuration::ReturnTrueCrossValidator,
         "","","",
         Common::Configuration::enuConfigSource::File,
         false
         ),
    Password(_basePath + "Password",
         "Administration password",
         "",
         Common::Configuration::ReturnTrueCrossValidator,
         "","","",
         Common::Configuration::enuConfigSource::File,
         false
         ),
    Statistics(gConfigs::stuServer::stuStatistics(_basePath))
{}

gConfigs::stuServer::stuStatistics::stuStatistics(const QString &_basePath) :
    Load1MinPercent(_basePath + "Stats/Load1Min",
                    "Load average on 1min basis divided by count of cores",
                    -1,
                    Common::Configuration::ReturnTrueCrossValidator,
                    "","","",
                    Common::Configuration::enuConfigSource::ReadOnly
                    ),
    Load15MinPercent(_basePath + "Stats/Load15Min",
                     "Load average on 15min basis divided by count of cores",
                     -1,
                     Common::Configuration::ReturnTrueCrossValidator,
                     "","","",
                     Common::Configuration::enuConfigSource::ReadOnly
                     ),
    FreeMemoryPercent(_basePath + "Stats/FreeMemory",
                      "FreeMemory Available on server divided by total amount of memory",
                      -1,
                      Common::Configuration::ReturnTrueCrossValidator,
                      "","","",
                      Common::Configuration::enuConfigSource::ReadOnly
                      ),
    TranslationQueuePercent(_basePath + "Stats/TranslationQueue",
                            "Translation Queue usage",
                            -1,
                            Common::Configuration::ReturnTrueCrossValidator,
                            "","","",
                            Common::Configuration::enuConfigSource::ReadOnly
                            )
  {}

tmplConfigurableArray<gConfigs::stuServer> gConfigs::TranslationServers(
        "TranslationServers",
        "List of valid translation servers to connect to them",
        1);

QString ActorUUID;
}
}
