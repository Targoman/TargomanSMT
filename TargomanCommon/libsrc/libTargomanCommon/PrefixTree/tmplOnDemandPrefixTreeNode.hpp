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
