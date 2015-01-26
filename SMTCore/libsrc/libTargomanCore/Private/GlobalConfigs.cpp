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

#include "GlobalConfigs.h"

namespace Targoman {
namespace Core {
namespace Private{

using namespace Common;

stuGlobalConfigs gConfigs;

clsConfigurable<QString> stuGlobalConfigs::Separator("/Common/Separator",
                                            "Separator used on multi field configurations",
                                            ";:;");
clsConfigurable<QString> stuGlobalConfigs::SourceLanguage("/Language/Source",
                                            "Source language from which translating",
                                            "en");
clsConfigurable<QString> stuGlobalConfigs::TargetLanguage("/Language/Destination",
                                                          "Destination Language to which translating",
                                                          "fa");

LanguageModel::intfLMSentenceScorer *stuGlobalConfigs::newLMScorerInstance()
{
    //TODO
}

}
}
}
