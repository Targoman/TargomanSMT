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

#ifndef TARGOMAN_NLPLIBS_TEXTPROCESSOR_PRIVATE_CONFIGS_H
#define TARGOMAN_NLPLIBS_TEXTPROCESSOR_PRIVATE_CONFIGS_H

#include "libTargomanCommon/Configuration.h"

namespace Targoman {
namespace NLPLibs {
namespace TextProcessor_{
namespace Private {

struct stuConfigs{
    static Targoman::Common::clsConfigurable<QString> AbbreviationFile;
    static Targoman::Common::clsConfigurable<QString> NormalizationFile;
    static Targoman::Common::clsConfigurable<QString> SpellCorrectorBaseConfigPath;
    static Targoman::Common::clsFileBasedConfig SpellCorrectorLanguageBasedConfigs;
}extern Configs;


}
}
}
}
#endif // TARGOMAN_NLPLIBS_TEXTPROCESSOR_PRIVATE_TTPCONFIGS_H
