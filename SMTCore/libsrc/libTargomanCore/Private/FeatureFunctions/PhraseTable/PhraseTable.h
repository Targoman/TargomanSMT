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

    inline Common::Cost_t scoreSearchGraphNode(SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode) const{
        Q_UNUSED(_newHypothesisNode)
        //@TODO NASTY!!!!
        //@note This will return zero to distinguish between phrase table and other feature funtions.
        return 0;
    }

    inline Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                      unsigned _sourceEnd,
                                      const RuleTable::clsTargetRule& _targetRule) const{
        return this->getTargetRuleCost(_sourceStart, _sourceEnd, _targetRule);
    }

    Common::Cost_t getTargetRuleCost(unsigned _sourceStart,
                                     unsigned _sourceEnd,
                                     const RuleTable::clsTargetRule &_targetRule) const;

    inline QStringList columnNames() const{return PhraseTable::ColumnNames;}
    static inline void setColumnNames(const QStringList _columnNames){PhraseTable::ColumnNames = _columnNames;}

private:
    PhraseTable();
    Q_DISABLE_COPY(PhraseTable)
    TARGOMAN_DEFINE_SINGLETONMODULE("PhraseTable", PhraseTable)

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
