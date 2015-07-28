/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#ifndef TARGOMAN_COMMON_PREFIXTREE_TMPLONMEMORYPREFIXTREENODE_H
#define TARGOMAN_COMMON_PREFIXTREE_TMPLONMEMORYPREFIXTREENODE_H

#include "libTargomanCommon/PrefixTree/tmplAbstractPrefixTreeNode.hpp"
#include <QDataStream>

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
    QMap<itmplKey_t, QExplicitlySharedDataPointer<tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>>> Children;
};

/////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This class is a derivation of abstract prefix tree node, which is specific for on memory
 * working with prefix tree.
 */
template <class itmplKey_t, class itmplData_t> class tmplOnMemoryPrefixTreeNode :
        public tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t> {
public:
    typedef typename tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::Node_t Node_t;
    typedef typename tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::pNode_t pNode_t;

public:
    tmplOnMemoryPrefixTreeNode() :
        Data(new tmplOnMemoryPrefixTreeNodeData<itmplKey_t, itmplData_t>()) {
        this->IsInvalid = false;
    }

    tmplOnMemoryPrefixTreeNode(clsIFStreamExtended& _inputStream) :
        Data(new tmplOnMemoryPrefixTreeNodeData<itmplKey_t, itmplData_t>())
    {
        QMap<itmplKey_t, PosType_t> ChildPositions;
        int ChildCount = _inputStream.read<int>();
        for(int i = 0; i < ChildCount; ++i) {
            itmplKey_t Key = _inputStream.read<itmplKey_t>();
            PosType_t Position = _inputStream.read<PosType_t>();
            ChildPositions[Key] = Position;
        }
        this->Data->NodeData.readBinary(_inputStream);
        for(auto ChildPosIter = ChildPositions.begin(); ChildPosIter != ChildPositions.end(); ++ChildPosIter)
            this->Data->Children[ChildPosIter.key()] =
                    new tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>(_inputStream);
        this->IsInvalid = false;
    }

    virtual pNode_t follow(itmplKey_t _key) {
        auto Iter = this->Data->Children.find(_key);
        if(Iter == this->Data->Children.end())
            return pNode_t(&tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>::invalidInstance());
        else
            return *Iter;
    }

    virtual pNode_t getOrCreateChildByKey(const itmplKey_t _key) {
        QExplicitlySharedDataPointer<
                tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>>& Result = this->Data->Children[_key];
        if(Result.data() == NULL)
            Result = new tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>();
        return Result;    }

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
            (*Iterator)->writeBinary(_outStream);
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

    itmplData_t& getData() {
        return this->Data->NodeData;
    }

private:
    QExplicitlySharedDataPointer<tmplOnMemoryPrefixTreeNodeData<itmplKey_t, itmplData_t>> Data;
};

}
}
}

#endif // TMPLONMEMORYPREFIXTREENODE_H
