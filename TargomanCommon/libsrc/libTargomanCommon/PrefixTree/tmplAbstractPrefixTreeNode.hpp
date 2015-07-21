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
