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

typedef qint64 PosType_t;

//TODO configure cache

template<class itmplKey_t, class itmplData_t> class tmplOnDemandPrefixTreeNode;

template <class itmplKey_t, class itmplData_t> class tmplOnDemandPrefixTreeNodeData : public QSharedData {
public:
    tmplOnDemandPrefixTreeNodeData(clsIFStreamExtended& _inputStream) :
        InputStream(_inputStream)
    { }

    /**
     * @brief This is copy constructor for this class.
     */
    tmplOnDemandPrefixTreeNodeData(const tmplOnDemandPrefixTreeNodeData& _other) :
        QSharedData(_other),
        NodeData(_other.NodeData),
        Children(_other.Children),
        InputStream(_other.InputStream),
        ChildPositionInStream(_other.ChildPositionInStream)
    { }

public:
    itmplData_t NodeData;
    tmplExpirableCache<QMap, itmplKey_t, tmplOnDemandPrefixTreeNode<itmplKey_t, itmplData_t>> Children;
    clsIFStreamExtended& InputStream;
    QMap<itmplKey_t, PosType_t> ChildPositionInStream;
};

/////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief This class is a derivation of abstract prefix tree node, which is specific for on demand
 *  working with prefix tree.
 */
template <class itmplKey_t, class itmplData_t> class tmplOnDemandPrefixTreeNode :
        public tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t> {
public:
    tmplOnDemandPrefixTreeNode(clsIFStreamExtended& _inputStream) :
        Data(new tmplOnDemandPrefixTreeNodeData<itmplKey_t, itmplData_t>(_inputStream))
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
    virtual tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& follow(itmplKey_t _key) {
        auto Iterator = this->Data->Children.find(_key);
        if(Iterator == this->Data->Children.end())
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

        auto PositonIterator = this->Data->ChildPositionInStream.find(_key);
        if(PositonIterator == this->Data->ChildPositionInStream.end())
            return tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::invalidInstance();
        this->Data->InputStream.lock();
        this->Data->InputStream.seekg(*PositonIterator, std::ios_base::beg);
        auto Iterator = this->Data->Children.insert(
                    _key,
                    tmplOnDemandPrefixTreeNode<itmplKey_t, itmplData_t>(this->Data->InputStream)
                    );
        this->Data->InputStream.unlock();
        return *Iterator;
    }


    itmplData_t& getData() {
        return this->Data->NodeData;
    }

private:
    QExplicitlySharedDataPointer<tmplOnDemandPrefixTreeNodeData<itmplKey_t, itmplData_t>> Data;
};


}
}
}

#endif // TARGOMAN_COMMON_PREFIXTREE_TMPLONDEMANDPREFIXTREENODE_HPP
