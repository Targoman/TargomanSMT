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

#include "NBestSuggestions.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace NBestFinder {

using namespace Common;
using namespace Common::Configuration;
using namespace SearchGraphBuilder;
using namespace RuleTable;

tmplConfigurable<quint8> NBestSuggestions::MaxSuggestions(
        "NBestFinder/MaxSuggestions",
        "Maximum number of suggestions to output",
        5
        );

/**
 * @brief clsNBestFinder::retrieve  main interface functionality of NBest finder class that scans the translation graph and fills the NBest translation data structure
 * @return                          returns the filled structure containing the translation alternatives
 */
NBestSuggestions::Container_t NBestSuggestions::retrieve(const clsSearchGraph &_searchGraph)
{
    NBestSuggestions::Container_t NBestSuggestions;
    NBestSuggestions::fillBestOptions(NBestSuggestions,
                                      _searchGraph,
                                      _searchGraph.goalNode());
    return NBestSuggestions;
}

/**
 * @brief clsNBestFinder::fillBestOptions   fills the NBest structure containing the alternate translation options
 *
 * this function gets as input the best translation hypothesis and recursively tracks the translation path through the graph and collects alternate translations for source phrases translated in the path
 * @param _node                             node representing best translation hypothesis or partial translation across the best translation path
 * @return                                  index of the output phrase in the final translation, used for recursion
 * @note                                    the returning value is not used outside the recursion
 */
size_t NBestSuggestions::fillBestOptions(NBestSuggestions::Container_t& _storage,
                                         const SearchGraphBuilder::clsSearchGraph& _searchGraph,
                                         const SearchGraphBuilder::clsSearchGraphNode &_currNode)
{
    if(_currNode.isInvalid())
        return 0;

    size_t TargetIndexStart = NBestSuggestions::fillBestOptions(_storage,
                                                                _searchGraph,
                                                                _currNode.prevNode());

    TargetRulesContainer_t TargetRules;
    TargetRules.append(_currNode.targetRule());

    foreach (const clsSearchGraphNode& Node,
             _searchGraph.getSameCoverageNodes(_currNode.coverage())) {
        if (Node.sourceRangeBegin() == _currNode.sourceRangeBegin() &&
            Node.sourceRangeEnd() == _currNode.sourceRangeEnd())
            //_node.targetRule has been appended before so take care to not append it twice
            if (Node.targetRule().isSame(_currNode.targetRule()) == false)
                TargetRules.append(Node.targetRule());

        if (TargetRules.size() >= NBestSuggestions::MaxSuggestions.value())
            break;
    }

    _storage.insert(stuPos(_currNode.sourceRangeBegin(), _currNode.sourceRangeEnd()),
                    NBestSuggestions::stuTargetOption(
                        stuPos(TargetIndexStart, TargetIndexStart + _currNode.targetRule().size()),
                        TargetRules));

    return TargetIndexStart + _currNode.targetRule().size();
}

}
}
}
}
