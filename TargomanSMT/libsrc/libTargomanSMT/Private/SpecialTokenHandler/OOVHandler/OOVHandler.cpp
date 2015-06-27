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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#include "OOVHandler.h"
#include "intfOOVHandlerModule.hpp"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace OOV{

using namespace Common;
using namespace RuleTable;
using namespace Targoman::Common::Configuration;

tmplConfigurable<QString> OOVHandler::OOVHandlerModules(
        OOVHandler::moduleName() + "/Handlers",
        "Name of OOVHandler Modules to be used. Comma Separated",
        "",
        [] (const intfConfigurable& _item, QString& _errorMessage) {
    QSet<QString> ModuleNames = QSet<QString>::fromList(_item.toVariant().toString().split(",", QString::SkipEmptyParts));
    for(auto Iterator = ModuleNames.begin(); Iterator != ModuleNames.end(); ++Iterator) {
        const QString& ModuleName = *Iterator;
        fpModuleInstantiator_t Instantiator = ConfigManager::instance().getInstantiator(ModuleName);
        if(Instantiator == NULL) {
            _errorMessage = "Unknown OOVHandlerModule `" + ModuleName + "`";
            return false;
        }
        // Just create the object and leave it alone, this will be handled by OOVHandler itself
        Instantiator();
    }
    return true;
});

QMap<QString, intfOOVHandlerModule*>                       OOVHandler::AvailableOOVHandlers;

TARGOMAN_REGISTER_SINGLETON_MODULE(OOVHandler);

/**
 * @brief OOVHandler::initialize Sets #ActivesOOVHandlers from #OOVHandlerModules and checks whethere they are available handlers or not.
 * @exception throws exception if name of special OOV handler is not in #AvailableOOVHandlers.
 */

void OOVHandler::initialize()
{
    QStringList OOVHandlers = OOVHandler::OOVHandlerModules.value().split(",", QString::SkipEmptyParts);
    foreach(const QString& OOVHandlerName, OOVHandlers){
        intfOOVHandlerModule* pOOVHandler = this->AvailableOOVHandlers.value(OOVHandlerName);
        if (pOOVHandler == NULL)
            throw exOOVHandler("Invalid OOVHandler name: "+ OOVHandlerName );
        this->ActiveOOVHandlers.append(pOOVHandler);
    }

}

/**
 * @brief OOVHandler::getWordIndex  This function stores types of used special OOV Handler in _attrs and returns a word index for it.
 * @param[in] _token                input OOV word string.
 * @param[in,out] _attrs            Types of special OOV handlers will be set in this argument.
 * @return                          Word index of input OOV word.
 */

QList<Common::WordIndex_t> OOVHandler::getWordIndexOptions(const QString &_token, QVariantMap &_attrs)
{
    SpecialTokensRegistry::clsExpirableSpecialToken ExpirableSpecialToken =
            SpecialTokensRegistry::instance().getExpirableSpecialToken(_token);

    if (ExpirableSpecialToken.Data->NotSet){
        TargetRulesContainer_t TargetRules;
        foreach(intfOOVHandlerModule* pOOVHandler, this->ActiveOOVHandlers){
            const clsTargetRule&  OOVHandlerTargetRule = pOOVHandler->process(_token, _attrs);
            if (OOVHandlerTargetRule.isInvalid() == false){
                TargetRules.append(OOVHandlerTargetRule);
            }
        }
        if (TargetRules.isEmpty()){
            SpecialTokensRegistry::instance().insertExpirableSpecialToken(_token, SpecialTokensRegistry::clsExpirableSpecialToken(Constants::SrcVocabUnkWordIndex, _attrs));
            return (QList<Common::WordIndex_t>() << Constants::SrcVocabUnkWordIndex); // There are no new handlers so keep it as unknown and cache result
        }

        clsRuleNode RuleNode;
        RuleNode.detachInvalidData();
        RuleNode.targetRules().append(TargetRules);

        WordIndex_t WordIndex = SpecialTokensRegistry::instance().obtainWordIndex();
        SpecialTokensRegistry::instance().insertExpirableSpecialToken(_token, SpecialTokensRegistry::clsExpirableSpecialToken(WordIndex, _attrs));
        SpecialTokensRegistry::instance().insertRuleNode(WordIndex, RuleNode);

        return (QList<Common::WordIndex_t>() << WordIndex);
    }

    //When ExpirableOOVWord has been found
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
