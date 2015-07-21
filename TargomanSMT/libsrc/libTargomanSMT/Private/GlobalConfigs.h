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

#ifndef TARGOMAN_CORE_PRIVATE_GLOBALCONFIGS_H
#define TARGOMAN_CORE_PRIVATE_GLOBALCONFIGS_H

#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/clsModuleConfig.hpp"
#include "Translator.h"
#include "libTargomanCommon/Types.h"
#include "Private/PrivateTypes.h"

namespace Targoman {
namespace SMT {
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

    static QScopedPointer<SMT::Private::Proxies::intfLMSentenceScorer>      EmptyLMScorer;
    static QHash<QString, Common::WordIndex_t>                              SourceVocab;

    static QMap<QString, FeatureFunction::intfFeatureFunction*>             ActiveFeatureFunctions;
};

extern stuGlobalConfigs gConfigs;

}
}
}

ENUM_CONFIGURABLE(SMT::Private::enuWorkingModes)

#endif // TARGOMAN_CORE_PRIVATE_GLOBALCONFIGS_H
