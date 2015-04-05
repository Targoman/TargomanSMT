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

template <class itmplKey_t, class itmplData_t> class tmplAbstractPrefixTreeNode {
public:
    tmplAbstractPrefixTreeNode(const tmplAbstractPrefixTreeNode& _other)
    {
        this->IsInvalid = _other.IsInvalid;
    }

    virtual ~tmplAbstractPrefixTreeNode()
    { }

    virtual void writeBinary(clsOFStreamExtended& _outStream) const{
        Q_UNUSED(_outStream);
        throw exPrefixTree("AbstractPrefixTreeNode can not be written to streams.");
    }

    virtual tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>& getChildByKey(const itmplKey_t _key, bool _createIfNotFound) {
        Q_UNUSED(_key);
        Q_UNUSED(_createIfNotFound);
        throw exPrefixTree("AbstractPrefixTreeNode can not be extended.");
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

    virtual tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& follow(itmplKey_t _key) {
        Q_UNUSED(_key);
        return tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>::invalidInstance();
    }

protected:
    tmplAbstractPrefixTreeNode()
    {
        this->IsInvalid = true;
    }

    bool IsInvalid;

protected:
    static tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& invalidInstance() {
        static tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t> InvalidInstance;
        return InvalidInstance;
    }

};

}
}
}

#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTPREFIXTREENODE_H
