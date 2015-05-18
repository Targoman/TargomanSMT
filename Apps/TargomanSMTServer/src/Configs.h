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

#ifndef TARGOMAN_APPS_TARGOMANSMTCONSOLE_CONFIGS_H
#define TARGOMAN_APPS_TARGOMANSMTCONSOLE_CONFIGS_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman {
namespace Apps {

TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanSMTServer, Common::exTargomanBase);

class gConfigs{
public:
    static inline QString appConfig(const QString& _name){
        return "App/" + _name;
    }

    static Common::Configuration::tmplConfigurable<QString>          InputFile;
    static Common::Configuration::tmplConfigurable<QString>          InputText;
    static Common::Configuration::tmplConfigurable<QString>          OutputFile;
    static Common::Configuration::tmplConfigurable<quint8>           MaxThreads;
};

}
}
#endif // TARGOMAN_APPS_TARGOMANSMTCONSOLE_CONFIGS_H
