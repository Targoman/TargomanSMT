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

namespace Targoman {
namespace Core {
namespace Private {
namespace RuleTable{

using namespace Common;

static clsRuleNodeData InvalidRuleNodeData;

//RulesForSourcePart.cc
clsRuleNode::clsRuleNode() :
    Data(&InvalidRuleNodeData)
{}

bool clsRuleNode::isInvalid() const
{
    return (this->Data.data() == &InvalidRuleNodeData);
}

void clsRuleNode::readBinary(std::istream &_input)
{
    Q_UNUSED(_input)
//TODO
}

void clsRuleNode::writeBinary(std::ostream &_output) const
{
    Q_UNUSED(_output)
//TODO
}

}
}
}
}
