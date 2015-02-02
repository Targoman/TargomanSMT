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

#include "clsLexicalHypothesis.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

using namespace Common;
const Cost_t PBT_LEXICAL_HYPOTHESIS_CONTAINER_EMPTY_BEST = 1e10;

clsLexicalHypothesis::clsLexicalHypothesis()
{

}

bool clsLexicalHypothesis::mustBePruned(Cost_t _totalCost)
{
    // TODO: compare with or without rest costs? check wheter restcost is stored in node

    return !( this->Nodes.isEmpty() ||
               (this->LexicalMaxHistogramSize.value() == 0 ||
               this->Nodes.size() < this->LexicalMaxHistogramSize.value() ||
               _totalCost < this->Nodes.last().getTotalCost()));

}

Cost_t clsLexicalHypothesis::getBestCost() const
{
    if (this->Nodes.isEmpty())
        return PBT_LEXICAL_HYPOTHESIS_CONTAINER_EMPTY_BEST;

    return this->bestNode().getCost();
}

void clsLexicalHypothesis::insertHypothesis(clsSearchGraphNode *_node)
{
    //TODO  bool LexicalHypothesisContainer::insertHypothesis(SourceCardinalitySynchronousSearchNode *node)
}

}
}
}
}
