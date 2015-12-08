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

#include "GlobalConfigs.h"
#include "ISO639.h"

#include "Private/Proxies/LanguageModel/intfLMSentenceScorer.hpp"
#include "Private/Proxies/Transliteration/intfTransliterator.h"
#include "RuleTable/intfRuleTable.hpp"

namespace Targoman {
namespace SMT {
/**
 * @brief Private section of TargomanSMT library which obfuscates all internal namespaces and classes
 */
namespace Private{

using namespace Common;
using namespace Common::Configuration;
using namespace Proxies::LanguageModel;

QString ActorUUID;

stuGlobalConfigs gConfigs;

tmplConfigurable<QString> stuGlobalConfigs::SourceLanguage(
        MAKE_CONFIG_PATH("Language/Source"),
        "Source language from which translating",
        "en",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            if (ISO639isValid(_item.toVariant().toString().toLatin1().constData()))
                return true;
            _errorMessage = "Invalid ISO639 Code ("+_item.toVariant().toString() +") for Source Language";
            return false;
        }
#ifndef SMT
        ,"sl",
        "SOURCE_LANG",
        "source-lang"
#endif
);
tmplConfigurable<QString> stuGlobalConfigs::TargetLanguage(
        MAKE_CONFIG_PATH("Language/Destination"),
        "Destination Language to which translating",
        "fa",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            if (ISO639isValid(_item.toVariant().toString().toLatin1().constData()))
                return true;
            _errorMessage = "Invalid ISO639 Code ("+_item.toVariant().toString() +") for Target Language";
            return false;
        }
#ifndef SMT
        ,"tl",
        "SOURCE_LANG",
        "target-lang"
#endif
);

tmplConfigurable<enuWorkingModes::Type> stuGlobalConfigs::WorkingMode(
        MAKE_CONFIG_PATH("Language/WorkingMode"),
        "WorkingModes can be (" + enuWorkingModes::options().join("|") + ")",
        enuWorkingModes::toStr(enuWorkingModes::Decode),
        ReturnTrueCrossValidator
#ifndef SMT
    ,"wm",
    "WORKING_MODE",
    "working-mode"
#endif
        );

QMap<QString, FeatureFunction::intfFeatureFunction*>       stuGlobalConfigs::ActiveFeatureFunctions;

tmplModuleConfig<Proxies::LanguageModel::intfLMSentenceScorer>         stuGlobalConfigs::LM(
        MAKE_CONFIG_PATH("Modules/LM"),
        "TODO Desc",
        "KenLMProxy");
#ifndef SMT
// There is no transliteration for anything but Statistical Machine Translation!
tmplModuleConfig<Proxies::Transliteration::intfTransliterator>         stuGlobalConfigs::Transliterator(
        MAKE_CONFIG_PATH("Modules/Transliterator"),
        "TODO Desc",
        "TargomanTransliteratorProxy");
#endif
tmplModuleConfig<RuleTable::intfRuleTable>         stuGlobalConfigs::RuleTable(
        MAKE_CONFIG_PATH("Modules/RuleTable"),
        "TODO Desc",
        "BinaryRuleTable");

QScopedPointer<Targoman::SMT::Private::Proxies::LanguageModel::intfLMSentenceScorer>
    stuGlobalConfigs::EmptyLMScorer;

QHash<QString, Common::WordIndex_t>
    stuGlobalConfigs::SourceVocab;



}
}
}

ENUM_CONFIGURABLE_IMPL(SMT::Private::enuWorkingModes)
