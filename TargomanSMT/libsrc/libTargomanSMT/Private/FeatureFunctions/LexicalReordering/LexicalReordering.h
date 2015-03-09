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
namespace SMT {
namespace Private{
namespace FeatureFunction {

TARGOMAN_DEFINE_ENHANCED_ENUM(enuLexicalReorderingFields,
                              ForwardMonotone,
                              ForwardSwap,
                              ForwardDiscontinous,
                              BackwardMonotone,
                              BackwardSwap,
                              BackwardDiscontinous
                              );

TARGOMAN_ADD_EXCEPTION_HANDLER(exLexicalReordering, exFeatureFunction);

/**
 * @brief The LexicalReordering class is a derived class from intfFeatureFunction which handles lexical reordering costs.
 */

class LexicalReordering : public intfFeatureFunction
{
public:
    ~LexicalReordering(){}
    void initialize(const QString&);

    void newSentence(const InputDecomposer::Sentence_t &inputSentence);

    Common::Cost_t scoreSearchGraphNode(
            SearchGraph::clsSearchGraphNode& _newHypothesisNode) const;
    Common::Cost_t getRestCostForPosition(const Coverage_t& _coverage, size_t _beginPos, size_t endPos) const {
        Q_UNUSED(_coverage);
        Q_UNUSED(_beginPos);
        Q_UNUSED(endPos);
        return 0;
    }
    Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                      unsigned _sourceEnd,
                                      const RuleTable::clsTargetRule& _targetRule) const;

private:
    LexicalReordering():
        intfFeatureFunction(this->moduleName(), false)
    {}

    enuLexicalReorderingFields::Type getBackwardOreientation(
            SearchGraph::clsSearchGraphNode &_newHypothesisNode) const;
    enuLexicalReorderingFields::Type getForwardOreientation(
            SearchGraph::clsSearchGraphNode &_newHypothesisNode) const;

    inline QStringList columnNames() const{
        return LexicalReordering::IsBidirectional.value() ?
                    enuLexicalReorderingFields::options() :
                    enuLexicalReorderingFields::options().mid(0,3);
    }

    void initRootNode(SearchGraph::clsSearchGraphNode &_rootNode);

private:
    static Common::Configuration::tmplConfigurable<bool>      IsBidirectional;      /**< Whether our lexical reordering is biderctional or not.*/
    static Common::Configuration::tmplConfigurable<double>    ScalingFactors[6];    /**< Scale factor of lrm costs.*/


    TARGOMAN_DEFINE_SINGLETONMODULE("FeatureFunctions/LexicalReordering", LexicalReordering);
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_LEXICALREORDERING_H
