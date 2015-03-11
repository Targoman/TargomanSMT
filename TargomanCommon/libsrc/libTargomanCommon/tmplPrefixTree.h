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

#ifndef TARGOMAN_COMMON_CLSPREFIXTREE_H
#define TARGOMAN_COMMON_CLSPREFIXTREE_H

#include <QExplicitlySharedDataPointer>
#include <QMap>
#include "libTargomanCommon/FStreamExtended.h"
#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/tmplExpirableCache.hpp"

namespace Targoman {
namespace Common {

// TODO Create two types for prefix tree node, one for on disk and one for memory
// TODO Make prefix tree caching expirable for on disk nodes
// TODO Binary rule table that loads completely instantly!

typedef quint32 PosType_t;

template<class Key_t, class Data_t> class tmplPrefixTreeNode;

template <class Key_t, class Data_t> class tmplPrefixTreeNodeData : public QSharedData {
public:
    tmplPrefixTreeNodeData() :
        InputStream(NULL)
    { }

    tmplPrefixTreeNodeData(const tmplPrefixTreeNodeData& _other) :
        QSharedData(_other),
        InputStream(_other.InputStream),
        DataNode(_other.DataNode),
        Children(_other.Children)
    { }

    tmplPrefixTreeNodeData(clsIFStreamExtended* _inputStream) :
        InputStream(_inputStream)
    { }

public:
    clsIFStreamExtended* InputStream;
    Data_t DataNode;

    QMap<Key_t, tmplPrefixTreeNode<Key_t,Data_t>> Children;
    QMap<Key_t, PosType_t> ChildPositionInStream;

    /*
    Common::tmplExpirableCache<
        Key_t,
        clsPrefixTreeNode,
        100000, // maximum number of cached items
        3600000 // One hour time limit before releasing a cached item
        > Children;*/

};

template <class Key_t, class Data_t> class tmplPrefixTreeNode {
public:
    tmplPrefixTreeNode() :
        Data(new tmplPrefixTreeNodeData<Key_t,Data_t>())
    { }

    tmplPrefixTreeNode(clsIFStreamExtended* _inputStream, bool _loadChildren = false):
        Data(new tmplPrefixTreeNodeData<Key_t, Data_t>(_inputStream))
    {
        int ChildCount = _inputStream->read<int>();
        for(int i = 0; i < ChildCount; ++i) {
            WordIndex_t Key = _inputStream->read<WordIndex_t>();
            PosType_t Position = _inputStream->read<PosType_t>();
            this->Data->ChildPositionInStream[Key] = Position;
        }
        this->Data->DataNode.readBinary(*static_cast<std::istream*>(this->Data->InputStream));
        if(_loadChildren) {
            for(auto Iterator = this->Data->ChildPositionInStream.begin();
                Iterator != this->Data->ChildPositionInStream.end();
                ++Iterator) {
                this->loadChildFromDisk(Iterator.key(), true);
            }
        }
    }


    tmplPrefixTreeNode(const tmplPrefixTreeNode& _other) :
        Data(_other.Data)
    { }

    void writeBinary(std::ostream& _stream) const{
        clsOFStreamExtended& Stream = static_cast<clsOFStreamExtended&>(_stream);
        PosType_t StartPosition = Stream.tellp();
        PosType_t NullPosition = 0;
        Stream.write(this->Data->Children.size());
        for(auto Iterator = this->Data->Children.begin(); Iterator != this->Data->Children.end(); ++Iterator) {
            Stream.write(Iterator.key());
            Stream.write(NullPosition);
        }
        QMap<WordIndex_t, PosType_t> ChildPositions;
        this->Data->DataNode.writeBinary(_stream);
        for(auto Iterator = this->Data->Children.begin(); Iterator != this->Data->Children.end(); ++Iterator) {
            ChildPositions[Iterator.key()] = Stream.tellp();
            Iterator->writeBinary(_stream);
        }
        PosType_t EndPosition = Stream.tellp();
        Stream.seekp(StartPosition + sizeof(int), std::ios_base::beg);
        for(auto Iterator = this->Data->Children.begin(); Iterator != this->Data->Children.end(); ++Iterator) {
            Stream.seekp(sizeof(WordIndex_t), std::ios_base::cur);
            Stream.write(ChildPositions[Iterator.key()]);
        }
        Stream.seekp(EndPosition, std::ios_base::beg);
    }

    tmplPrefixTreeNode& getChildByKey(const Key_t _key) {
        return this->Data->Children[_key];
    }

    Data_t& getData() {
        return this->Data->DataNode;
    }

    tmplPrefixTreeNode& follow(Key_t _key)
    {
        auto Iterator = this->Data->Children.find(_key);
        if(Iterator == this->Data->Children.end()) {
            if(this->Data->InputStream != NULL)
                return loadChildFromDisk(_key);
            return NULL;
        }
        return *Iterator;
    }

    tmplPrefixTreeNode loadChildFromDisk(Key_t _key, bool _loadChildren = false) {
        auto PositonIterator = this->Data->ChildPositionInStream.find(_key);
        if(PositonIterator == this->Data->ChildPositionInStream.end())
            return NULL;
        this->Data->InputStream->lock();
        this->Data->InputStream->seekg(*PositonIterator, std::ios_base::beg);
        auto Iterator = this->Data->Children.insert(
                    _key,
                    tmplPrefixTreeNode(this->Data->InputStream, _loadChildren)
                    );
        this->Data->InputStream->unlock();
        return *Iterator;
    }

private:
    QExplicitlySharedDataPointer<tmplPrefixTreeNodeData<key_t, Data_t>> Data;
};

template <class Key_t, class Data_t> class clsPrefixTree {
public:
    void readBinary(std::istream&_stream, bool _loadAll = false){
        clsIFStreamExtended* pStream = static_cast<clsIFStreamExtended*>(&_stream);
        this->RootNode = tmplPrefixTreeNode<Key_t, Data_t>(pStream, _loadAll);
    }

    void writeBinary(std::ostream& _stream) const {
        this->RootNode.writeBinary(_stream);
    }

    tmplPrefixTreeNode<Key_t, Data_t>& getOrCreateNode(std::vector<Key_t> _path) {
        tmplPrefixTreeNode<Key_t, Data_t>& Result = this->RootNode;
        for(size_t i = 0; i < _path.size(); ++i)
            Result = Result.getChildByKey(_path.at(i));
        return Result;
    }

    tmplPrefixTreeNode<Key_t, Data_t>& rootNode() {
        return this->RootNode;
    }

private:
    tmplPrefixTreeNode<key_t, Data_t> RootNode;
};

}
}
#endif // TARGOMAN_COMMON_CLSPREFIXTREE_H
