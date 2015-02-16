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

#ifndef TARGOMAN_CORE_PRIVATE_RULETABLE_RULENODE_H
#define TARGOMAN_CORE_PRIVATE_RULETABLE_RULENODE_H

#include <QList>
#include "clsTargetRule.h"
#include "libTargomanCommon/PrefixTree/tmplFullVectorFilePrefixTree.hpp"

namespace Targoman {
namespace Core {
namespace Private {
namespace RuleTable{

class clsRuleNodeData : public QSharedData{
public:
    clsRuleNodeData(){}
    clsRuleNodeData(const clsRuleNodeData& _other):
        QSharedData(_other),
        TargetRules(_other.TargetRules)
    {}
    ~clsRuleNodeData(){}

public:
    QList<clsTargetRule> TargetRules;
};

class clsRuleNode
{
public:
    clsRuleNode();
    clsRuleNode(const clsRuleNode & _other) : Data(_other.Data){}

    bool isInvalid() const;
    inline QList<clsTargetRule>& targetRules() {
        return this->Data->TargetRules;
    }

    // Following functions are needed for the binary input/output
    void readBinary(std::istream &_input);
    void writeBinary(std::ostream &_output) const;
    void detachInvalidData(){this->Data.detach();}

private:
    QExplicitlySharedDataPointer<clsRuleNodeData> Data;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_RULENODE_H
