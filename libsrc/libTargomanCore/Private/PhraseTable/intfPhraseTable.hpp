/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
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

#ifndef TARGOMAN_CORE_PRIVATE_PHRASETABLE_INTFPHRASETABLE_H
#define TARGOMAN_CORE_PRIVATE_PHRASETABLE_INTFPHRASETABLE_H

#include "libTargomanCommon/Macros.h"
#include "Private/InputDecomposer/clsInput.h"
#include "Private/PhraseTable/clsRuleNode.h"
#include "libTargomanCommon/PrefixTree/tmplFullVectorFilePrefixTree.hpp"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable{

TARGOMAN_ADD_EXCEPTION_HANDLER(exPhraseTable, exTargomanCore);

typedef Common::PrefixTree::tmplFullVectorFilePrefixTree<clsRuleNode> RulesPrefixTree_t;
//typedef ::Core::tmplFullVectorFilePrefixTree<clsRuleNode> RulesPrefixTree_t;
/*
class clsRulesPrefixTreeNode : RulesPrefixTree_t::Node{
public:
    const clsRulesPrefixTreeNode& follow(Common::WordIndex_t _index){return clsRulesPrefixTreeNode::invalidPTNode();}
    clsRuleNode& getData(){return clsRuleNode::invalidRuleNode();}
    inline bool isValid(){return true;}

    static clsRulesPrefixTreeNode& invalidPTNode(){return NullNode;}
private:
    static clsRulesPrefixTreeNode NullNode;
};
*/

class intfPhraseTable : public Common::Configuration::intfModule
{
public:
    intfPhraseTable(const QString& _typeName):
        intfModule(_typeName){}

    virtual void init() = 0;
    virtual void newSentence() = 0;
    virtual bool isBinary() = 0;
    virtual RulesPrefixTree_t& getPrefixTree(){
        this->PrefixTree;
    }

private:
    RulesPrefixTree_t PrefixTree;
};


}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_PHRASETABLE_INTFPHRASETABLE_H
