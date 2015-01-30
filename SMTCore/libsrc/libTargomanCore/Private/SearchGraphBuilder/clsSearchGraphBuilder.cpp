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

#include "clsSearchGraphBuilder.h"
#include "../GlobalConfigs.h"
#include "Private/LanguageModel/intfLMSentenceScorer.hpp"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

using namespace PhraseTable;
using namespace LanguageModel;
using namespace InputDecomposer;

class clsInvalidPhraseTable : public intfPhraseTable{
public:
    clsInvalidPhraseTable():
        intfPhraseTable("INVALID_PHRASETABLE"){}
    void init(){throw Common::exTargomanNotImplemented("clsInvalidPhraseTable::init");}
    void newSentence(){throw Common::exTargomanNotImplemented("clsInvalidPhraseTable::newSentence");}
    bool isBinary(){throw Common::exTargomanNotImplemented("clsInvalidPhraseTable::isBinary"); return false;}
}InvalidPhraseTable;


intfPhraseTable& clsSearchGraphBuilder::PhraseTable = InvalidPhraseTable;

clsSearchGraphBuilder::clsSearchGraphBuilder()
{
}

void clsSearchGraphBuilder::init()
{
    PhraseTable = *gConfigs.PhraseTable.getInstance<intfPhraseTable>();
}

void clsSearchGraphBuilder::matchPhrase(const QList<clsToken *> &_input)
{
    QScopedPointer<intfLMSentenceScorer> LMScorer(
                gConfigs.LM.getInstance<intfLMSentenceScorer>());

    for (size_t FirstPosition = 0; FirstPosition < _input.size(); ++FirstPosition) {
        this->PhraseMatchTable.append(QVector<const clsRuleNode*>(_input.size() - FirstPosition,NULL));
        const RulesPrefixTree_t::Node* PrevNode = this->PhraseTable.getPrefixTree().rootNode();
        //Max PhraseTable order will be implicitly checked by follow
        for (size_t LastPosition = FirstPosition; LastPosition < _input.size() ; ++LastPosition){
            PrevNode = PrevNode->follow(_input.at(LastPosition)->wordIndex());
            if (PrevNode == NULL)
                break; // appending next word breaks phrase lookup

            //ERROR !!!!!!!!!!!!! referencecounter will be buggy
            this->PhraseMatchTable[FirstPosition][LastPosition - FirstPosition] = &PrevNode->getData();
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
