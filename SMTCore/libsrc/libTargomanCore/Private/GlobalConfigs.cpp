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

#include "GlobalConfigs.h"
#include "Private/LanguageModel/intfLMSentenceScorer.hpp"

namespace Targoman {
namespace Core {
namespace Private{

using namespace Common;
using namespace Common::Configuration;

stuGlobalConfigs gConfigs;

tmplConfigurable<QString> stuGlobalConfigs::Separator(
        "/Common/Separator",
        "Separator used on multi field configurations",
        ";:;");
tmplConfigurable<QString> stuGlobalConfigs::SourceLanguage(
        "/Language/Source",
        "Source language from which translating",
        "en");
tmplConfigurable<QString> stuGlobalConfigs::TargetLanguage(
        "/Language/Destination",
        "Destination Language to which translating",
        "fa");

QMap<QString, FeatureFunction::intfFeatureFunction*>       stuGlobalConfigs::ActiveFeatureFunctions;

clsModuleConfig         stuGlobalConfigs::LM("/Modules/LM",
                                             "TODO Desc",
                                             "TargomanLM");
clsModuleConfig         stuGlobalConfigs::RuleTable("/Modules/RuleTable",
                                                    "TODO Desc",
                                                    "JanePTPlain");
Targoman::Core::Private::LanguageModel::intfLMSentenceScorer*  stuGlobalConfigs::EmptyLMScorer;

}
}
}
