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

#ifndef TARGOMAN_CORE_PRIVATE_GLOBALCONFIGS_H
#define TARGOMAN_CORE_PRIVATE_GLOBALCONFIGS_H

#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/clsModuleConfig.hpp"
#include "libTargomanCore/clsTranslator.h"

namespace Targoman {
namespace Core {
namespace Private {

namespace FeatureFunction{
class intfFeatureFunction;
}

struct stuGlobalConfigs{
    static Targoman::Common::Configuration::tmplConfigurable<QString> Separator;
    static Targoman::Common::Configuration::tmplConfigurable<QString> SourceLanguage;
    static Targoman::Common::Configuration::tmplConfigurable<QString> TargetLanguage;


    static Targoman::Common::Configuration::tmplConfigurable<bool> KeepRecombined;

    static Targoman::Common::Configuration::clsModuleConfig          LM;
    static Targoman::Common::Configuration::clsModuleConfig          RuleTable;

    static QMap<QString, FeatureFunction::intfFeatureFunction*>       ActiveFeatureFunctions;
};

extern stuGlobalConfigs gConfigs;

}
}
}
#endif // TARGOMAN_CORE_PRIVATE_GLOBALCONFIGS_H
