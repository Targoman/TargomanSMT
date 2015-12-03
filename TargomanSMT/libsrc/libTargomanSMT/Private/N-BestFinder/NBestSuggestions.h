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

#ifndef TARGOMAN_CORE_PRIVATE_NBESTFINDER_CLSNBESTFINDER_H
#define TARGOMAN_CORE_PRIVATE_NBESTFINDER_CLSNBESTFINDER_H

#include "libTargomanCommon/CmdIO.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Types.h"
#include "Private/SearchGraphBuilder/clsSearchGraph.h"


namespace Targoman{
namespace SMT {
namespace Private{

/**
 *  @brief NBest finder module
 */
namespace NBestFinder {

class NBestSuggestions
{
public:
    struct stuTargetOption{
        stuPos Pos;
        QList<RuleTable::clsTargetRule> TargetRules;


        stuTargetOption(const stuPos& _pos = stuPos(),
                        const QList<RuleTable::clsTargetRule>& _target =
                              QList<RuleTable::clsTargetRule>()){
            this->Pos = _pos;
            this->TargetRules = _target;
        }
    };

    typedef QMap<stuPos, stuTargetOption> Container_t;

public:
    static Container_t retrieve(const SearchGraphBuilder::clsSearchGraph& _searchGraph);
    static QString moduleName(){return "NBest";}
private:
    static size_t fillBestOptions(NBestSuggestions::Container_t& _storage, const SearchGraphBuilder::clsSearchGraph &_searchGraph,
                                  const SearchGraphBuilder::clsSearchGraphNode& _currNode);

private:
    static Common::Configuration::tmplConfigurable<quint8> MaxSuggestions;
    friend class UnitTestNameSpace::clsUnitTest;

};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_NBESTFINDER_CLSNBESTFINDER_H
