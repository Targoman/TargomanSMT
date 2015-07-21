#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLNOCACHEPREFIXTREENODE_HPP
#define TARGOMAN_COMMON_PREFIXTREE_TMPLNOCACHEPREFIXTREENODE_HPP

#include "libTargomanCommon/PrefixTree/tmplAbstractOnDiskPrefixTreeNode.hpp"

namespace Targoman {
namespace Common {
namespace PrefixTree {

/////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief This class is a derivation of abstract prefix tree node, which is specific for on Disk
 *  working with prefix tree.
 */
template <class itmplKey_t, class itmplData_t> class tmplNoCachePrefixTreeNode :
        public tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t> {
public:
    typedef typename tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::Node_t Node_t;
    typedef typename tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::pNode_t pNode_t;
public:
    tmplNoCachePrefixTreeNode(clsIFStreamExtended& _inputStream) :
                 tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t>(_inputStream, 0)
    {}

protected:
    virtual pNode_t getOrCreateChildByKey(const itmplKey_t _key) {
        QExplicitlySharedDataPointer<
                tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t>>& Result = this->Data->Children[_key];
        if(Result.data() == NULL)
            Result = new tmplNoCachePrefixTreeNode<itmplKey_t, itmplData_t>(this->Data->InputStream);
        return Result;
    }

    inline pNode_t loadChildFromDisk(itmplKey_t _key) {
        return tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t>::loadChildFromDisk(_key, false);
    }

};

}
}
}
#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLNOCACHEPREFIXTREENODE_HPP
