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

#include "tmplPrefixTree.h"

namespace Targoman {
namespace Common {
namespace PrefixTree {

template <class Key_t, class Data_t>
tmplPrefixTree<Key_t,Data_t>::~tmplPrefixTree()
{
    //Just to suppressCompiler Error on QScoppedPointer
}

/////////////////////////////////////////////////////////////////////////////////////
template <class Key_t, class Data_t>
tmplAbstractPrefixTreeNodeData<Key_t,Data_t>::~tmplAbstractPrefixTreeNodeData()
{
    //Just to suppressCompiler Error on QScoppedPointer
}

template <class Key_t, class Data_t>
void tmplAbstractPrefixTreeNode<Key_t, Data_t>::writeBinary(clsOFStreamExtended &_outStream) const
{
    PosType_t StartPosition = _outStream.tellp();
    PosType_t NullPosition = 0;
    _outStream.write(this->Data->Children.size());
    for(auto Iterator = this->Data->Children.begin(); Iterator != this->Data->Children.end(); ++Iterator) {
        _outStream.write(Iterator.key());
        _outStream.write(NullPosition);
    }
    QMap<WordIndex_t, PosType_t> ChildPositions;
    this->Data->DataNode.writeBinary(_outStream);
    for(auto Iterator = this->Data->Children.begin(); Iterator != this->Data->Children.end(); ++Iterator) {
        ChildPositions[Iterator.key()] = _outStream.tellp();
        Iterator->writeBinary(_outStream);
    }
    PosType_t EndPosition = _outStream.tellp();
    _outStream.seekp(StartPosition + sizeof(int), std::ios_base::beg);
    for(auto Iterator = this->Data->Children.begin(); Iterator != this->Data->Children.end(); ++Iterator) {
        _outStream.seekp(sizeof(WordIndex_t), std::ios_base::cur);
        _outStream.write(ChildPositions[Iterator.key()]);
    }
    _outStream.seekp(EndPosition, std::ios_base::beg);
}
//////////////////////////////////////////////////////////////////////////////////////////////
template <class Key_t, class Data_t>
tmplAbstractPrefixTreeNode<Key_t, Data_t> tmplOnDemandPrefixTreeNode<Key_t, Data_t>::loadChildFromDisk(
        Key_t _key, bool _loadChildren){
    auto PositonIterator = this->OnDemandData->ChildPositionInStream.find(_key);
    if(PositonIterator == this->OnDemandData->ChildPositionInStream.end())
        return NULL;
    ((tmplOnDemandPrefixTreeNodeData<Key_t, Data_t>*)this->Data)->inputStream.lock();
    ((tmplOnDemandPrefixTreeNodeData<Key_t, Data_t>*)this->Data)->inputStream.seekg(*PositonIterator, std::ios_base::beg);
    auto Iterator = this->Data->Children.insert(
                _key,
                tmplAbstractPrefixTreeNode<Key_t, Data_t>(
                    ((tmplOnDemandPrefixTreeNodeData<Key_t, Data_t>*)this->Data)->InputStream, _loadChildren)
                );
    ((tmplOnDemandPrefixTreeNodeData<Key_t, Data_t>*)this->Data)->inputStream.unlock();
    return *Iterator;
}

}
}
}
