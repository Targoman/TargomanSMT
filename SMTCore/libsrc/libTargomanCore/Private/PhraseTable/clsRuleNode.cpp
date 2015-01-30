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
#include "intfPhraseTable.hpp"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable{

using namespace Common;

static clsRuleNodeData SharedNull;

//RulesForSourcePart.cc
clsRuleNode::clsRuleNode() :
    d(&SharedNull)
{

}

clsRuleNode::~clsRuleNode()
{
    //Just to suppress compiler error on QSharedDataPointer
}

bool clsRuleNode::isNull() const
{
    return (this->d.data() == &SharedNull);
}

void clsRuleNode::readBinary(std::istream &input)
{
//TODO
}

void clsRuleNode::writeBinary(std::ostream &output) const
{
//TODO
}

}
}
}
}
