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
/**
 * @brief The intfFeatureFunction class is an interface class that every feature function like phrase table or lexical reordering table is derivated from that.
 */
class intfFeatureFunction : public Common::Configuration::intfModule
{
public:
    intfFeatureFunction(const QString& _moduleName) :
        intfModule(_moduleName)
    {
        gConfigs.ActiveFeatureFunctions.insert(_moduleName, this);
        this->CanComputePositionSpecificRestCost = false;
        this->PrecomputedIndex = RuleTable::clsTargetRule::allocatePrecomputedValue();
        this->DataIndex =  SearchGraphBuilder::clsSearchGraphNode::allocateFeatureFunctionData();
    }

    virtual ~intfFeatureFunction(){}

    virtual bool nodesHaveSameState(const SearchGraphBuilder::clsSearchGraphNode& _first,
                                    const SearchGraphBuilder::clsSearchGraphNode& _second) const {
        Q_UNUSED(_first)
        Q_UNUSED(_second)
        return true;
    }

    virtual void initialize(const QString& _configFile)  = 0;
    virtual void newSentence(const InputDecomposer::Sentence_t &_inputSentence) {Q_UNUSED(_inputSentence)}

    /**
     * This can be called to score the new hypothesis and initialize its state correctly.
     * The first secondary model will encounter an uninitialized hypothesis state, thus don't forget to call
     * newHypothesisNode.getHypothesisState()->initializeSecondaryModelStatesIfNecessary();
     */
    virtual Common::Cost_t scoreSearchGraphNode(SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode) const =0 ;

    /**
     * @brief getApproximateCost This is to add an approximate cost to the future cost heuristic in SCSS
     */
    // NOTE: Either getRestCostForPosition or this function must return 0
    virtual Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                              unsigned _sourceEnd,
                                              const RuleTable::clsTargetRule& _targetRule) const = 0;


    virtual QStringList columnNames() const = 0;

protected:
    static QString baseScalingFactorsConfigPath(){ return "/ScalingFactors"; }
    QVector<size_t>         FieldIndexes;       /**<  List of indices of this feature function in rule table.*/
    size_t                  PrecomputedIndex;   /**<  Precomputed values of this feature function should be stored in this index of precomputedValues of targetRule.*/
    size_t                  DataIndex;
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_INTFFEATUREFUNCTION_HPP
