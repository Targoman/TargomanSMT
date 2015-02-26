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

/**
 * @brief The clsRuleNodeData class is used to store data member of clsRuleNode class.
 */

class clsRuleNodeData : public QSharedData{
public:
    clsRuleNodeData(){}
    clsRuleNodeData(const clsRuleNodeData& _other):
        QSharedData(_other),
        TargetRules(_other.TargetRules)
    {}
    ~clsRuleNodeData(){}

public:
    QList<clsTargetRule> TargetRules;   /**< A container to store translations for this node (source phrase). */
};

//TODO: RuleNode is not an adequated name must be changed to a  more common sense DB Row name
/**
 * @brief Nodes of Rule Table data structutre is of this type. Each node is corresponding to a source phrase.
 * This class stores translation phrases of target language for this node (source phrase).
 *
 * @note: The string (vector of wordIndex) of source phrase is not stored here or anywhere (It is not important for us).
 *
 */
class clsRuleNode
{
public:
    clsRuleNode();
    clsRuleNode(const clsRuleNode & _other) : Data(_other.Data){}
    ~clsRuleNode(){}

    bool isInvalid() const;
    inline TargetRulesContainer_t& targetRules() {
        return this->Data->TargetRules;
    }

    // Following functions are needed for the binary input/output
    void readBinary(std::istream &_input);
    void writeBinary(std::ostream &_output) const;
    /**
     * @brief detachInvalidData
     *
     * #Data member of every instance of this class points to the same memory place, because it is a shared pointer.
     * we can individualize the memory for an instance by detaching it.
     * detaching should be done once, when the data is invalid.
     * In this function we first check whether it is invalid or not and if it is invalid we detach its memory.
     *
     */
    void detachInvalidData(){
        Q_ASSERT(this->isInvalid());
        this->Data.detach();
    }

//private:
public:
    QExplicitlySharedDataPointer<clsRuleNodeData> Data;  /**< Data member of this class is stored in a seperate class. A shared pointer of this seperate class is stored here */
};

extern clsRuleNodeData* InvalidRuleNodeData;
}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_RULENODE_H
