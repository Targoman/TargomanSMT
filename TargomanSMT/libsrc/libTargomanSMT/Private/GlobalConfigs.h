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
#include "libTargomanCommon/Configuration/tmplModuleConfig.hpp"
#include "Translator.h"
#include "libTargomanCommon/Types.h"
#include "Private/PrivateTypes.h"

namespace Targoman {
namespace SMT {
namespace Private {

namespace FeatureFunction
{
class intfFeatureFunction;
}

namespace RuleTable {
class intfRuleTable;
}

namespace Proxies {
namespace LanguageModel {class intfLMSentenceScorer;}
namespace Transliteration {class intfTransliterator;}
namespace NamedEntityRecognition {class intfNamedEntityRecognizer;}
}

TARGOMAN_DEFINE_ENHANCED_ENUM(enuWorkingModes,
                              Decode,
                              Train,
                              DebugDecode,
                              DebugTrain);

struct stuGlobalConfigs{
    static Common::Configuration::tmplConfigurable<QString>                 SourceLanguage;
    static Common::Configuration::tmplConfigurable<QString>                 TargetLanguage;
    static Common::Configuration::tmplConfigurable<enuWorkingModes::Type>   WorkingMode;

    static Common::Configuration::tmplModuleConfig<Proxies::LanguageModel::intfLMSentenceScorer>     LM;
    static Common::Configuration::tmplModuleConfig<RuleTable::intfRuleTable>   RuleTable;
#ifndef SMT
    static Common::Configuration::tmplModuleConfig<Proxies::Transliteration::intfTransliterator> Transliterator;
    static Common::Configuration::tmplModuleConfig<Proxies::NamedEntityRecognition::intfNamedEntityRecognizer> NER;
#endif

    static QScopedPointer<SMT::Private::Proxies::LanguageModel::intfLMSentenceScorer>      EmptyLMScorer;
// There is no transliteration for anything but Statistical Machine Translation!
    static QHash<QString, Common::WordIndex_t>                              SourceVocab;
    static QSet<WordIndex_t>                                                VocabWithSingleWordRule;
    static QMap<QString, FeatureFunction::intfFeatureFunction*>             ActiveFeatureFunctions;
    static QString moduleName(){return "Common";}
};

extern stuGlobalConfigs gConfigs;

}
}
}

ENUM_CONFIGURABLE(SMT::Private::enuWorkingModes)

#endif // TARGOMAN_CORE_PRIVATE_GLOBALCONFIGS_H
