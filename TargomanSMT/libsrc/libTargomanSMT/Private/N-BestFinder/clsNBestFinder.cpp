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

#include "clsNBestFinder.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace NBestFinder {

using namespace Common;
using namespace Common::Configuration;
using namespace SearchGraph;
using namespace RuleTable;

tmplConfigurable<quint8> clsNBestFinder::MaxOptions(
        "NBestFinder/MaxOptions",
        "Maximum number of options to output",
        5
        );

const clsNBestFinder::NBestOptions_t &clsNBestFinder::nBestOptions()
{
    if (this->NBestOptions.isEmpty())
        this->fillBestOptions(this->SearchGraphBuilderRef.goalNode());
    return this->NBestOptions;
}

size_t clsNBestFinder::fillBestOptions(const SearchGraph::clsSearchGraphNode &_node)
{
    if(_node.isInvalid())
        return 0;

    size_t TargetIndexStart = fillBestOptions(_node.prevNode());

    TargetRulesContainer_t TargetRules;
    TargetRules.append(_node.targetRule());

    foreach (const clsSearchGraphNode& Node, this->SearchGraphBuilderRef.getSameCoverageNodes(_node.coverage())) {
        if (Node.sourceRangeBegin() == _node.sourceRangeBegin() &&
            Node.sourceRangeEnd() == _node.sourceRangeEnd())
            //_node.targetRule has been appended before so take care to not append it twice
            if (Node.targetRule().isSame(_node.targetRule()))
                TargetRules.append(Node.targetRule());

        if (TargetRules.size() > clsNBestFinder::MaxOptions.value())
            break;
    }

    this->NBestOptions.insert(stuPhrasePos(_node.sourceRangeBegin(), _node.sourceRangeEnd()),
                             clsNBestFinder::stuTargetOption(
                                  stuPhrasePos(TargetIndexStart, TargetIndexStart + _node.targetRule().size() - 1),
                                  TargetRules));

    return TargetIndexStart + _node.targetRule().size();
}

}
}
}
}
