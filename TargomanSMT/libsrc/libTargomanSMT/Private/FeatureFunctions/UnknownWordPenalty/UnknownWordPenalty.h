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

#ifndef TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_UNKNOWNUnknownWordPenalty_H
#define TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_UNKNOWNUnknownWordPenalty_H

#include "Private/FeatureFunctions/intfFeatureFunction.hpp"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

TARGOMAN_ADD_EXCEPTION_HANDLER(exUnknownWordPenalty, exFeatureFunction);


class UnknownWordPenalty : public intfFeatureFunction
{
public:
    ~UnknownWordPenalty(){}

    void initialize(const QString &){}

    Common::Cost_t scoreSearchGraphNode(SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode) const;

    Common::Cost_t getRestCostForPosition(const Coverage_t& _coverage, size_t _beginPos, size_t endPos) const {
        Q_UNUSED(_coverage)
        Q_UNUSED(_beginPos)
        Q_UNUSED(endPos)
        return 0;
    }

    inline Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                             unsigned _sourceEnd,
                                             const RuleTable::clsTargetRule& _targetRule) const {
        Q_UNUSED(_sourceStart)
        Q_UNUSED(_sourceEnd)
        return this->getUnknownWordPenaltyCost(_targetRule);
    }

    inline QStringList columnNames() const{return QStringList();}

    void initRootNode(SearchGraphBuilder::clsSearchGraphNode &_rootNode);

private:
    UnknownWordPenalty():
        intfFeatureFunction(this->moduleName(), false)
    {}

    TARGOMAN_DEFINE_SINGLETONMODULE("FeatureFunctions/UnknownWordPenalty", UnknownWordPenalty);

public:
    inline Common::Cost_t getUnknownWordPenaltyCost(const RuleTable::clsTargetRule& _targetRule) const {
        Common::Cost_t Cost = 0;
        if(_targetRule.isUnknownWord())
            Cost = 100;
        return Cost * UnknownWordPenalty::ScalingFactor.value();
    }

private:
    static Common::Configuration::tmplConfigurable<double> ScalingFactor;

};


}
}
}
}


#endif // TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_UNKNOWNUnknownWordPenalty_H
