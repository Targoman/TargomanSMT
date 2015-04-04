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

template <class itmplKey_t, class itmplData_t> class tmplPrefixTree {
public:
    typedef tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t> Node_t;
public:
    tmplPrefixTree() :
        RootNode(new tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>)
    { }
    ~tmplPrefixTree(){
        //Just to suppress compiler Error on QScoppedPointer
    }

    void readBinary(clsIFStreamExtended& _file, bool _loadAll = false){
        if (_loadAll)
            this->RootNode.reset(new tmplOnMemoryPrefixTreeNode<itmplKey_t, itmplData_t>(_file));
        else
            this->RootNode.reset(new tmplOnDemandPrefixTreeNode<itmplKey_t, itmplData_t>(_file));
    }

    inline void writeBinary(Common::clsOFStreamExtended& _stream) const {
        this->RootNode->writeBinary(_stream);
    }

    inline tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& getOrCreateNode(const QList<itmplKey_t>& _path) {
        tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& Result = *this->RootNode;
        foreach(itmplKey_t Key, _path)
            Result = Result.getChildByKey(Key, true);
        return Result;
    }

    inline tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>& rootNode() {
        return *this->RootNode;
    }

private:
    QScopedPointer<tmplAbstractPrefixTreeNode<itmplKey_t, itmplData_t>> RootNode;
};

}
}
}
#endif // TARGOMAN_COMMON_PREFIXTREE_CLSPREFIXTREE_H
