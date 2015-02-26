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
#include "libTargomanCommon/Configuration/clsFileBasedConfig.hpp"

namespace Targoman {
namespace Core {
namespace Private {
namespace FeatureFunction{

TARGOMAN_ADD_EXCEPTION_HANDLER(exPhraseTable, exFeatureFunction);


class PhraseTable : public intfFeatureFunction
{
public:
    ~PhraseTable(){}

    void initialize(const QString &_configFile);

    Common::Cost_t scoreSearchGraphNode(
            SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode) const;
    inline Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                             unsigned _sourceEnd,
                                             const RuleTable::clsTargetRule& _targetRule) const{
        Q_UNUSED(_sourceStart)
        Q_UNUSED(_sourceEnd)
        return this->getPhraseCost(_targetRule);
    }
    Common::Cost_t getRestCostForPosition(const Coverage_t& _coverage, size_t _beginPos, size_t endPos) const {
        Q_UNUSED(_coverage);
        Q_UNUSED(_beginPos);
        Q_UNUSED(endPos);
        return 0;
    }

    inline QStringList columnNames() const{return PhraseTable::ColumnNames;}
    static inline void setColumnNames(const QStringList _columnNames){
        PhraseTable::ColumnNames = _columnNames;}

    Common::Cost_t getPhraseCost(const RuleTable::clsTargetRule& _targetRule) const;

private:
    PhraseTable():
        intfFeatureFunction(this->moduleName(), false)
    {}
    TARGOMAN_DEFINE_SINGLETONMODULE("FeatureFunctions/PhraseTable", PhraseTable)


    QList<double> ScalingFactors;
    static QStringList   ColumnNames;

private:
    static Targoman::Common::Configuration::clsFileBasedConfig ScalingFactorsConfigSection;
};


}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_PHRASETABLE_H
