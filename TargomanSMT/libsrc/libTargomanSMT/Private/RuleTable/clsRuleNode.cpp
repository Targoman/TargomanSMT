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

#include "clsRuleNode.h"
#include "FStreamExtended.h"

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
{}

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
    Q_UNUSED(_input)
//TODO
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
