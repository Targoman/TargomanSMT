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
#include "Private/SearchGraphBuilder/clsSearchGraph.h"


namespace Targoman{
namespace SMT {
namespace Private{

/**
 *  @brief NBest finder module
 */
namespace NBestFinder {

class NBestSuggestions
{
public:
    struct stuTargetOption{
        stuPhrasePos Pos;
        QList<RuleTable::clsTargetRule> TargetRules;


        stuTargetOption(const stuPhrasePos& _pos = stuPhrasePos(),
                        const QList<RuleTable::clsTargetRule>& _target =
                              QList<RuleTable::clsTargetRule>()){
            this->Pos = _pos;
            this->TargetRules = _target;
        }
    };

    typedef QMap<stuPhrasePos, stuTargetOption> Container_t;

public:
    static Container_t retrieve(const SearchGraphBuilder::clsSearchGraph& _searchGraph);

private:
    static size_t fillBestOptions(NBestSuggestions::Container_t& _storage, const SearchGraphBuilder::clsSearchGraph &_searchGraph,
                                  const SearchGraphBuilder::clsSearchGraphNode& _currNode);

private:
    static Common::Configuration::tmplConfigurable<quint8> MaxSuggestions;
    friend class UnitTestNameSpace::clsUnitTest;

};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_NBESTFINDER_CLSNBESTFINDER_H
