#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTAbstractOnDiskPREFIXTREENODE_HPP
#define TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTAbstractOnDiskPREFIXTREENODE_HPP

#include "libTargomanCommon/PrefixTree/tmplAbstractPrefixTreeNode.hpp"
#include "libTargomanCommon/tmplBoundedCache.hpp"
#include "libTargomanCommon/FStreamExtended.h"

namespace Targoman {
namespace Common {
namespace PrefixTree {

typedef qint64 PosType_t;

template<class itmplKey_t, class itmplData_t> class tmplAbstractOnDiskPrefixTreeNode;

template <class itmplKey_t, class itmplData_t> class tmplAbstractOnDiskPrefixTreeNodeData :
        public QSharedData {
public:
    tmplAbstractOnDiskPrefixTreeNodeData(
            clsIFStreamExtended& _inputStream,
            quint32 _maxCacheItems) :
        InputStream(_inputStream)
    {
        this->Children.setMaxItems(_maxCacheItems);
    }

    /**
     * @brief This is copy constructor for this class.
     */
    tmplAbstractOnDiskPrefixTreeNodeData(const tmplAbstractOnDiskPrefixTreeNodeData& _other) :
        QSharedData(_other),
        NodeData(_other.NodeData),
        Children(_other.Children),
        InputStream(_other.InputStream),
        ChildPositionInStream(_other.ChildPositionInStream)
    { }

public:
    itmplData_t NodeData;
    tmplBoundedCache<QMap, itmplKey_t, QExplicitlySharedDataPointer<tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t>>> Children;
    clsIFStreamExtended& InputStream;
    QMap<itmplKey_t, PosType_t> ChildPositionInStream;
};


/////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief This class is a derivation of abstract prefix tree node, which is specific for on Disk
 *  working with prefix tree.
 */
template <class itmplKey_t, class itmplData_t> class tmplAbstractOnDiskPrefixTreeNode :
        public tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t> {
public:
    typedef typename tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::Node_t Node_t;
    typedef typename tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::pNode_t pNode_t;

public:
    tmplAbstractOnDiskPrefixTreeNode(clsIFStreamExtended& _inputStream, quint32 _maxCacheItems) :
        Data(new tmplAbstractOnDiskPrefixTreeNodeData<itmplKey_t, itmplData_t>(
                 _inputStream, _maxCacheItems))
    {
        int ChildCount = _inputStream.read<int>();
        for(int i = 0; i < ChildCount; ++i) {
            itmplKey_t Key = _inputStream.read<itmplKey_t>();
            PosType_t Position = _inputStream.read<PosType_t>();
            this->Data->ChildPositionInStream[Key] = Position;
        }
        this->Data->NodeData.readBinary(_inputStream);
        this->IsInvalid = false;
    }

    /**
     * @brief follow  Goes directly from this node to child node.
     * @param _key    key of child
     * @return        returns node of child if it is already loaded else loads it  from file .
     */
    virtual pNode_t follow(itmplKey_t _key) {
        auto Iterator = this->Data->Children.find(_key);
        if(Iterator == this->Data->Children.end())
            return loadChildFromDisk(_key);
        else
            return *Iterator;
    }

    itmplData_t& getData() {
        return this->Data->NodeData;
    }


protected:
    /** @brief loadChildFromDisk Loads a child data from disk.
     *
     * This function gets position of this child from #ChildPositionInStream map. If it is not existed
     * returns the "invalid node". If existed, seeks input stream to the begining of that node and
     * inserts that children to #Children Map. then data of that child will be read from binary file.
     *
     * @param _key child key.
     * @return returns child node if founded else returns the "invalid node".
     */
    virtual pNode_t loadChildFromDisk(itmplKey_t _key) {
        Q_UNUSED(_key);
        throw exTargomanNotImplemented("loadChildFromDisk()");
    }

    pNode_t loadChildFromDisk(itmplKey_t _key, bool _updateCache)
    {
        auto PositonIterator = this->Data->ChildPositionInStream.find(_key);
        if(PositonIterator == this->Data->ChildPositionInStream.end())
            return pNode_t(&tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::invalidInstance());
        this->Data->InputStream.lock();
        this->Data->InputStream.seekg(*PositonIterator, std::ios_base::beg);

        typedef tmplAbstractOnDiskPrefixTreeNode<itmplKey_t, itmplData_t> LocalNode_t;
        typedef QExplicitlySharedDataPointer<LocalNode_t> pLocalNode_t;

        pLocalNode_t Node = pLocalNode_t(
                    new LocalNode_t(this->Data->InputStream, this->Data->Children.maxItems()));

        if (_updateCache)
            this->Data->Children.insert(_key, Node);

        this->Data->InputStream.unlock();
        return pNode_t(Node.data());
    }



protected:
    QExplicitlySharedDataPointer<tmplAbstractOnDiskPrefixTreeNodeData<itmplKey_t, itmplData_t>> Data;
};


}
}
}

#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLABSTRACTAbstractOnDiskPREFIXTREENODE_HPP
