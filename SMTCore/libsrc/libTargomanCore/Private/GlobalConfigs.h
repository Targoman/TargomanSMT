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
#include "libTargomanCommon/Types.h"
#include "Private/PrivateTypes.h"

namespace Targoman {
namespace Core {
namespace Private {

namespace FeatureFunction{
class intfFeatureFunction;
}

namespace Proxies {
class intfLMSentenceScorer;
}

// Global ActorUUID, back-off when the logging class is not a module or singlton
extern QString ActorUUID;

TARGOMAN_DEFINE_ENHANCED_ENUM(enuWorkingModes,
                              Decode,
                              Train,
                              DebugDecode,
                              DebugTrain);

struct stuGlobalConfigs{
    static Common::Configuration::tmplConfigurable<QString>                 Separator;
    static Common::Configuration::tmplConfigurable<QString>                 SourceLanguage;
    static Common::Configuration::tmplConfigurable<QString>                 TargetLanguage;
    static Common::Configuration::tmplConfigurable<enuWorkingModes::Type>   WorkingMode;

    static Common::Configuration::clsModuleConfig                           LM;
    static Common::Configuration::clsModuleConfig                           RuleTable;

    static QScopedPointer<Core::Private::Proxies::intfLMSentenceScorer>     EmptyLMScorer;
    static QHash<QString, Common::WordIndex_t>                              SourceVocab;

    static QMap<QString, FeatureFunction::intfFeatureFunction*>             ActiveFeatureFunctions;
};

extern stuGlobalConfigs gConfigs;

}
}

namespace Common {
namespace Configuration {
_SPECIAL_CONFIGURABLE(Core::Private::enuWorkingModes::Type)
}
}

}
#endif // TARGOMAN_CORE_PRIVATE_GLOBALCONFIGS_H
