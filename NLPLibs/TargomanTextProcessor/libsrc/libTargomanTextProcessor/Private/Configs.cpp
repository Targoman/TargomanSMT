/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include "Configs.h"
#include "libTargomanCommon/Configuration/Validators.hpp"

namespace Targoman {
namespace NLPLibs {
namespace TargomanTP{
namespace Private {

using namespace Common;
using namespace Common::Configuration;
stuConfigs Configs;

tmplConfigurable<FilePath_t> stuConfigs::AbbreviationFile(
        MAKE_CONFIG_PATH("AbbreviationFile"),
        "File path where abbreviations are stored",
        "",
        Validators::tmplPathAccessValidator<
        (enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable),
        false>
        );
tmplConfigurable<FilePath_t> stuConfigs::NormalizationFile(
        MAKE_CONFIG_PATH("NormalizationFile"),
        "File path where normalization rules are stored",
        "",
        Validators::tmplPathAccessValidator<
        (enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable),
        false>
        );
tmplConfigurable<FilePath_t> stuConfigs::SpellCorrectorBaseConfigPath(
        MAKE_CONFIG_PATH("SpellCorrectorBaseConfigPath"),
        "File path where spell correction dics are stored",
        "",
        Validators::tmplPathAccessValidator<
        (enuPathAccess::Type)(enuPathAccess::Dir | enuPathAccess::Readable),
        false>
        );
clsFileBasedConfig       stuConfigs::SpellCorrectorLanguageBasedConfigs(
        MAKE_CONFIG_PATH("SpellCorrectorLanguageBasedConfigs")
        );

}
}
}
}
