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

#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLFULLCACHEPREFIXTREENODE_HPP
#define TARGOMAN_COMMON_PREFIXTREE_TMPLFULLCACHEPREFIXTREENODE_HPP

#include "libTargomanCommon/PrefixTree/tmplAbstractOnDiskPrefixTreeNode.hpp"

namespace Targoman {
namespace Common {
namespace PrefixTree {

/////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief This class is a derivation of abstract prefix tree node, which is specific for on Disk
 *  working with prefix tree.
 */
template <class itmplKey_t, class itmplData_t> class tmplFullCachePrefixTreeNode :
        public tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t> {
public:
    typedef typename tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::Node_t Node_t;
    typedef typename tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::pNode_t pNode_t;
public:
    tmplFullCachePrefixTreeNode(clsIFStreamExtended& _inputStream) :
                 tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t>(_inputStream, 0)
    {}

protected:
    virtual pNode_t getOrCreateChildByKey(const itmplKey_t _key) {
        QExplicitlySharedDataPointer<
                tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t>>& Result = this->Data->Children[_key];
        if(Result.data() == NULL)
            Result = new tmplFullCachePrefixTreeNode<itmplKey_t, itmplData_t>(this->Data->InputStream);
        return Result;
    }

    inline pNode_t loadChildFromDisk(itmplKey_t _key) {
        return tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t>::loadChildFromDisk(
                    _key,
                    true,
                    [] (clsIFStreamExtended& _fstream, quint32 _maxItems) {
            Q_UNUSED(_maxItems);
            return new tmplFullCachePrefixTreeNode(_fstream);
        });
    }

};

}
}
}
#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLFULLCACHEPREFIXTREENODE_HPP
