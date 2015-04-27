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
         "",
         Common::Configuration::ReturnTrueCrossValidator,
         "","","",
         Common::Configuration::enuConfigSource::File
         ),
    AbsoluteScore(_basePath + "AbsoluteScore",
         "Absolute score of Server used in priority computations",
         "",
         [] (const Common::Configuration::intfConfigurable& _item, QString&) {
            return _item.toVariant().toUInt() <= 100;
         },
         "","","",
         Common::Configuration::enuConfigSource::File
         ),
    Active(_basePath + "Active",
         "Host or IP Address of Server",
         "",
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
         )

  {}

tmplConfigurableArray<gConfigs::stuServer> gConfigs::TranslationServers(
        "TranslationServers",
        "List of valid translation servers to connect to them");

}
}
