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

#ifndef TARGOMAN_CORE_PRIVATE_NBESTFINDER_CLSNBESTFINDER_H
#define TARGOMAN_CORE_PRIVATE_NBESTFINDER_CLSNBESTFINDER_H

#include "libTargomanCommon/CmdIO.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Types.h"
#include "Private/SearchGraph/clsSearchGraphBuilder.h"

namespace Targoman{
namespace SMT {
namespace Private{

/**
 *  @brief NBest finder module
 */
namespace NBestFinder {

class clsNBestFinder
{
public:
    struct stuTargetOption{
        stuPhrasePos Pos;
        QList<RuleTable::clsTargetRule> TargetRules;


        stuTargetOption(const stuPhrasePos& _pos,
                        const QList<RuleTable::clsTargetRule>& _target){
            this->Pos = _pos;
            this->TargetRules = _target;
        }
    };

    typedef QMap<stuPhrasePos, stuTargetOption> NBestOptions_t;

public:
    clsNBestFinder(const SearchGraph::clsSearchGraphBuilder& _searchGraphBuilder) :
        SearchGraphBuilderRef(_searchGraphBuilder)
    {}
    const NBestOptions_t& nBestOptions();

    const SearchGraph::clsSearchGraphNode& goalNode(){
        return  this->SearchGraphBuilderRef.goalNode();
    }


private:
    size_t fillBestOptions(const SearchGraph::clsSearchGraphNode &_node);

private:
    NBestOptions_t                                        NBestOptions;
    const SearchGraph::clsSearchGraphBuilder&     SearchGraphBuilderRef;

private:
    static Common::Configuration::tmplConfigurable<quint8> MaxOptions;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_NBESTFINDER_CLSNBESTFINDER_H
