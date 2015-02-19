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

#ifndef TARGOMAN_CONFIGS_H
#define TARGOMAN_CONFIGS_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "libTargomanCommon/Macros.h"


namespace Targoman {

TARGOMAN_DEFINE_ENHANCED_ENUM_BEGIN(enuAppMode,
                                    Text2IXML,
                                    Normalize,
                                    Tokenize,
                                    IXML2Text
                                    );

namespace Common{
    namespace Configuration {
    _SPECIAL_CONFIGURABLE(enuAppMode::Type)

    }
}

class gConfigs{
public:
    static inline QString appConfig(const QString& _name){
        return "/App/" + _name;
    }

    static Common::Configuration::tmplConfigurable<enuAppMode::Type> Mode;
    static Common::Configuration::tmplConfigurable<QString>          InputFile;
    static Common::Configuration::tmplConfigurable<QString>          InputDir;
    static Common::Configuration::tmplConfigurable<QString>          Output;
    static Common::Configuration::tmplConfigurable<QRegExp,true>     IncludePattern;
    static Common::Configuration::tmplConfigurable<QString>          DTDFilePath;
};

}
#endif // TARGOMAN_CONFIGS_H
