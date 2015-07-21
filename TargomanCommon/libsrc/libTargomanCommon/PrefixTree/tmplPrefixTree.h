/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author Saeed Torabzadeh <saeed.torabzadeh@gmail.com>
 @author Behrooz Vedadian <vedadian@gmail.com>
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_COMMON_PREFIXTREE_CLSPREFIXTREE_H
#define TARGOMAN_COMMON_PREFIXTREE_CLSPREFIXTREE_H

#include "libTargomanCommon/PrefixTree/tmplOnMemoryPrefixTreeNode.hpp"
#include "libTargomanCommon/PrefixTree/tmplOnDemandPrefixTreeNode.hpp"
#include "libTargomanCommon/PrefixTree/tmplFullCachePrefixTreeNode.hpp"
#include "libTargomanCommon/PrefixTree/tmplNoCachePrefixTreeNode.hpp"

namespace Targoman {
namespace Common {
namespace PrefixTree {

TARGOMAN_DEFINE_ENHANCED_ENUM(enuBinaryLoadMode,
                              OnMemory,
                              OnDemand,
                              FullCache,
                              NoCache);

/**
 * @brief the tmplPrefixTree class is a template data structure to hold data in prefix tree.
 *
 * this data structure is a tree and stores prefix data in manner that every key with same prefix
 * have same parrent. For example if we want to store "I go" and "I went", this will have one
 * root node, this root node is connected to a node that stores "I" and two nodes corresponding
 * to "go" and "went" will be connected to "I", because both of them have same prefix (so
 * same parrent). This prefix tree can load all tree on memory or it can load data on demand. Two
 * classes of tmplOnDemandPrefixTreeNode and tmplOnMemoryPrefixTreeNode is corresponding to these
 * two variation of working with data. Both of these classes are extended from
 * tmplAbstractPrefixTreeNode.
 *
 * itmplKey_t is input template key which can be for example WordIndex_t.
 * itmplData_t is input template Data which can be for example clsRuleNode.
 *
 */
template <class itmplKey_t, class itmplData_t> class tmplPrefixTree {
public:
    typedef typename tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::Node_t Node_t;
    typedef typename tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::pNode_t pNode_t;

public:
    /**
     * @brief constructor of this class makes the root node of this tree. It assumes that we want
     * to load all prefix tree on memory. So it creates a tmplOnMemoryPrefixTreeNode as a node
     * type of root node. later if readBinary() function of class is called root node can be
     * changed to tmplOnDemandPrefixTreeNode.
     */
    tmplPrefixTree() :
        RootNode(new tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>())
    { }

    ~tmplPrefixTree(){
        //Just to suppress compiler Error on QScoppedPointer
    }

    /**
     * @brief readBinary    this function loads prefix tree from binary file.
     * @param _file         input file stream
     * @param _loadAll      if this argument is true root node will be of type
     *                      tmplOnMemoryPrefixTreeNode and if not it will be of type
     *                      tmplOnDemandPrefixTreeNode
     */
    void readBinary(clsIFStreamExtended& _file, enuBinaryLoadMode::Type _binLoadMode, quint32 _maxCachedItems){
        switch(_binLoadMode){
        case enuBinaryLoadMode::OnMemory:
            this->RootNode = new tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>(_file);
            break;
        case enuBinaryLoadMode::OnDemand:
            this->RootNode = new tmplOnDemandPrefixTreeNode<itmplKey_t, itmplData_t>(_file, _maxCachedItems);
            break;
        case enuBinaryLoadMode::FullCache:
            this->RootNode = new tmplFullCachePrefixTreeNode<itmplKey_t, itmplData_t>(_file);
            break;
        case enuBinaryLoadMode::NoCache:
            this->RootNode = new tmplNoCachePrefixTreeNode<itmplKey_t, itmplData_t>(_file);
            break;
        default:
            throw exPrefixTree("Invalid Binary Load Mode");
        }
    }

    /**
     * @brief writeBinary   writes prefix tree in binary format to file.
     * @param _stream       output file stream.
     */
    inline void writeBinary(Common::clsOFStreamExtended& _stream) const {
        this->RootNode->writeBinary(_stream);
    }

    /**
     * @brief getOrCreateNode   this function finds or creates a node in the prefix tree for
     *                          input list of template keys.
     *
     *                          each key in the input list is child of prefix key (which is in
     *                          previous index in the list). so we can go deep the tree using
     *                          root node and every prefixes of input keys in order.
     * @param _path             this is list of input template keys. For example it can be a
     *                          list of word indices of a sentence.
     * @return                  returns created or founded node.
     */
    inline pNode_t getOrCreateNode(const QList<itmplKey_t>& _path) {
        pNode_t Result = this->RootNode;
        foreach(itmplKey_t Key, _path)
            Result = Result->getOrCreateChildByKey(Key);
        return Result;
    }
    /**
     * @brief returns root node of prefix tree.
     */
    inline pNode_t rootNode() {
        return this->RootNode;
    }

private:
    pNode_t RootNode;        /** A pointer to root node of this preefix data. */
};

}
}
}
#endif // TARGOMAN_COMMON_PREFIXTREE_CLSPREFIXTREE_H
