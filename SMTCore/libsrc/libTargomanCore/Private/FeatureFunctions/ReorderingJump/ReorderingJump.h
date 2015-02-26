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

#ifndef TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_REORDERINGJUMP_H
#define TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_REORDERINGJUMP_H

#include "Private/FeatureFunctions/intfFeatureFunction.hpp"

namespace Targoman {
namespace Core {
namespace Private {
namespace FeatureFunction{

TARGOMAN_ADD_EXCEPTION_HANDLER(exReorderingJump, exFeatureFunction);


class ReorderingJump : public intfFeatureFunction
{
public:
    ~ReorderingJump(){}

    void initialize(const QString &){}

    Common::Cost_t scoreSearchGraphNode(SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode) const;

    inline Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                             unsigned _sourceEnd,
                                             const RuleTable::clsTargetRule& _targetRule) const {
        Q_UNUSED(_sourceStart)
        Q_UNUSED(_sourceEnd)
        Q_UNUSED(_targetRule)
        return 0;
    }

    inline QStringList columnNames() const{return QStringList();}

private:
    ReorderingJump();
    TARGOMAN_DEFINE_SINGLETONMODULE("FeatureFunctions/ReorderingJump", ReorderingJump)

private:
    static Common::Configuration::tmplConfigurable<double> ScalingFactor;
    static Common::Configuration::tmplConfigurable<quint8> MaximumJumpWidth;

};


}
}
}
}


#endif // TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_REORDERINGJUMP_H
