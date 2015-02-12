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

#ifndef TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_PHRASETABLE_H
#define TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_PHRASETABLE_H


#include "Private/FeatureFunctions/intfFeatureFunction.hpp"
#include "libTargomanCommon/tmplExpirableCache.hpp"

namespace Targoman {
namespace Core {
namespace Private {
namespace FeatureFunction{

TARGOMAN_DEFINE_ENHANCED_ENUM(enuPhraseTableFields,
                              s2t,
                              t2s,
                              ibm1s2t,
                              ibm1t2s,
                              phrasePenalty,
                              wordPenalty,
                              s2tRatio,
                              t2sRatio,
                              cnt1,
                              cnt2,
                              cnt3
                              )

TARGOMAN_ADD_EXCEPTION_HANDLER(exPhraseTable, exFeatureFunction);

class PhraseTable : public intfFeatureFunction
{
    void initialize();

    inline Common::Cost_t scoreSearchGraphNode(SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode) const{
        Q_UNUSED(_newHypothesisNode)
        return 0; //This will return zero to distinguish betwwen phrase table ans other feature funtions.
        //TODO NASTY!!!!
        //this->getTargetRuleCost(0,0,_newHypothesisNode.targetRule());
    }

    Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                      unsigned _sourceEnd,
                                      const RuleTable::clsTargetRule& _targetRule) const;
    Common::Cost_t getTargetRuleCost(unsigned _sourceStart,
                                     unsigned _sourceEnd,
                                     const RuleTable::clsTargetRule &_targetRule) const;
private:
    PhraseTable();
    Q_DISABLE_COPY(PhraseTable)
    TARGOMAN_DEFINE_SINGLETONMODULE("PhraseTable", PhraseTable)

private:
    static Common::Configuration::tmplConfigurable<double>    ScalingFactors[11];
};


}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_PHRASETABLE_H
