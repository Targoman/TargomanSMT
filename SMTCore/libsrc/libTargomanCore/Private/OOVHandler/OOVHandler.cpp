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
namespace Core {
namespace Private{
namespace OOV{

using namespace Common;
using namespace RuleTable;


Targoman::Common::Configuration::tmplConfigurable<QString> OOVHandler::OOVHandlerModules(
        OOVHandler::moduleName() + "/Handlers",
        "Name of OOVHandler Modules to be used. Comma Separated");

QMap<QString, intfOOVHandlerModule*>                       OOVHandler::AvailableOOVHandlers;

TARGOMAN_REGISTER_SINGLETON_MODULE(OOVHandler)

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

Common::WordIndex_t OOVHandler::getWordIndex(const QString &_token, QVariantMap &_attrs)
{
    QMutexLocker Locker(&this->Lock);
    clsExpiranbleOOVWord ExpiranbleOOVWord = this->OOVWords.value(_token);
    Locker.unlock();
    if (ExpiranbleOOVWord.NotSet){
        QList<clsTargetRule> TargetRules;
        foreach(intfOOVHandlerModule* pOOVHandler, this->ActiveOOVHandlers){
            const clsTargetRule&  OOVHandlerTargetRule = pOOVHandler->process(_token, _attrs);
            if (OOVHandlerTargetRule.isInvalid() == false){
                TargetRules.append(OOVHandlerTargetRule);
            }
        }
        if (TargetRules.isEmpty()){
            this->OOVWords.insert(_token,clsExpiranbleOOVWord(0,_attrs));
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

        this->OOVWords.insert(_token, clsExpiranbleOOVWord(WordIndex, _attrs));
        this->HandledOOVs.insert(WordIndex, RuleNode);

        return WordIndex;
    }

    //When ExpiranbleOOVWord has been found
    for(QVariantMap::ConstIterator Attr = ExpiranbleOOVWord.Attributes.begin();
        Attr != ExpiranbleOOVWord.Attributes.end();
        ++Attr){
        _attrs.insert(Attr.key(), Attr.value());
    }

    return ExpiranbleOOVWord.WordIndex;
}

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
