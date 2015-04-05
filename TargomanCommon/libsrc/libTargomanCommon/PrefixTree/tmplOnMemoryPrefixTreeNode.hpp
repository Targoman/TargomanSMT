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


/**
 * @brief  This class is a derivation of tmplAbstractPrefixTreeNodeData class which just adds two
 * constructor to base class specific for tmplOnMemoryPrefixTreeNode class.
 */
template <class itmplKey_t, class itmplData_t> class tmplOnMemoryPrefixTreeNodeData :
        public tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t> {
public:
    tmplOnMemoryPrefixTreeNodeData():
        tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>(new QMap<itmplKey_t, tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>>())
    {}

    tmplOnMemoryPrefixTreeNodeData(const tmplOnMemoryPrefixTreeNodeData& _other) :
        tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>(_other)
    {}
};

/////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This class is a derivation of abstract prefix tree node, which is specific for on memory
 * working with prefix tree.
 */
template <class itmplKey_t, class itmplData_t> class tmplOnMemoryPrefixTreeNode :
        public tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>{
public:
    tmplOnMemoryPrefixTreeNode() :
        tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>(new tmplOnMemoryPrefixTreeNodeData<itmplKey_t, itmplData_t>)
    { }

    /**
     * @brief loadBinary    Loads all prefix tree from input stream
     */
    void loadBinary(clsIFStreamExtended& _inputStream)
    {
        Q_UNUSED(_inputStream);
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

    /**
     * @brief createRootNode    Creates and returns root node of tmplOnMemoryPrefixTreeNode type
     *                          and loads all prefix tree from input stream.
     * @param _inputStream      Input stream.
     * @return                  Returns root node.
     */
    inline static tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>* createRootNode(clsIFStreamExtended& _inputStream) {
        tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>* Root = createRootNode();
        Root->loadBinary(_inputStream);
        return Root;
    }

    /**
     * @brief createRootNode    Creates and returns root node of tmplOnMemoryPrefixTreeNode type.
     * @return                  Returns root node.
     */
    inline static tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>* createRootNode() {
        tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::invalidInstance();
        tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>* Root = new
                tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>;
        Root->Data->ref.ref();
        Root->detachInvalidData();
        return Root;
    }

};

}
}
}

#endif // TMPLONMEMORYPREFIXTREENODE_H
