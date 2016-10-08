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

#include "clsRuleNode.h"
#include "libTargomanCommon/FStreamExtended.h"
#include<iostream>

namespace Targoman {
namespace SMT {
namespace Private {
/**
 * @brief Namespace surrounding all classes related to manage, read and write RuleTable.
 */
namespace RuleTable{

using namespace Common;
/**
 * This instance of clsRuleNodeData class is static and is used in initilization of clsRuleNode class.
 */
clsRuleNodeData* InvalidRuleNodeData = new clsRuleNodeData;


/**
 * @brief This is default constructor of clsRuleNode class and it always instantiates #Data member with InvalidRuleNodeData which is a static data pointer of type clsRuleNodeData.
 * This default value for #Data ables us check validity of rule node.
 */
clsRuleNode::clsRuleNode() :
    Data(InvalidRuleNodeData)
{
}

/**
 * @brief #Data member of class is inititialized to InvalidRuleNodeData which is a static instance of clsRuleNodeData.
 * So if we have changed #Data member it is not equal to InvalidRuleNodeData anymore.
 * So if #Data member is not equal to InvalidRuleNodeData is a valid node.
 *
 * @return whether it is a valid node or not.
 */

bool clsRuleNode::isInvalid() const
{
    return (this->Data.data() == InvalidRuleNodeData);
}

void clsRuleNode::readBinary(std::istream &_input)
{
    clsIFStreamExtended& InStream = (clsIFStreamExtended&)(_input);
    int TargetRuleCount = InStream.read<int>();
    if (TargetRuleCount == 0)
        return;
    if(this->isInvalid())
        this->detachInvalidData();
    this->Data->TargetRules.reserve(TargetRuleCount);
    for(int i = 0; i < TargetRuleCount; ++i) {
        clsTargetRule TargetRule;
        TargetRule.readBinary(InStream);
        this->Data->TargetRules.append(TargetRule);
    }
}

void clsRuleNode::writeBinary(std::ostream &_output) const
{
    clsOFStreamExtended& OutStream = (clsOFStreamExtended&)(_output);
    OutStream.write(this->Data->TargetRules.size());
    foreach(const clsTargetRule& TargetRule, this->Data->TargetRules)
        TargetRule.writeBinary(OutStream);
}

}
}
}
}
