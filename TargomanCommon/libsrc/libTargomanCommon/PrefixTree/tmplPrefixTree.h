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
    typedef tmplAbstractPrefixTreeNode<Key_t, Data_t> Node_t;
public:
    tmplPrefixTree() :
        RootNode(new tmplOnMemoryPrefixTreeNode<Key_t, Data_t>)
    { }
    ~tmplPrefixTree(){
        //Just to suppress compiler Error on QScoppedPointer
    }

    void readBinary(clsIFStreamExtended& _file, bool _loadAll = false){
        if (_loadAll)
            this->RootNode.reset(new tmplOnMemoryPrefixTreeNode<Key_t, Data_t>(_file));
        else
            this->RootNode.reset(new tmplOnDemandPrefixTreeNode<Key_t, Data_t>(_file));
    }

    inline void writeBinary(Common::clsOFStreamExtended& _stream) const {
        this->RootNode->writeBinary(_stream);
    }

    inline tmplAbstractPrefixTreeNode<Key_t, Data_t>& getOrCreateNode(const QList<Key_t>& _path) {
        tmplAbstractPrefixTreeNode<Key_t, Data_t>& Result = *this->RootNode;
        foreach(Key_t Key, _path)
            Result = Result.getChildByKey(Key, true);
        return Result;
    }

    inline tmplAbstractPrefixTreeNode<Key_t, Data_t>& rootNode() {
        return *this->RootNode;
    }

private:
    QScopedPointer<tmplAbstractPrefixTreeNode<Key_t, Data_t>> RootNode;
};

}
}
}
#endif // TARGOMAN_COMMON_PREFIXTREE_CLSPREFIXTREE_H
