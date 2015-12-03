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
#include "IXMLTagHandler.h"
#include "intfIXMLTagHandlerModule.hpp"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace IXMLTagHandler {

using namespace Common;
using namespace RuleTable;
using namespace Targoman::Common::Configuration;
using namespace std::placeholders;
using namespace InputDecomposer;

tmplConfigurable<QString> IXMLTagHandler::IXMLTagHandlerModules(
        MAKE_CONFIG_PATH("Handlers"),
        "Name of IXMLTagHandler Modules to be used. Comma Separated",
        "",
        [] (const intfConfigurable& _item, QString& _errorMessage) {
    QSet<QString> ModuleNames = QSet<QString>::fromList(_item.toVariant().toString().split(",", QString::SkipEmptyParts));
    for(auto Iterator = ModuleNames.begin(); Iterator != ModuleNames.end(); ++Iterator) {
        const QString& ModuleName = *Iterator;
        fpModuleInstantiator_t Instantiator = ConfigManager::instance().getInstantiator(ModuleName);
        if(Instantiator == NULL) {
            _errorMessage = "Unknown IXMLTagHandlerModule `" + ModuleName + "`";
            return false;
        }
        // Just create the object and leave it alone, this will be handled by IXMLTagHandler itself
        Instantiator();
    }
    return true;
});

tmplConfigurable<bool> IXMLTagHandler::IgnoreUserDefinedTags(
        MAKE_CONFIG_PATH("IgnoreUserDefinedTags"),
        "Whether ignore user defined tags or not",
        true);

tmplConfigurable<bool> IXMLTagHandler::KeepUnknownUserDefinedTags(
        MAKE_CONFIG_PATH("KeepUnknownUserDefinedTags"),
        "If user defined tag does not exist in source vocab, whether keep user defined tags or not",
        false);



QMap<QString, intfIXMLTagHandlerModule*>                     IXMLTagHandler::AvailableTagHandlers;

TARGOMAN_REGISTER_SINGLETON_MODULE(IXMLTagHandler);

void IXMLTagHandler::initialize()
{
    QStringList TagHandlers = IXMLTagHandler::IXMLTagHandlerModules.value().split(",", QString::SkipEmptyParts);
    foreach(const QString& TagHandlerName, TagHandlers){
        intfIXMLTagHandlerModule* pTagHandler = this->AvailableTagHandlers.value(TagHandlerName);
        if (pTagHandler == NULL)
            throw exIXMLTagHandler("Invalid TagHandler name: "+ TagHandlerName );
        this->ActiveTagHandlers.insert(TagHandlerName, pTagHandler);
    }
}

QList<WordIndex_t> IXMLTagHandler::getWordIndexOptions(const QString& _tagStr, const QString& _token, INOUT QVariantMap& _attrs) {
    QList<WordIndex_t> Result;
    if(this->ActiveTagHandlers.contains(_tagStr) != false)
    {
        Result = this->process(_tagStr,
                               _token,
                               _attrs,
                               this->ActiveTagHandlers[_tagStr]->getTargetRules(_token),
                               this->ActiveTagHandlers[_tagStr]->getDefaultTranslation(_token));
        return Result;
    }
    else{
        if(Targoman::NLPLibs::enuTextTags::options().contains(_tagStr))
            return QList<WordIndex_t>();
        else {
            if(IXMLTagHandler::IgnoreUserDefinedTags.value())
                return QList<WordIndex_t>();
            else{
                WordIndex_t UDTWordIndex = gConfigs.SourceVocab.value(_tagStr, gConfigs.EmptyLMScorer->unknownWordIndex());
                if(UDTWordIndex != gConfigs.EmptyLMScorer->unknownWordIndex())
                    return QList<WordIndex_t>() << UDTWordIndex;
                return IXMLTagHandler::KeepUnknownUserDefinedTags.value() ?
                            QList<WordIndex_t>() << gConfigs.EmptyLMScorer->unknownWordIndex() :
                            QList<WordIndex_t>();
            }
        }
    }
}

QList<Common::WordIndex_t> IXMLTagHandler::process(const QString& _tagStr,
                                                   const QString& _token,
                                                   INOUT QVariantMap& _attrs ,
                                                   const TargetRulesContainer_t& _targetRules,
                                                   const QString& _defaultTranslation)
{
    SpecialTokensRegistry::clsExpirableSpecialToken ExpirableSpecialToken =
            SpecialTokensRegistry::instance().getExpirableSpecialToken(_token);

    if (ExpirableSpecialToken.Data->NotSet){

        if (_targetRules.isEmpty())
        {
            // TODO: Add it to HandledSpecialTokens. Also it has not really been handled.
            return QList<Common::WordIndex_t>();
        }

        clsRuleNode RuleNode;
        RuleNode.detachInvalidData();
        RuleNode.targetRules().append(_targetRules);
        QList<WordIndex_t> WordIndexes;
        WordIndex_t TokenWordIndex;

        if(gConfigs.SourceVocab.contains(_tagStr)){
            if(gConfigs.SourceVocab.contains(_token)){ //source vocab has tagStr and token
                TokenWordIndex = gConfigs.SourceVocab.value(_token);
                WordIndexes.push_back(TokenWordIndex);
                WordIndexes.push_back(gConfigs.SourceVocab.value(_tagStr));
                if(_defaultTranslation.isEmpty() == false)
                    _attrs.insert(enuDefaultAttrs::toStr(enuDefaultAttrs::DefaultTranslation), _defaultTranslation);
            }else{                                    //source vocab has just tagStr
                TokenWordIndex = SpecialTokensRegistry::instance().obtainWordIndex();
                WordIndexes.push_back(TokenWordIndex);
                WordIndexes.push_back(gConfigs.SourceVocab.value(_tagStr));
                if(_defaultTranslation.isEmpty() == false)
                    _attrs.insert(enuDefaultAttrs::toStr(enuDefaultAttrs::DefaultTranslation), _defaultTranslation);
            }
        }else{
            if(gConfigs.SourceVocab.contains(_token)) {//source vocab has just token
                TokenWordIndex = gConfigs.SourceVocab.value(_token);
                WordIndexes.push_back(TokenWordIndex);
            }else{                                     //source vocab has neither tagStr nor token.
                TokenWordIndex = SpecialTokensRegistry::instance().obtainWordIndex();
                WordIndexes.push_back(TokenWordIndex);
            }
        }

        SpecialTokensRegistry::instance().insertExpirableSpecialToken(_token, SpecialTokensRegistry::clsExpirableSpecialToken(WordIndexes, _attrs));
        SpecialTokensRegistry::instance().insertRuleNode(TokenWordIndex, RuleNode);

        return WordIndexes;
    }


    //When ExpirableSpecialToken has been found
    for(QVariantMap::ConstIterator Attr = ExpirableSpecialToken.Data->Attributes.begin();
        Attr != ExpirableSpecialToken.Data->Attributes.end();
        ++Attr){
        _attrs.insert(Attr.key(), Attr.value());
    }

    return ExpirableSpecialToken.Data->WordIndexes;

}

}
}
}
}
}
