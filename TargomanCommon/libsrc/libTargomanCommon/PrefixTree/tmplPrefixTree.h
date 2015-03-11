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

namespace Targoman {
namespace Common {
namespace PrefixTree {

template <class Key_t, class Data_t> class tmplPrefixTree {
public:
    tmplPrefixTree() :
        RootNode(new tmplOnMemoryPrefixTreeNode<Key_t, Data_t>)
    {}
    ~tmplPrefixTree();

    void readBinary(const QString& _filePath, bool _loadAll = false){
        clsIFStreamExtended File(_filePath);
        if (File.is_open() == false)
            throw exPrefixTree("Unable to open "+ _filePath);

        if (_loadAll)
            this->RootNode.reset(tmplOnMemoryPrefixTreeNode<Key_t, Data_t>(File));
        else
            this->RootNode.reset(tmplOnDemandPrefixTreeNode<Key_t, Data_t>(File));
    }

    void writeBinary(std::ostream& _stream) const {
        this->RootNode->writeBinary(_stream);
    }

    tmplAbstractPrefixTreeNode<Key_t, Data_t>& getOrCreateNode(std::vector<Key_t> _path) {
        tmplAbstractPrefixTreeNode<Key_t, Data_t>& Result = *this->RootNode;
        for(size_t i = 0; i < _path.size(); ++i)
            Result = Result.getChildByKey(_path.at(i));
        return *Result;
    }

    tmplAbstractPrefixTreeNode<Key_t, Data_t>& rootNode() {
        return *this->RootNode;
    }

private:
    QScopedPointer<tmplAbstractPrefixTreeNode<key_t, Data_t>> RootNode;
};

}
}
}
#endif // TARGOMAN_COMMON_PREFIXTREE_CLSPREFIXTREE_H
