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

#ifndef TARGOMAN_CORE_PRIVATE_PHRASETABLE_RULENODE_H
#define TARGOMAN_CORE_PRIVATE_PHRASETABLE_RULENODE_H

#include <QList>
#include "clsTargetRule.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable{

class clsRuleNodeData : public QSharedData{
    QList<QSharedDataPointer<clsTargetRule> > TargetRules;
};

class clsRuleNode
{
public:
    clsRuleNode();
    clsRuleNode(const clsRuleNode & _other) : d(_other.d){}
    ~clsRuleNode();

    bool isInvalid() const;
    static inline clsRuleNode& invalidRuleNode() {return clsRuleNode::InvalidRuleNode;}

    // Following functions are needed for the binary input/output
    void readBinary(std::istream &input);
    void writeBinary(std::ostream &output) const;
    inline const QList<QSharedDataPointer<clsTargetRule> >& targetRules() const {
        return this->d->TargetRules;//TODO OJO
    }

private:
    QSharedDataPointer<clsRuleNodeData> d;

    static clsRuleNode InvalidRuleNode;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_PHRASETABLE_RULENODE_H
