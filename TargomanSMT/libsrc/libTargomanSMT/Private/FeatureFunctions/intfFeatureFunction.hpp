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
#include "libTargomanSMT/Types.h"
#include "Private/SearchGraph/clsSearchGraphNode.h"

namespace Targoman{
namespace SMT {
namespace Private{
/**
 * @brief Namespace surrounding all feature function.
 */
namespace FeatureFunction {

TARGOMAN_ADD_EXCEPTION_HANDLER(exFeatureFunction, exTargomanCore);
/**
 * @brief The intfFeatureFunction class is an interface class that every feature function like phrase table or lexical reordering table is derivated from that.
 */
class intfFeatureFunction : public Common::Configuration::intfModule
{
public:
    /**
     * @brief constructor of each feature function specifies its ability about computing position specific rest costs,
     * Its index in PreComputed vector of target rule class data.
     * @param _moduleName
     * @param _canComputePositionSpecificRestCost
     */
    intfFeatureFunction(const QString& _moduleName, bool _canComputePositionSpecificRestCost) :
        intfModule(_moduleName)
    {
        gConfigs.ActiveFeatureFunctions.insert(_moduleName, this);
        this->CanComputePositionSpecificRestCost = _canComputePositionSpecificRestCost;
        this->PrecomputedIndex = RuleTable::clsTargetRule::allocatePrecomputedValue();
        this->DataIndex =  SearchGraph::clsSearchGraphNode::allocateFeatureFunctionData();
    }

    virtual ~intfFeatureFunction(){}

    /**
     * @brief nodesHaveSameState checks equality of states of two search graph noedes.
     * For feature functions like language model that stores their previous states this function should be oveloaded and for others it returns true.
     * @param _first First node.
     * @param _second Second node.
     * @return returns true if both of nodes have same state.
     */
    virtual bool nodesHaveSameState(const SearchGraph::clsSearchGraphNode& _first,
                                    const SearchGraph::clsSearchGraphNode& _second) const {
        Q_UNUSED(_first)
        Q_UNUSED(_second)
        return true;
    }

    /**
     * @brief Stores field indices of this feature function to #FieldIndexes.
     */
    virtual void initialize(const QString& _configFile)  = 0;

    virtual void newSentence(const InputDecomposer::Sentence_t &_inputSentence) {Q_UNUSED(_inputSentence)}

    /**
     * @brief This can be called to score the new hypothesis and initialize its state correctly.
     * The first secondary model will encounter an uninitialized hypothesis state, thus don't forget to call
     * newHypothesisNode.getHypothesisState()->initializeSecondaryModelStatesIfNecessary();
     */
    virtual Common::Cost_t scoreSearchGraphNode(SearchGraph::clsSearchGraphNode& _newHypothesisNode) const =0 ;

    /**
     * @brief Returns wethere this feature function can compute node specific rest costs or not. If it can,
     * the getRestCostForPosition must return valid values
     * otherwise getApproximateCost will be called when creating RestCost matrix
     */
    inline bool canComputePositionSpecificRestCost() const { return this->CanComputePositionSpecificRestCost; }

    /**
     * @brief This can be called to compute node specific rest costs is implemented
     * If the implemeneted feature function does not have this capability it
     * must explicitly return a 0 as the result of execution of this function
     * @note Either getApproximateCost or this function must return 0
     */
    virtual Common::Cost_t getRestCostForPosition(const Coverage_t& _coverage, size_t _beginPos, size_t endPos) const = 0;

     /**
      * @brief Computes approximate cost to the future cost heuristic
      * @note Either getRestCostForPosition or this function must return 0
      */
    virtual Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                              unsigned _sourceEnd,
                                              const RuleTable::clsTargetRule& _targetRule) const = 0;

    /**
     * @brief This function will be called in the constructor of searchGraphNode
     * in order to always have a valid previous node data for feature functions in scoreSearchGraphNode function.
     * @param _rootNode
     */
    virtual void initRootNode(SearchGraph::clsSearchGraphNode &_rootNode) = 0;

    /**
     * @return Returns string list of features' names.
     */
    virtual QStringList columnNames() const = 0;

protected:
    static QString baseScalingFactorsConfigPath(){ return "/ScalingFactors"; }
    bool                    CanComputePositionSpecificRestCost;         /**<  Whether this feature function can compute rest cost based on position or not.*/
    QVector<size_t>         FieldIndexes;                               /**<  List of indices correspond to this feature function in rule table.*/
    size_t                  PrecomputedIndex;                           /**<  Precomputed values of this feature function should be stored in this index of precomputedValues of targetRule.*/
    size_t                  DataIndex;                                  /**<  Each feature function has a field in FeatureFunctionsData data of clsSearchGraphNode class. Index of This feature is stored in this data member. */
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_INTFFEATUREFUNCTION_HPP
