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

#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLONDEMANDPREFIXTREENODE_HPP
#define TARGOMAN_COMMON_PREFIXTREE_TMPLONDEMANDPREFIXTREENODE_HPP

#include "libTargomanCommon/PrefixTree/tmplAbstractPrefixTreeNode.hpp"
#include "libTargomanCommon/tmplExpirableCache.hpp"

namespace Targoman {
namespace Common {
namespace PrefixTree {

typedef quint32 PosType_t;

//TODO configure cache

template <class Key_t, class Data_t> class tmplOnDemandPrefixTreeNodeData :
        public tmplAbstractPrefixTreeNodeData<Key_t, Data_t> {
public:
    tmplOnDemandPrefixTreeNodeData():
        tmplAbstractPrefixTreeNodeData<Key_t, Data_t>(
            new tmplExpirableCache<QMap,Key_t, tmplAbstractPrefixTreeNode<Key_t,Data_t>>())
    { }

    tmplOnDemandPrefixTreeNodeData(const tmplOnDemandPrefixTreeNodeData& _other) :
        tmplAbstractPrefixTreeNodeData<Key_t, Data_t>(_other),
        InputStream(_other.InputStream),
        ChildPositionInStream(_other.ChildPositionInStream)
    { }

    tmplOnDemandPrefixTreeNodeData(clsIFStreamExtended& _inputStream) :
        InputStream(_inputStream)
    { }

public:
    clsIFStreamExtended& InputStream;
    QMap<Key_t, PosType_t> ChildPositionInStream;
};
/////////////////////////////////////////////////////////////////////////////////////

template <class Key_t, class Data_t> class tmplOnDemandPrefixTreeNode :
        public tmplAbstractPrefixTreeNode<Key_t, Data_t>{
public:
    tmplOnDemandPrefixTreeNode(clsIFStreamExtended& _inputStream):
        tmplAbstractPrefixTreeNode<Key_t, Data_t>(
            tmplOnDemandPrefixTreeNodeData<Key_t,Data_t>::invalidInstance())
    {
        tmplOnDemandPrefixTreeNodeData<Key_t, Data_t>* PrivData =
                ((tmplOnDemandPrefixTreeNodeData<Key_t, Data_t>*)this->Data.data());

        int ChildCount = _inputStream.read<int>();
        for(int i = 0; i < ChildCount; ++i) {
            WordIndex_t Key = _inputStream.read<WordIndex_t>();
            PosType_t Position = _inputStream.read<PosType_t>();
            PrivData->ChildPositionInStream[Key] = Position;
        }
        this->Data->DataNode.readBinary(PrivData->InputStream);
    }

    virtual tmplAbstractPrefixTreeNode<Key_t, Data_t>& follow(Key_t _key) {
        auto Iterator = this->Data->Children->find(_key);
        if(Iterator == this->Data->Children->end())
            return loadChildFromDisk(_key);
        else
            return *Iterator;
    }

    tmplAbstractPrefixTreeNode<Key_t, Data_t>& loadChildFromDisk(Key_t _key){
        tmplOnDemandPrefixTreeNodeData<Key_t, Data_t>* PrivData =
                ((tmplOnDemandPrefixTreeNodeData<Key_t, Data_t>*)this->Data.data());

        auto PositonIterator = PrivData->ChildPositionInStream.find(_key);
        if(PositonIterator == PrivData->ChildPositionInStream.end())
            return *tmplAbstractPrefixTreeNode<Key_t, Data_t>::invalidInstance();
        PrivData->InputStream.lock();
        PrivData->InputStream.seekg(*PositonIterator, std::ios_base::beg);
        auto Iterator = PrivData->Children->insert(
                    _key,
                    tmplOnDemandPrefixTreeNode<Key_t, Data_t>(PrivData->InputStream)
                    );
        PrivData->InputStream.unlock();
        return *Iterator;
    }
};


}
}
}

#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLONDEMANDPREFIXTREENODE_HPP
