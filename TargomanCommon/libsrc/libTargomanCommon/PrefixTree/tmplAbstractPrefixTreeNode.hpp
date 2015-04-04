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

template<class itmplKey_t, class itmplData_t> class tmplAbstractPrefixTreeNode;

template <class itmplKey_t, class itmplData_t> class tmplAbstractPrefixTreeNodeData : public QSharedData {
public:
    tmplAbstractPrefixTreeNodeData():
        Children(
            new QMap<itmplKey_t, tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>>
            )
    { }

    tmplAbstractPrefixTreeNodeData(QMap<itmplKey_t, tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>>* _children):
        Children(_children)
    { }

    tmplAbstractPrefixTreeNodeData(const tmplAbstractPrefixTreeNodeData& _other) :
        QSharedData(_other),
        DataNode(_other.DataNode),
        Children(_other.Children.data())
    { }

    virtual ~tmplAbstractPrefixTreeNodeData(){
    }

    static tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>* invalidInstance() {
        static tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>* Instance = NULL;
        return (Q_LIKELY(Instance) ?
                    Instance :
                    (Instance = new tmplAbstractPrefixTreeNodeData<itmplKey_t,itmplData_t>));
    }

public:
    itmplData_t DataNode;
    //Defined as pointer to be overridable
    QScopedPointer<QMap<itmplKey_t, tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>>> Children;
};

/////////////////////////////////////////////////////////////////////////////////////
template <class itmplKey_t, class itmplData_t> class tmplAbstractPrefixTreeNode {
public:
    tmplAbstractPrefixTreeNode() :
        Data(tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>::invalidInstance())
    { }

    tmplAbstractPrefixTreeNode(tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>* _data) :
        Data(_data)
    { }

    tmplAbstractPrefixTreeNode(const tmplAbstractPrefixTreeNode& _other) :
        Data(_other.Data)
    { }

    virtual ~tmplAbstractPrefixTreeNode()
    {}

    void writeBinary(clsOFStreamExtended& _outStream) const{
        PosType_t StartPosition = _outStream.tellp();
        PosType_t NullPosition = 0;
        _outStream.write(this->Data->Children->size());
        for(auto Iterator = this->Data->Children->begin();
            Iterator != this->Data->Children->end();
            ++Iterator) {
            _outStream.write(Iterator.key());
            _outStream.write(NullPosition);
        }
        QMap<WordIndex_t, PosType_t> ChildPositions;
        this->Data->DataNode.writeBinary(_outStream);
        for(auto Iterator = this->Data->Children->begin();
            Iterator != this->Data->Children->end();
            ++Iterator) {
            ChildPositions[Iterator.key()] = _outStream.tellp();
            Iterator->writeBinary(_outStream);
        }
        PosType_t EndPosition = _outStream.tellp();
        _outStream.seekp(StartPosition + sizeof(int), std::ios_base::beg);
        for(auto Iterator = this->Data->Children->begin();
            Iterator != this->Data->Children->end();
            ++Iterator) {
            _outStream.seekp(sizeof(WordIndex_t), std::ios_base::cur);
            _outStream.write(ChildPositions[Iterator.key()]);
        }
        _outStream.seekp(EndPosition, std::ios_base::beg);
    }

    tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>& getChildByKey(const itmplKey_t _key, bool _detachInvalid) {
        tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>& Node = this->Data->Children->operator [](_key);
        if(Q_LIKELY(_detachInvalid) && Node.isInvalid())
            Node.detachInvalidData();
        return Node;
    }

    inline bool isInvalid() const {
        return (this->Data.constData() == tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>::invalidInstance());
    }

    inline itmplData_t& getData() { return this->Data->DataNode; }


    virtual tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& follow(itmplKey_t _key) {
        auto Iter = this->Data->Children->find(_key);
        if(Iter == this->Data->Children->end())
            return tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::invalidInstance();
        else
            return *Iter;
    }

    static tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& invalidInstance() {
        static tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>* Instance = NULL;
        return *(Q_LIKELY(Instance) ?
                    Instance :
                    (Instance = new tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>()));
    }

    virtual void detachInvalidData(){
        this->Data.detach();
    }

protected:
    QExplicitlySharedDataPointer<tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>> Data;
};

}
}
}

#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTPREFIXTREENODE_H
