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
#include "Private/GlobalConfigs.h"

namespace Targoman{
namespace SMT {
namespace Private{
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
    this->WordIndexOffset = gConfigs.SourceVocab.size() + 1;
}

/**
 * @brief OOVHandler::getWordIndex  This function stores types of used special OOV Handler in _attrs and returns a word index for it.
 * @param[in] _token                input OOV word string.
 * @param[in,out] _attrs            Types of special OOV handlers will be set in this argument.
 * @return                          Word index of input OOV word.
 */

Common::WordIndex_t OOVHandler::getWordIndex(const QString &_token, QVariantMap &_attrs)
{
    QMutexLocker Locker(&this->Lock);
    clsExpirableOOVWord ExpirableOOVWord = this->OOVWords.value(_token);
    Locker.unlock();
    if (ExpirableOOVWord.NotSet){
        TargetRulesContainer_t TargetRules;
        foreach(intfOOVHandlerModule* pOOVHandler, this->ActiveOOVHandlers){
            const clsTargetRule&  OOVHandlerTargetRule = pOOVHandler->process(_token, _attrs);
            if (OOVHandlerTargetRule.isInvalid() == false){
                TargetRules.append(OOVHandlerTargetRule);
            }
        }
        if (TargetRules.isEmpty()){
            this->OOVWords.insert(_token,clsExpirableOOVWord(0,_attrs));
            return 0; // There are no new handlers so keep it as unknown and cache result
        }

        clsRuleNode RuleNode;
        RuleNode.detachInvalidData();
        RuleNode.targetRules().append(TargetRules);

        Locker.relock();
        WordIndex_t WordIndex;
        if (this->AvailableOOVHandlers.size())
            WordIndex = this->AvailableWordIndexes.takeFirst();
        else
            WordIndex = this->WordIndexOffset + this->OOVWords.keys().size();

        this->OOVWords.insert(_token, clsExpirableOOVWord(WordIndex, _attrs));
        this->HandledOOVs.insert(WordIndex, RuleNode);

        return WordIndex;
    }

    //When ExpiranbleOOVWord has been found
    for(QVariantMap::ConstIterator Attr = ExpirableOOVWord.Attributes.begin();
        Attr != ExpirableOOVWord.Attributes.end();
        ++Attr){
        _attrs.insert(Attr.key(), Attr.value());
    }

    return ExpirableOOVWord.WordIndex;
}
/**
 * @brief OOVHandler::removeWordIndex When an OOV exipres this function  will be called to removes that word index from #HandledOOVs and add that word index to #AvailableWordIndexes.
 * @param _wordIndex input word index.
 */
void OOVHandler::removeWordIndex(WordIndex_t _wordIndex)
{
    QMutexLocker Locker(&this->Lock);
    this->HandledOOVs.remove(_wordIndex);
    this->AvailableWordIndexes.append(_wordIndex);
}


}
}
}
}
