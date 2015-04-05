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
#include "libTargomanCommon/FStreamExtended.h"

namespace Targoman {
namespace Common {
namespace PrefixTree {

typedef quint64 PosType_t;

//TODO configure cache

template <class itmplKey_t, class itmplData_t> class tmplOnDemandPrefixTreeNode;

/**
 * @brief  This class is a derivation of tmplAbstractPrefixTreeNodeData class which just adds two
 * constructors and two data members to base class specific for tmplOnDemandPrefixTreeNode class.
 */
template <class itmplKey_t, class itmplData_t> class tmplOnDemandPrefixTreeNodeData :
        public tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t> {
public:
    /**
     * @brief This constructor of class sets InputStream and instantiates
     * parrent's tmplAbstractPrefixTreeNodeData with tmplAbstractPrefixTreeNodeData.
     */
    tmplOnDemandPrefixTreeNodeData(clsIFStreamExtended& _inputStream):
        tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>(
            new tmplExpirableCache<QMap,itmplKey_t, tmplAbstractPrefixTreeNode<itmplKey_t,itmplData_t>>()),
        InputStream(_inputStream)
    { }

    /**
     * @brief This is copy constructor for this class.
     */
    tmplOnDemandPrefixTreeNodeData(const tmplOnDemandPrefixTreeNodeData& _other) :
        tmplAbstractPrefixTreeNodeData<itmplKey_t, itmplData_t>(_other),
        InputStream(_other.InputStream),
        ChildPositionInStream(_other.ChildPositionInStream)
    { }

public:
    clsIFStreamExtended& InputStream;                   /** Input stream */
    QMap<itmplKey_t, PosType_t> ChildPositionInStream;  /** This variable stores position of storing each child in the file in QMap.*/
};
/////////////////////////////////////////////////////////////////////////////////////

/**
 *  @brief This class is a derivation of abstract prefix tree node, which is specific for on demand
 *  working with prefix tree.
 */
template <class itmplKey_t, class itmplData_t> class tmplOnDemandPrefixTreeNode :
        public tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>{
public:
    tmplOnDemandPrefixTreeNode(clsIFStreamExtended& _inputStream) :
        tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>(
            new tmplOnDemandPrefixTreeNodeData<itmplKey_t, itmplData_t>(_inputStream)
            )
    { }

    /**
     * @brief follow  Goes directly from this node to child node.
     * @param _key    key of child
     * @return        returns node of child if it is already loaded else loads it  from file .
     */
    virtual tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& follow(itmplKey_t _key) {
        auto Iterator = this->Data->Children->find(_key);
        if(Iterator == this->Data->Children->end())
            return loadChildFromDisk(_key);
        else
            return *Iterator;
    }

    /**
     * @brief loadChildFromDisk Loads a child data from disk.
     *
     * This function gets position of this child from #ChildPositionInStream map. If it is not existed
     * returns the "invalid node". If existed, seeks input stream to the begining of that node and
     * inserts that children to #Children Map. then data of that child will be read from binary file.
     *
     * @param _key child key.
     * @return returns child node if founded else returns the "invalid node".
     */
    tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& loadChildFromDisk(itmplKey_t _key){
        tmplOnDemandPrefixTreeNodeData<itmplKey_t, itmplData_t>* PrivData =
                dynamic_cast<tmplOnDemandPrefixTreeNodeData<itmplKey_t, itmplData_t>*>(this->Data.data());

        auto PositonIterator = PrivData->ChildPositionInStream.find(_key);
        if(PositonIterator == PrivData->ChildPositionInStream.end())
            return tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::invalidInstance();
        PrivData->InputStream.lock();
        PrivData->InputStream.seekg(*PositonIterator, std::ios_base::beg);
        auto Iterator = PrivData->Children->insert(
                    _key,
                    tmplOnDemandPrefixTreeNode<itmplKey_t, itmplData_t>(PrivData->InputStream)
                    );
        ((tmplOnDemandPrefixTreeNode&)(*Iterator)).loadBinary();
        PrivData->InputStream.unlock();
        return *Iterator;
    }
    /**
     * @brief loadBinary Loads child position of this node in the file to #ChildPositionInStream data
     * member and then reads the main data of this node.
     */

    void loadBinary(){
        tmplOnDemandPrefixTreeNodeData<itmplKey_t, itmplData_t>* PrivData =
                dynamic_cast<tmplOnDemandPrefixTreeNodeData<itmplKey_t, itmplData_t>*>(this->Data.data());

        clsIFStreamExtended& Stream = PrivData->InputStream;

        int ChildCount = Stream.read<int>();
        for(int i = 0; i < ChildCount; ++i) {
            WordIndex_t Key = Stream.read<WordIndex_t>();
            PosType_t Position = Stream.read<PosType_t>();
            PrivData->ChildPositionInStream[Key] = Position;
        }
        this->Data->DataNode.readBinary(PrivData->InputStream);
    }

    /**
     * @brief Casts #Data pointer of Abstract class to
     * QExplicitlySharedDataPointer<tmplOnDemandPrefixTreeNodeData>* and then detaches that.
     */
    virtual void detachInvalidData(){
        void* Dummy = &this->Data;
        QExplicitlySharedDataPointer<tmplOnDemandPrefixTreeNodeData<itmplKey_t, itmplData_t>>* PrivData =
                (QExplicitlySharedDataPointer<tmplOnDemandPrefixTreeNodeData<itmplKey_t, itmplData_t>>*)(Dummy);
        PrivData->detach();
    }

    /**
     * @brief createRootNode    Creates and returns root node of tmplOnMemoryPrefixTreeNode type
     *                          and loads root node data from input stream.
     * @param _inputStream      Input stream.
     * @return                  Returns root node.
     */
    inline static tmplOnDemandPrefixTreeNode<itmplKey_t, itmplData_t>* createRootNode(clsIFStreamExtended& _inputStream) {
        tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::invalidInstance();
        tmplOnDemandPrefixTreeNode<itmplKey_t, itmplData_t>* Root = new
                tmplOnDemandPrefixTreeNode<itmplKey_t, itmplData_t>(_inputStream);
        Root->Data->ref.ref();
        Root->detachInvalidData();
        Root->loadBinary();
        return Root;
    }
};


}
}
}

#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLONDEMANDPREFIXTREENODE_HPP
