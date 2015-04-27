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

/**
 * @brief clsNBestFinder::nBestOptions  main interface functionality of NBest finder class that scans the translation graph and fills the NBest translation data structure
 * @return                              returns the filled structure containing the translation alternatives
 */
const clsNBestFinder::NBestOptions_t &clsNBestFinder::nBestOptions()
{
    if (this->NBestOptions.isEmpty())
        this->fillBestOptions(this->SearchGraphBuilderRef.goalNode());
    return this->NBestOptions;
}

/**
 * @brief clsNBestFinder::fillBestOptions   fills the NBest structure containing the alternate translation options
 *
 * this function gets as input the best translation hypothesis and recursively tracks the translation path through the graph and collects alternate translations for source phrases translated in the path
 * @param _node                             node representing best translation hypothesis or partial translation across the best translation path
 * @return                                  index of the output phrase in the final translation, used for recursion
 * @note                                    the returning value is not used outside the recursion
 */
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
            if (Node.targetRule().isSame(_node.targetRule()) == false)
                TargetRules.append(Node.targetRule());

        if (TargetRules.size() > clsNBestFinder::MaxOptions.value())
            break;
    }

    this->NBestOptions.insert(stuPhrasePos(_node.sourceRangeBegin(), _node.sourceRangeEnd()),
                             clsNBestFinder::stuTargetOption(
                                  stuPhrasePos(TargetIndexStart, TargetIndexStart + _node.targetRule().size()),
                                  TargetRules));

    return TargetIndexStart + _node.targetRule().size();
}

}
}
}
}
