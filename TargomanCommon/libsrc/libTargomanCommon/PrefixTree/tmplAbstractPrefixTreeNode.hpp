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

namespace Targoman {
namespace Common {
namespace PrefixTree {

TARGOMAN_ADD_EXCEPTION_HANDLER(exPrefixTree, exTargomanBase);

template<class Key_t, class Data_t> class tmplAbstractPrefixTreeNode;

template <class Key_t, class Data_t> class tmplAbstractPrefixTreeNodeData : public QSharedData {
public:
    tmplAbstractPrefixTreeNodeData(QMap<Key_t, tmplAbstractPrefixTreeNode<Key_t,Data_t>>* _children):
        Children(_children)
    { }

    tmplAbstractPrefixTreeNodeData(const tmplAbstractPrefixTreeNodeData& _other) :
        QSharedData(_other),
        DataNode(_other.DataNode),
        Children(_other.Children)
    { }

    ~tmplAbstractPrefixTreeNodeData();

public:
    Data_t DataNode;
    QScopedPointer<QMap<Key_t, tmplAbstractPrefixTreeNode<Key_t,Data_t>>> Children;
    //Defined as pointer to be overridable
};

/////////////////////////////////////////////////////////////////////////////////////
template <class Key_t, class Data_t> class tmplAbstractPrefixTreeNode {
public:

    tmplAbstractPrefixTreeNode(const tmplAbstractPrefixTreeNode& _other) :
        Data(_other.Data)
    { }

    void writeBinary(clsOFStreamExtended& _outStream) const;

    tmplAbstractPrefixTreeNode& getChildByKey(const Key_t _key) {
        return this->Data->Children[_key];
    }

    Data_t& getData() {
        return this->Data->DataNode;
    }

    virtual tmplAbstractPrefixTreeNode& follow(Key_t _key) {
        return this->Data->Children.find(_key);
    }

protected:
    QExplicitlySharedDataPointer<tmplAbstractPrefixTreeNodeData<key_t, Data_t>> Data;
};

}
}
}

#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTPREFIXTREENODE_H
