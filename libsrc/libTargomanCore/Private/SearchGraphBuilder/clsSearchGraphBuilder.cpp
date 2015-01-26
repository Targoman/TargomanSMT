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
 */

#include "clsSearchGraphBuilder.h"
#include "../GlobalConfigs.h"
#include "Private/LanguageModel/intfLMSentenceScorer.hpp"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

clsSearchGraphBuilder::clsSearchGraphBuilder()
{
}

void clsSearchGraphBuilder::init()
{
}

void clsSearchGraphBuilder::matchPhrase(const QList<InputDecomposer::clsToken *> &_input)
{
    //QScopedPointer<LanguageModel::intfLMSentenceScorer> LMScorer(gConfigs.newLMScorerInstance());

    //check Qlist vs Vector
    //resize PhraseMatchTable to rest of sentence
    for (unsigned FirstPosition = 0; FirstPosition < _input.size(); ++FirstPosition) {
        this->PhraseMatchTable.append(QVector<PhraseTable::clsRuleNode*>(_input.size() - FirstPosition,NULL));
        //get rootnode -> prevNode
        for (unsigned LastPosition = FirstPosition; LastPosition < _input.size(); ++LastPosition){

            //lookup prevNode + LastPos in phrase table -> prev node
            // if (prevNode == Null) break;
            //this->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition] = PrevNode

        }
    }
}

void clsSearchGraphBuilder::parseSentence()
{

}

}
}
}
}
