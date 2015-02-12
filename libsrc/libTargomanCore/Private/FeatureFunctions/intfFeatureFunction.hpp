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

#ifndef TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_INTFFEATUREFUNCTION_HPP
#define TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_INTFFEATUREFUNCTION_HPP

#include "Private/InputDecomposer/clsInput.h"
#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"
#include "libTargomanCore/clsTranslator.h"
#include "Private/SearchGraphBuilder/clsSearchGraphNode.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace FeatureFunction {

TARGOMAN_ADD_EXCEPTION_HANDLER(exFeatureFunction, exTargomanCore);

class intfFeatureFunction : public Common::Configuration::intfModule
{
public:
    intfFeatureFunction(const QString& _name) :
        Common::Configuration::intfModule(_name)
    {
        gConfigs.ActiveFeatureFunctions.insert(_name, this);
        this->PrecomputedIndex = RuleTable::clsTargetRule::allocatePrecomputedValue();
    }

    virtual bool nodesHaveSameState(const SearchGraphBuilder::clsSearchGraphNode& _first,
                                    const SearchGraphBuilder::clsSearchGraphNode& _second) const {
        Q_UNUSED(_first)
        Q_UNUSED(_second)
        return true;
    }

    virtual void initialize()  = 0;
    virtual void newSentence(const InputDecomposer::Sentence_t &inputSentence) {}

    /*
     * This can be called to score the new hypothesis and initialize its state correctly.
     * The first secondary model will encounter an uninitialized hypothesis state, thus don't forget to call
     * newHypothesisNode.getHypothesisState()->initializeSecondaryModelStatesIfNecessary();
     */
    virtual Common::Cost_t scoreSearchGraphNode(SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode) const =0 ;

    //This is to add an approximate cost to the future cost heuristic in SCSS
    virtual Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                              unsigned _sourceEnd,
                                              const RuleTable::clsTargetRule& _targetPhrase) const{
        return 0;
    }

    virtual Common::Cost_t getTargetRuleCost(unsigned _sourceStart,
                                       unsigned _sourceEnd,
                                       const RuleTable::clsTargetRule& _targetPhrase) const {
        throw Common::exTargomanNotImplemented("getTargetRuleCost must be implemented in subclasses");
    }

protected:
    static QString baseScalingFactorsConfigPath(){ return "/ScalingFactors"; }
    QVector<size_t>         FieldIndexes;
    size_t                  PrecomputedIndex;
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_INTFFEATUREFUNCTION_HPP
