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

#ifndef TARGOMAN_APPS_CONFIGS_H
#define TARGOMAN_APPS_CONFIGS_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "libTargomanCommon/Macros.h"


namespace Targoman {
namespace Apps {

TARGOMAN_ADD_EXCEPTION_HANDLER(exAppE4SMT, Common::exTargomanBase);

TARGOMAN_DEFINE_ENHANCED_ENUM(enuAppMode,
                              Text2IXML,
                              Normalize,
                              Tokenize,
                              IXML2Text,
                              Validate,
                              Server
                              );
}
}

ENUM_CONFIGURABLE(Targoman::Apps::enuAppMode);

namespace Targoman {
namespace Apps {

class gConfigs{
public:
    static inline QString appConfig(const QString& _name){
        return "/App/" + _name;
    }

    static Common::Configuration::tmplConfigurable<enuAppMode::Type>    Mode;
    static Common::Configuration::tmplConfigurable<QString>             Input;
    static Common::Configuration::tmplConfigurable<QString>             Language;
    static Common::Configuration::tmplConfigurable<FilePath_t>          InputFile;
    static Common::Configuration::tmplConfigurable<FilePath_t>          InputDir;
    static Common::Configuration::tmplConfigurable<FilePath_t>          OutputDir;
    static Common::Configuration::tmplConfigurable<QRegExp,true>        IncludePattern;
    static Common::Configuration::tmplConfigurable<FilePath_t>          DTDFilePath;
    static Common::Configuration::tmplConfigurable<bool>                Overwrite;
    static Common::Configuration::tmplConfigurable<bool>                KeepTitles;
    static Common::Configuration::tmplConfigurable<bool>                PlainText;
    static Common::Configuration::tmplConfigurable<bool>                Interactive;
    static Common::Configuration::tmplConfigurable<bool>                NoSpellcorrector;
    static Common::Configuration::tmplConfigurable<bool>                Recursive;
};

}
}
#endif // TARGOMAN_APPS_CONFIGS_H
