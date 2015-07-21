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
#include "Private/Proxies/intfLMSentenceScorer.hpp"
#include "ISO639.h"

namespace Targoman {
namespace SMT {
/**
 * @brief Private section of TargomanSMT library which obfuscates all internal namespaces and classes
 */
namespace Private{

using namespace Common;
using namespace Common::Configuration;

QString ActorUUID;

stuGlobalConfigs gConfigs;

tmplConfigurable<QString> stuGlobalConfigs::Separator(
        "/Common/Separator",
        "Separator used on multi field configurations",
        ";:;");
tmplConfigurable<QString> stuGlobalConfigs::SourceLanguage(
        "/Common/Language/Source",
        "Source language from which translating",
        "en",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            if (ISO639isValid(_item.toVariant().toString().toLatin1().constData()))
                return true;
            _errorMessage = "Invalid ISO639 Code ("+_item.toVariant().toString() +") for Source Language";
            return false;
        },
        "sl",
        "SOURCE_LANG",
        "source-lang");
tmplConfigurable<QString> stuGlobalConfigs::TargetLanguage(
        "/Common/Language/Destination",
        "Destination Language to which translating",
        "fa",
        [] (const intfConfigurable& _item, QString& _errorMessage){
            if (ISO639isValid(_item.toVariant().toString().toLatin1().constData()))
                return true;
            _errorMessage = "Invalid ISO639 Code ("+_item.toVariant().toString() +") for Target Language";
            return false;
        },
        "tl",
        "SOURCE_LANG",
        "target-lang");

tmplConfigurable<enuWorkingModes::Type> stuGlobalConfigs::WorkingMode(
        "/Common/WorkingMode",
        "WorkingModes can be (" + enuWorkingModes::options().join("|") + ")",
        enuWorkingModes::toStr(enuWorkingModes::Decode),
        ReturnTrueCrossValidator,
        "wm",
        "WORKING_MODE",
        "working-mode"
        );

QMap<QString, FeatureFunction::intfFeatureFunction*>       stuGlobalConfigs::ActiveFeatureFunctions;

clsModuleConfig         stuGlobalConfigs::LM("/Modules/LM",
                                             "TODO Desc",
                                             "KenLMProxy");
clsModuleConfig         stuGlobalConfigs::RuleTable("/Modules/RuleTable",
                                                    "TODO Desc",
                                                    "BinaryRuleTable");
QScopedPointer<Targoman::SMT::Private::Proxies::intfLMSentenceScorer>  stuGlobalConfigs::EmptyLMScorer;
QHash<QString, Common::WordIndex_t>                            stuGlobalConfigs::SourceVocab;



}
}
}

ENUM_CONFIGURABLE_IMPL(SMT::Private::enuWorkingModes)
