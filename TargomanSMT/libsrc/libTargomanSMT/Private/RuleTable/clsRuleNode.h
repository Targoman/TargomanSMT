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

#ifndef TARGOMAN_CORE_PRIVATE_RULETABLE_RULENODE_H
#define TARGOMAN_CORE_PRIVATE_RULETABLE_RULENODE_H

#include <QList>
#include "clsTargetRule.h"

namespace Targoman {
namespace SMT {
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
    clsRuleNode(const clsRuleNode & _other, bool _isUnknown = false) : Data(_other.Data) {
        if(Q_UNLIKELY(_isUnknown))
            for(clsTargetRule& TargetRule : this->Data->TargetRules)
                TargetRule.setIsUnknownWord(true);
    }

    ~clsRuleNode(){
        // InvalidRuleNodeData needs to be alive during the whole run
        // otherwise we will encounter invalid rule nodes with
        // invalid invalid data!
        if(this->isInvalid()) {
            this->Data->ref.ref();
        }
    }

    bool isInvalid() const;
    inline TargetRulesContainer_t& targetRules() {
        return this->Data->TargetRules;
    }

    inline const TargetRulesContainer_t& targetRules() const {
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

private:
    QExplicitlySharedDataPointer<clsRuleNodeData> Data;  /**< Data member of this class is stored in a seperate class. A shared pointer of this seperate class is stored here */
    friend class UnitTestNameSpace::clsUnitTest;
};

extern clsRuleNodeData* InvalidRuleNodeData;

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_RULENODE_H
