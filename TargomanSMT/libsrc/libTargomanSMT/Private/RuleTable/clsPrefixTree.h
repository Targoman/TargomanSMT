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
 */

#ifndef TARGOMAN_CORE_PRIVATE_RULETABLE_CLSPREFIXTREE_H
#define TARGOMAN_CORE_PRIVATE_RULETABLE_CLSPREFIXTREE_H

#include <QExplicitlySharedDataPointer>
#include <QMap>
#include "libTargomanCommon/Types.h"
#include "Private/RuleTable/clsRuleNode.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace RuleTable {

typedef long PosType_t;

class clsPrefixTreeNode;

class clsPrefixTreeNodeData : public QSharedData {
public:
    clsPrefixTreeNodeData() :
        InputStream(NULL)
    { }

    clsPrefixTreeNodeData(const clsPrefixTreeNodeData& _other) :
        QSharedData(_other),
        InputStream(_other.InputStream),
        RuleNode(_other.RuleNode),
        Children(_other.Children)
    { }

    clsPrefixTreeNodeData(clsIFStreamExtended* _inputStream) :
        InputStream(_inputStream)
    { }

public:
    clsIFStreamExtended* InputStream;
    clsRuleNode RuleNode;
    QMap<Common::WordIndex_t, clsPrefixTreeNode> Children;
    QMap<Common::WordIndex_t, PosType_t> ChildPositionInStream;
};

class clsPrefixTreeNode {
public:
    clsPrefixTreeNode() :
        Data(new clsPrefixTreeNodeData())
    { }

    clsPrefixTreeNode(clsIFStreamExtended* _inputStream);

    clsPrefixTreeNode(const clsPrefixTreeNode& _other) :
        Data(_other.Data)
    { }

    void writeBinary(std::ostream& _stream) const;

    clsPrefixTreeNode& getChildByKey(const Common::WordIndex_t _key)
    {
        return this->Data->Children[_key];
    }

    clsRuleNode& getData()
    {
        return this->Data->RuleNode;
    }

    clsPrefixTreeNode* follow(Common::WordIndex_t _key)
    {
        auto Iterator = this->Data->Children.find(_key);
        if(Iterator == this->Data->Children.end()) {
            if(this->Data->InputStream != NULL) {
                auto PositonIterator = this->Data->ChildPositionInStream.find(_key);
                if(PositonIterator == this->Data->ChildPositionInStream.end())
                    return NULL;
                this->Data->InputStream->seekg(*PositonIterator);
                Iterator = this->Data->Children.insert(
                            _key,
                            clsPrefixTreeNode(this->Data->InputStream)
                            );
            } else
                return NULL;
        }
        return &(*Iterator);
    }

private:
    QExplicitlySharedDataPointer<clsPrefixTreeNodeData> Data;
};

class clsPrefixTree {
public:
    typedef clsPrefixTreeNode Node;

    void readBinary(std::istream&_stream);
    void writeBinary(std::ostream& _stream) const {
        this->RootNode.writeBinary(_stream);
    }

    clsPrefixTreeNode* getOrCreateNode(std::vector<Common::WordIndex_t> _path)
    {
        clsPrefixTreeNode* Result = &this->RootNode;
        for(size_t i = 0; i < _path.size(); ++i)
            Result = &Result->getChildByKey(_path.at(i));
        return Result;
    }

    clsPrefixTreeNode* rootNode()
    {
        return &this->RootNode;
    }

private:
    clsPrefixTreeNode RootNode;
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_RULETABLE_CLSPREFIXTREE_H
