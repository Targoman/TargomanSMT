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

#ifndef TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_WORDPENALTY_H
#define TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_WORDPENALTY_H

#include "Private/FeatureFunctions/intfFeatureFunction.hpp"

namespace Targoman {
namespace SMT {
namespace Private {
namespace FeatureFunction{

TARGOMAN_ADD_EXCEPTION_HANDLER(exWordPenalty, exFeatureFunction);


class WordPenalty : public intfFeatureFunction
{
public:
    ~WordPenalty(){}

    void initialize(QPointer<QSettings>){}

    Common::Cost_t scoreSearchGraphNodeAndUpdateFutureHash(
            SearchGraphBuilder::clsSearchGraphNode& _newHypothesisNode,
            QCryptographicHash& _hash) const;

    Common::Cost_t getRestCostForPosition(const Coverage_t& _coverage, size_t _beginPos, size_t endPos) const {
        Q_UNUSED(_coverage)
        Q_UNUSED(_beginPos)
        Q_UNUSED(endPos)
        return 0;
    }

    inline Common::Cost_t getApproximateCost(unsigned _sourceStart,
                                             unsigned _sourceEnd,
                                             const RuleTable::clsTargetRule& _targetRule) const {
        Q_UNUSED(_sourceStart)
        Q_UNUSED(_sourceEnd)
        return this->getWordPenaltyCost(_targetRule);
    }

    inline QStringList columnNames() const{return QStringList();}

    void initRootNode(SearchGraphBuilder::clsSearchGraphNode &_rootNode);

private:
    WordPenalty():
        intfFeatureFunction(this->moduleName(), false)
    {}

    TARGOMAN_DEFINE_SINGLETONMODULE("FeatureFunctions/WordPenalty", WordPenalty);

public:
    inline Common::Cost_t getWordPenaltyCost(const RuleTable::clsTargetRule& _targetRule) const {
        return _targetRule.size() * WordPenalty::ScalingFactor.value();
    }

private:
    static Common::Configuration::tmplConfigurable<double> ScalingFactor;

};


}
}
}
}


#endif // TARGOMAN_CORE_PRIVATE_FEATUREFUNCTIONS_WORDPENALTY_H
