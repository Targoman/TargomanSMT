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
/**
 *  this class holds and manages data members of tmplAbstractPrefixTreeNode.
 *
 */
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
    itmplData_t DataNode;                       /** Data of this node. */
    QScopedPointer<QMap<itmplKey_t, tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>>> Children;  /** This variable holds childrens prefix nodes in a QMap
                                                                                                        which can be accessed by each child key. This QMap is
                                                                                                        stored as pointer to be overridable by other inherited
                                                                                                        classes of QMap.*/
};

/////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief tmplAbstractPrefixTreeNode class is the base class of for other types prefix tree nodes
 * like tmplOnDemandPrefixTreeNode and tmplOnMemoryPrefixTreeNode.
 */
template <class itmplKey_t, class itmplData_t> class tmplAbstractPrefixTreeNode {
public:
    /**
     * @brief default constructor of this class instantiates data member of this class with
     * a unique instance, which will be called "invalid node".
     */
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

    /**
     * @brief writeBinary This function writes size and data of every child of this nodes to
     * output file stream.
     *
     * This node first writes number of his children. Then for each child, first writes its
     * data (which is of type itmplKey_t) and then fills a temporary zero for position of this
     * child. Then each child will be write it self recursively. End position of each child will
     * be stored in a QMap and those temporary zeros will be filled agained with their correct
     * values.
     * @param _outStream output stream.
     */
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
        QMap<itmplKey_t, PosType_t> ChildPositions;
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
            _outStream.seekp(sizeof(itmplKey_t), std::ios_base::cur);
            _outStream.write(ChildPositions[Iterator.key()]);
        }
        _outStream.seekp(EndPosition, std::ios_base::beg);
    }

    /**
     * @brief getChildByKey     Finds or creates a child and returns that child.
     * @param _key              key of the requested child
     * @param _detachInvalid    Specifies whether detach child node or not.
     * @return                  Returns founded or created child.
     */
    tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>& getChildByKey(const itmplKey_t _key, bool _detachInvalid) {
        tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>& Node = this->Data->Children->operator [](_key);
        if(Q_LIKELY(_detachInvalid) && Node.isInvalid())
            Node.detachInvalidData();
        return Node;
    }

    /**
     * @return returns true if this node is the "invalid node" else returns false.
     */
    inline bool isInvalid() const {
        return (this->Data.constData() == tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>::invalidInstance());
    }

    /**
     * @return returns data member of this node.
     */
    inline itmplData_t& getData() { return this->Data->DataNode; }

    /**
     * @brief follow  Goes directly from this node to child node.
     * @param _key    key (data) of child
     * @return        returns node of child if existed else returns "invalid node".
     */
    virtual tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& follow(itmplKey_t _key) {
        auto Iter = this->Data->Children->find(_key);
        if(Iter == this->Data->Children->end())
            return tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::invalidInstance();
        else
            return *Iter;
    }


    /**
     * @brief invalidInstance returns the static "invalid node"
     */
    static tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& invalidInstance() {
        static tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>* Instance = NULL;
        return *(Q_LIKELY(Instance) ?
                    Instance :
                    (Instance = new tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>()));
    }

    /**
     * @brief detaches Invalid Data
     */
    virtual void detachInvalidData(){
        this->Data.detach();
    }

protected:
    QExplicitlySharedDataPointer<tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>> Data; /** A pointer to data member of this class */
};

}
}
}

#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTPREFIXTREENODE_H
