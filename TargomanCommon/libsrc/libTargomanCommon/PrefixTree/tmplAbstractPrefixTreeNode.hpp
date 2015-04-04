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

typedef quint32 PosType_t;

namespace Targoman {
namespace Common {
namespace PrefixTree {

TARGOMAN_ADD_EXCEPTION_HANDLER(exPrefixTree, exTargomanBase);

template<class itmplKey_t, class itmplData_t> class tmplAbstractPrefixTreeNode;

template <class itmplKey_t, class itmplData_t> class tmplAbstractPrefixTreeNodeData : public QSharedData {
public:
    tmplAbstractPrefixTreeNodeData(QMap<itmplKey_t, tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>>* _children):
        Children(_children)
    { }

    tmplAbstractPrefixTreeNodeData(const tmplAbstractPrefixTreeNodeData& _other) :
        QSharedData(_other),
        DataNode(_other.DataNode),
        Children(_other.Children.data())
    { }

    ~tmplAbstractPrefixTreeNodeData(){
        //Just to suppressCompiler Error on QScoppedPointer
    }

public:
    itmplData_t DataNode;
    QScopedPointer<QMap<itmplKey_t, tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>>> Children;
    //Defined as pointer to be overridable
};

/////////////////////////////////////////////////////////////////////////////////////
template <class itmplKey_t, class itmplData_t> class tmplAbstractPrefixTreeNode {
public:
    tmplAbstractPrefixTreeNode(tmplAbstractPrefixTreeNodeData<itmplKey_t,itmplData_t>* _data = NULL) :
        Data(_data)
    { }

    tmplAbstractPrefixTreeNode(const tmplAbstractPrefixTreeNode& _other) :
        Data(_other.Data)
    { }

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
            Node.Data.detach();
        return Node;
    }

    inline bool isInvalid() const {
        return this->Data.constData() == NULL;
    }

    inline itmplData_t& getData() { return this->Data->DataNode; }

    virtual tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& follow(itmplKey_t _key) {
        return *this->Data->Children->find(_key);
    }

    static tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>*  invalidInstance() {
        static tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>* Instance = NULL;
        return (Q_LIKELY(Instance) ?
                    Instance :
                    (Instance = new tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>()));
    }

protected:
    QExplicitlySharedDataPointer<tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>> Data;
};

}
}
}

#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTPREFIXTREENODE_H
