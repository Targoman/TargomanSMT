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
namespace NLPLibs {
namespace TargomanTP{
namespace Private {

stuConfigs Configs;

Targoman::Common::Configuration::tmplConfigurable<QString> stuConfigs::AbbreviationFile(
        "TextProcessor/AbbreviationFile",
        "File path where abbreviations are stored",
        "");
Targoman::Common::Configuration::tmplConfigurable<QString> stuConfigs::NormalizationFile(
        "TextProcessor/NormalizationFile",
        "File path where normalization rules are stored",
        "");
Targoman::Common::Configuration::tmplConfigurable<QString> stuConfigs::SpellCorrectorBaseConfigPath(
        "TextProcessor/SpellCorrectorBaseConfigPath",
        "File path where spell correction dics are stored",
        "");
Targoman::Common::Configuration::clsFileBasedConfig       stuConfigs::SpellCorrectorLanguageBasedConfigs(
        "TextProcessor/SpellCorrectorLanguageBasedConfigs");

}
}
}
}
