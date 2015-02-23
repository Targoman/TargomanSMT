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
#include "Private/GlobalConfigs.h"
#include <QMutexLocker>

namespace Targoman{
namespace Core {
namespace Private{

using namespace Common;
using namespace Targoman::Core::Private::RuleTable;

OOVHandler* OOVHandler::Instance = NULL;


Common::WordIndex_t OOVHandler::getWordIndex(const QString &_word)
{
    QMutexLocker Lock(&this->Lock);

    WordIndex_t WordIndex = this->OOVWords.(_word,0);
    if (WordIndex == 0){
        if (this->AvailableWordIndexes.size()){
            WordIndex = this->AvailableWordIndexes.takeFirst();
        }else{
            WordIndex = this->HandledOOVs.size() + this->WordIndexOffset;
        }
        this->OOVWords.insert(_word, WordIndex);

        clsTargetRule
        clsRuleNode RuleNode;
        RuleNode.detachInvalidData();
        RuleNode.targetRules().append(TargetRule);

        this->HandledOOVs.insert(WordIndex,
                                 stuOOVItem(
                                     _word,
                                     cls
                                     ));
    }
    return WordIndex;
}

void OOVHandler::init()
{
    this->WordIndexOffset = gConfigs.SourceVocab.size();
}

OOVHandler::OOVHandler()
{

}

}
}
}
