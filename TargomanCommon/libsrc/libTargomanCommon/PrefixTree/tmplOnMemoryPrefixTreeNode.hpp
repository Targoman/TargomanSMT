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

namespace Targoman {
namespace Common {
namespace PrefixTree {

template <class Key_t, class Data_t> class tmplOnMemoryPrefixTreeData :
        public tmplAbstractPrefixTreeNodeData<Key_t, Data_t> {
public:
    tmplOnMemoryPrefixTreeData():
        tmplAbstractPrefixTreeNodeData<Key_t, Data_t>(new QMap<Key_t, tmplAbstractPrefixTreeNode<Key_t,Data_t>>())
    {}

    tmplOnMemoryPrefixTreeData(const tmplOnMemoryPrefixTreeData& _other) :
        tmplAbstractPrefixTreeNodeData<Key_t, Data_t>(_other)
    {}
};

/////////////////////////////////////////////////////////////////////////////////////
template <class Key_t, class Data_t> class tmplOnMemoryPrefixTreeNode :
        public tmplAbstractPrefixTreeNode<Key_t, Data_t>{
    tmplOnMemoryPrefixTreeNode() :
        tmplAbstractPrefixTreeNode<Key_t, Data_t>(new tmplOnMemoryPrefixTreeData<Key_t, Data_t>)
    { }

    tmplOnMemoryPrefixTreeNode(clsIFStreamExtended& _inputStream):
        tmplAbstractPrefixTreeNode<Key_t, Data_t>(new tmplOnMemoryPrefixTreeData<Key_t, Data_t>)
    {
        //TODO LoadAll from Binary
/*        int ChildCount = _inputStream.read<int>();
        for(int i = 0; i < ChildCount; ++i) {
            WordIndex_t Key = _inputStream.read<WordIndex_t>();
            PosType_t Position = _inputStream.read<PosType_t>();
            this->Data->ChildPositionInStream[Key] = Position;
        }
        this->Data->DataNode.readBinary(this->Data->InputStream);
        if(_loadChildren) {
            for(auto Iterator = this->Data->ChildPositionInStream.begin();
                Iterator != this->Data->ChildPositionInStream.end();
                ++Iterator) {
                this->loadChildFromDisk(Iterator.key(), true);
            }
        }*/

    }
};

}
}
}

#endif // TMPLONMEMORYPREFIXTREENODE_H
