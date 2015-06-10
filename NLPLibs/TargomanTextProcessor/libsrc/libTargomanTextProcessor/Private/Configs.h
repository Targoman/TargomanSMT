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

#ifndef TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_CONFIGS_H
#define TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_CONFIGS_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/clsFileBasedConfig.hpp"

namespace Targoman {
namespace NLPLibs {
namespace TargomanTP{
namespace Private {

struct stuConfigs{
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t> AbbreviationFile;
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t> NormalizationFile;
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t> SpellCorrectorBaseConfigPath;
    static Targoman::Common::Configuration::clsFileBasedConfig SpellCorrectorLanguageBasedConfigs;
}extern Configs;


}
}
}
}
#endif // TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_TTPCONFIGS_H
