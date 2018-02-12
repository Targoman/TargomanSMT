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

#ifndef TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_REORDERINGJUMP_H
#define TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_REORDERINGJUMP_H

#include "Private/FeatureFunctions/intfFeatureFunction.hpp"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

TARGOMAN_ADD_EXCEPTION_HANDLER(exReorderingJump, exFeatureFunction);


class ReorderingJump : public intfFeatureFunction
{
public:
    ~ReorderingJump(){}

    void initialize(QSharedPointer<QSettings>){}

    Common::Cost_t scoreSearchGraphNodeAndUpdateFutureHash(
            SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode,
            const InputDecomposer::Sentence_t& _input,
            QCryptographicHash& _hash) const;

    Common::Cost_t getRestCostForPosition(const Coverage_t& _coverage, size_t _beginPos, size_t endPos) const;

    inline Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                             unsigned _sourceEnd,
                                             const InputDecomposer::Sentence_t& _input,
                                             const RuleTable::clsTargetRule& _targetRule) const {
        Q_UNUSED(_sourceStart)
        Q_UNUSED(_sourceEnd)
        Q_UNUSED(_input)
        Q_UNUSED(_targetRule)
        return 0;
    }

    inline QStringList columnNames() const{return QStringList();}

    void initRootNode(SearchGraphBuilder::clsSearchGraphNode &_rootNode);

private:
    static Common::Cost_t getJumpCost(size_t _jumpWidth) {
        Common::Cost_t Cost = _jumpWidth;

        if (_jumpWidth > ReorderingJump::MaximumJumpWidth.value())
            Cost = _jumpWidth + (_jumpWidth * _jumpWidth);

        return Cost;
    }

private:
    static Common::Configuration::tmplRangedConfigurable<double> ScalingFactor;
    //TODO: change name.
    static Common::Configuration::tmplRangedConfigurable<quint8> MaximumJumpWidth;

    friend class UnitTestNameSpace::clsUnitTest;

    TARGOMAN_SMT_DEFINE_FEATUREFUNCTION(ReorderingJump, true)
};

}
}
}
}


#endif // TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_REORDERINGJUMP_H
