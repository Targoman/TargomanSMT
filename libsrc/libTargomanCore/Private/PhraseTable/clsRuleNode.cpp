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
 */

#include "clsRuleNode.h"
#include "intfPhraseTable.hpp"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable{

using namespace Common;

clsConfigurable<QList<quint8> > clsRuleNode::WhichCosts(intfPhraseTable::baseConfigPath() + "/RuleWhichCosts",
                                                        "Indictaes which costs to use when computing general costs");

//RulesForSourcePart.cc
clsRuleNode::clsRuleNode()
{
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
