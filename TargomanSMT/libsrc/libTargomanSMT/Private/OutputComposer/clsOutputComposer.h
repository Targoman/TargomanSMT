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

#ifndef TARGOMAN_CORE_PRIVATE_OUTPUTCOMPOSER_CLSOUTPUTCOMPOSER_H
#define TARGOMAN_CORE_PRIVATE_OUTPUTCOMPOSER_CLSOUTPUTCOMPOSER_H

#include "Private/SearchGraphBuilder/clsSearchGraph.h"
#include "libTargomanSMT/Types.h"

namespace Targoman{
namespace SMT {
namespace Private{

/**
 *  @brief Output composition module
 */
namespace OutputComposer {

class clsOutputComposer
{
public:
    clsOutputComposer(InputDecomposer::clsInput& _inputDecomposer,
                      SearchGraphBuilder::clsSearchGraph& _searchGraph):
        InputDecomposerRef(_inputDecomposer),
        SearchGraphRef(_searchGraph)
    {}

public:
    stuTranslationOutput translationOutput();
    QString translationString();

private:
    QString getTargetString(const RuleTable::clsTargetRule& _target, const stuPos& _sourcePhrasePos);
    QString nodeTranslation(const SearchGraphBuilder::clsSearchGraphNode& _node);

private:
    InputDecomposer::clsInput&            InputDecomposerRef;
    SearchGraphBuilder::clsSearchGraph&   SearchGraphRef;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_OUTPUTCOMPOSER_CLSOUTPUTCOMPOSER_H
