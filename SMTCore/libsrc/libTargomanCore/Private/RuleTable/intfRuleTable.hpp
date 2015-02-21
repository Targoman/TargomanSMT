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

#ifndef TARGOMAN_CORE_PRIVATE_RULETABLE_INTFRULETABLE_H
#define TARGOMAN_CORE_PRIVATE_RULETABLE_INTFRULETABLE_H

#include "libTargomanCommon/Macros.h"
#include "Private/InputDecomposer/clsInput.h"
#include "Private/RuleTable/clsRuleNode.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace RuleTable{

typedef Common::PrefixTree::tmplFullVectorFilePrefixTree<RuleTable::clsRuleNode> RulesPrefixTree_t;

class intfRuleTable : public Common::Configuration::intfModule
{
public:
    intfRuleTable(const QString& _moduleName, quint64 _instanceID) :
    intfModule(_moduleName, _instanceID)
    {}

    virtual ~intfRuleTable(){}

    virtual void init() = 0;

    virtual RuleTable::RulesPrefixTree_t& getPrefixTree(){
        return *this->PrefixTree;
    }

protected:
    QScopedPointer<RulesPrefixTree_t> PrefixTree;
};


}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_INTFRULETABLE_H
