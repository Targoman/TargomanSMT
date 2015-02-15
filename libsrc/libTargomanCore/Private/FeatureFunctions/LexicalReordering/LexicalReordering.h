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

#ifndef TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_LEXICALREORDERING_H
#define TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_LEXICALREORDERING_H

#include "Private/FeatureFunctions/intfFeatureFunction.hpp"

namespace Targoman{
namespace Core {
namespace Private{
namespace FeatureFunction {

TARGOMAN_DEFINE_ENHANCED_ENUM(enuLexicalReorderingFields,
                              LeftMonotone,
                              LeftSwap,
                              LeftDiscontinous,
                              RightMonotone,
                              RightSwap,
                              RightDiscontinous
                              )

TARGOMAN_ADD_EXCEPTION_HANDLER(exLexicalReordering, exFeatureFunction);

class LexicalReordering : public intfFeatureFunction
{
public:
    void initialize();
    void newSentence(const InputDecomposer::Sentence_t &inputSentence);
    virtual Common::Cost_t scoreSearchGraphNode(SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode) const;

    virtual Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                              unsigned _sourceEnd,
                                              const RuleTable::clsTargetRule& _targetRule);

private:
    LexicalReordering();
    Q_DISABLE_COPY(LexicalReordering)

    enuLexicalReorderingFields::Type getRightOreientation(SearchGraphBuilder::clsSearchGraphNode &_newHypothesisNode) const;
    enuLexicalReorderingFields::Type getLeftOreientation(SearchGraphBuilder::clsSearchGraphNode &_newHypothesisNode) const;

    inline QStringList columnNames() const{return enuLexicalReorderingFields::options();}

private:
    static Common::Configuration::tmplConfigurable<bool>      IsBidirectional;
    static Common::Configuration::tmplConfigurable<double>    ScalingFactors[6];


    TARGOMAN_DEFINE_SINGLETONMODULE("LRM", LexicalReordering)
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_LEXICALREORDERING_H
