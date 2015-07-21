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

#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLONDEMANDPREFIXTREENODE_HPP
#define TARGOMAN_COMMON_PREFIXTREE_TMPLONDEMANDPREFIXTREENODE_HPP

#include "libTargomanCommon/PrefixTree/tmplAbstractOnDiskPrefixTreeNode.hpp"

namespace Targoman {
namespace Common {
namespace PrefixTree {

/////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief This class is a derivation of abstract prefix tree node, which is specific for on Disk
 *  working with prefix tree.
 */

template <class itmplKey_t, class itmplData_t> class tmplOnDemandPrefixTreeNode :
        public tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t> {
public:
    typedef typename tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::Node_t Node_t;
    typedef typename tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::pNode_t pNode_t;
public:
    tmplOnDemandPrefixTreeNode(clsIFStreamExtended& _inputStream, quint64 _maxCachedItems) :
                 tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t>(_inputStream, _maxCachedItems)
    {}

protected:
    inline pNode_t loadChildFromDisk(itmplKey_t _key) {
        return tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t>::loadChildFromDisk(_key, true);
    }

};

}
}
}
#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLONDEMANDPREFIXTREENODE_HPP
