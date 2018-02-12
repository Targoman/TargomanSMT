/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#ifndef TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_LEXICALREORDERING_H
#define TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_LEXICALREORDERING_H

#include "Private/FeatureFunctions/intfFeatureFunction.hpp"
#include<iostream>
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
    void initialize(QSharedPointer<QSettings>);

    Common::Cost_t scoreSearchGraphNodeAndUpdateFutureHash(
            SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode,
            const InputDecomposer::Sentence_t& _input,
            QCryptographicHash& _hash) const;
    Common::Cost_t getRestCostForPosition(const Coverage_t& _coverage, size_t _beginPos, size_t endPos) const {
        Q_UNUSED(_coverage);
        Q_UNUSED(_beginPos);
        Q_UNUSED(endPos);
        return 0;
    }
    Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                      unsigned _sourceEnd,
                                      const InputDecomposer::Sentence_t& _input,
                                      const RuleTable::clsTargetRule& _targetRule) const;

    int compareStates(const SearchGraphBuilder::clsSearchGraphNode &_first, const SearchGraphBuilder::clsSearchGraphNode &_second) const;

private:
    enuLexicalReorderingFields::Type getBackwardOreientation(
            SearchGraphBuilder::clsSearchGraphNode &_newHypothesisNode) const;
    enuLexicalReorderingFields::Type getForwardOreientation(
            SearchGraphBuilder::clsSearchGraphNode &_newHypothesisNode) const;

    inline QStringList columnNames() const{
        return LexicalReordering::IsBidirectional.value() ?
                    enuLexicalReorderingFields::options() :
                    enuLexicalReorderingFields::options().mid(0,3);
    }

    void initRootNode(SearchGraphBuilder::clsSearchGraphNode &_rootNode);

private:
    static Common::Configuration::tmplConfigurable<bool>      IsBidirectional;      /**< Whether our lexical reordering is biderctional or not.*/
    static Common::Configuration::tmplConfigurable<double>    ScalingFactors[6];    /**< Scale factor of lrm costs.*/

    TARGOMAN_SMT_DEFINE_FEATUREFUNCTION(LexicalReordering, false)
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_LEXICALREORDERING_H
