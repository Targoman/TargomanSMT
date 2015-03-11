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
    { } //TODO convert to cacheable

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
    tmplOnDemandPrefixTreeNode(clsIFStreamExtended& _inputStream, bool _loadChildren = false):
        tmplAbstractPrefixTreeNode<Key_t, Data_t>(new tmplOnDemandPrefixTreeNodeData<Key_t, Data_t>)
    {
        tmplOnDemandPrefixTreeNodeData<Key_t, Data_t>* PrivData =
                (tmplOnDemandPrefixTreeNodeData<Key_t, Data_t>*)this->Data;

        int ChildCount = _inputStream.read<int>();
        for(int i = 0; i < ChildCount; ++i) {
            WordIndex_t Key = _inputStream.read<WordIndex_t>();
            PosType_t Position = _inputStream.read<PosType_t>();
            PrivData->ChildPositionInStream[Key] = Position;
        }
        this->Data->DataNode.readBinary(this->Data->InputStream);
        if(_loadChildren) {
            for(auto Iterator = PrivData->ChildPositionInStream.begin();
                Iterator != PrivData->ChildPositionInStream.end();
                ++Iterator) {
                this->loadChildFromDisk(Iterator.key(), true);
            }
        }
    }

    virtual tmplAbstractPrefixTreeNode<Key_t, Data_t>& follow(Key_t _key) {
        auto Iterator = this->Data->Children.find(_key);
        if(Iterator == this->Data->Children.end()) {
            if(((tmplOnDemandPrefixTreeNodeData<Key_t, Data_t>*)this->Data)->InputStream != NULL)
                return loadChildFromDisk(_key);
            return NULL;
        }
        return *Iterator;
    }

    tmplAbstractPrefixTreeNode<Key_t, Data_t> loadChildFromDisk(Key_t _key, bool _loadChildren = false);
};


}
}
}

#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLONDEMANDPREFIXTREENODE_HPP
