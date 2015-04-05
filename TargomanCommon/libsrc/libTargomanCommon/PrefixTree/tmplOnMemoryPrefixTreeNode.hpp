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

#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLONMEMORYPREFIXTREENODE_H
#define TARGOMAN_COMMON_PREFIXTREE_TMPLONMEMORYPREFIXTREENODE_H

#include "libTargomanCommon/PrefixTree/tmplAbstractPrefixTreeNode.hpp"
#include "libTargomanCommon/tmplExpirableCache.hpp"


namespace Targoman {
namespace Common {
namespace PrefixTree {

template <class itmplKey_t, class itmplData_t> class tmplOnMemoryPrefixTreeNode;

template <class itmplKey_t, class itmplData_t> class tmplOnMemoryPrefixTreeNodeData : public QSharedData {
public:
    tmplOnMemoryPrefixTreeNodeData()
    { }

    tmplOnMemoryPrefixTreeNodeData(const tmplOnMemoryPrefixTreeNodeData& _other) :
        QSharedData(_other),
        NodeData(_other.NodeData),
        Children(_other.Children)
    { }

public:
    itmplData_t NodeData;
    QMap<itmplKey_t, tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>> Children;
};

/////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This class is a derivation of abstract prefix tree node, which is specific for on memory
 * working with prefix tree.
 */
template <class itmplKey_t, class itmplData_t> class tmplOnMemoryPrefixTreeNode :
        public tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t> {
public:
    tmplOnMemoryPrefixTreeNode() :
        Data(new tmplOnMemoryPrefixTreeNodeData<itmplKey_t, itmplData_t>())
    {
        this->IsInvalid = false;
    }

    // TODO: This will be super-slow, make it run in a normal duration
    tmplOnMemoryPrefixTreeNode(clsIFStreamExtended& _inputStream) :
        Data(new tmplOnMemoryPrefixTreeNodeData<itmplKey_t, itmplData_t>())
    {
        int ChildCount = _inputStream.read<int>();
        for(int i = 0; i < ChildCount; ++i) {
            itmplKey_t Key = _inputStream.read<itmplKey_t>();
            PosType_t Position = _inputStream.read<PosType_t>();
            PosType_t CurrentPosition = _inputStream.tellp();
            _inputStream.seekp(Position);
            this->Data->Children[Key] = tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>(_inputStream);
            _inputStream.seekp(CurrentPosition);
        }
        this->IsInvalid = false;
    }

    virtual tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& follow(itmplKey_t _key) {
        auto Iter = this->Data->Children.find(_key);
        if(Iter == this->Data->Children.end())
            return tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::invalidInstance();
        else
            return *Iter;
    }

    virtual tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>& getChildByKey(const itmplKey_t _key, bool _createIfNotFound) {
        auto Iterator = this->Data->Children.find(_key);
        if(Iterator == this->Data->Children.end()) {
            if(Q_LIKELY(_createIfNotFound))
                Iterator = this->Data->Children.insert(_key,
                                                       tmplOnMemoryPrefixTreeNode<itmplKey_t,itmplData_t>()
                                                       );
            else
                return tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>::invalidInstance();
        }
        return *Iterator;
    }

    void writeBinary(clsOFStreamExtended& _outStream) const {
        PosType_t StartPosition = _outStream.tellp();
        PosType_t NullPosition = 0;
        _outStream.write(this->Data->Children.size());
        for(auto Iterator = this->Data->Children.begin();
            Iterator != this->Data->Children.end();
            ++Iterator) {
            _outStream.write(Iterator.key());
            _outStream.write(NullPosition);
        }
        QMap<WordIndex_t, PosType_t> ChildPositions;
        this->Data->NodeData.writeBinary(_outStream);
        for(auto Iterator = this->Data->Children.begin();
            Iterator != this->Data->Children.end();
            ++Iterator) {
            ChildPositions[Iterator.key()] = _outStream.tellp();
            Iterator->writeBinary(_outStream);
        }
        PosType_t EndPosition = _outStream.tellp();
        _outStream.seekp(StartPosition + sizeof(int), std::ios_base::beg);
        for(auto Iterator = this->Data->Children.begin();
            Iterator != this->Data->Children.end();
            ++Iterator) {
            _outStream.seekp(sizeof(WordIndex_t), std::ios_base::cur);
            _outStream.write(ChildPositions[Iterator.key()]);
        }
        _outStream.seekp(EndPosition, std::ios_base::beg);
    }

private:
    QExplicitlySharedDataPointer<tmplOnMemoryPrefixTreeNodeData<itmplKey_t, itmplData_t>> Data;
};

}
}
}

#endif // TMPLONMEMORYPREFIXTREENODE_H
