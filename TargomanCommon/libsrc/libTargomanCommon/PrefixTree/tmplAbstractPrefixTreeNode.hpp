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

#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTPREFIXTREENODE_H
#define TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTPREFIXTREENODE_H

#include <QExplicitlySharedDataPointer>
#include <QMap>
#include "libTargomanCommon/FStreamExtended.h"
#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/exTargomanBase.h"

typedef quint64 PosType_t;

namespace Targoman {
namespace Common {
namespace PrefixTree {

TARGOMAN_ADD_EXCEPTION_HANDLER(exPrefixTree, exTargomanBase);

template <class itmplKey_t, class itmplData_t> class tmplAbstractPrefixTreeNode : public QSharedData {
public:
    typedef tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t> Node_t;
    typedef QExplicitlySharedDataPointer<Node_t> pNode_t;

public:
    tmplAbstractPrefixTreeNode(const tmplAbstractPrefixTreeNode& _other) :
        QSharedData(_other)
    {
        this->IsInvalid = _other.IsInvalid;
    }

    virtual ~tmplAbstractPrefixTreeNode()
    { }

    virtual void writeBinary(clsOFStreamExtended& _outStream) const{
        Q_UNUSED(_outStream);
        throw exTargomanNotImplemented("writeBinary()");
    }

    virtual pNode_t getOrCreateChildByKey(const itmplKey_t _key) {
        Q_UNUSED(_key);
        throw exTargomanNotImplemented("getOrCreateChildByKey()");
    }

    /**
     * @return returns true if this node is the "invalid node" else returns false.
     */
    inline bool isInvalid() const {
        return this->IsInvalid;
    }

    virtual itmplData_t& getData() {
        static itmplData_t InvalidNodeData;
        return InvalidNodeData;
    }

    virtual pNode_t follow(itmplKey_t _key) {
        Q_UNUSED(_key);
        return pNode_t(&tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>::invalidInstance());
    }

protected:
    tmplAbstractPrefixTreeNode()
    {
        this->IsInvalid = true;
    }

    bool IsInvalid;

protected:
    static Node_t& invalidInstance() {
        static Node_t InvalidInstance;
        if(Q_UNLIKELY(InvalidInstance.ref == 0))
            InvalidInstance.ref.ref();
        return InvalidInstance;
    }

};

}
}
}

#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTPREFIXTREENODE_H
