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

#ifndef OOVHANDLER_H
#define OOVHANDLER_H

#include "Private/RuleTable/clsRuleNode.h"

namespace Targoman{
namespace Core {
namespace Private{

class OOVHandler
{

    struct stuOOVItem{
        RuleTable::clsRuleNode RuleNode;
        quint64                Usages;
    };

public:
    static OOVHandler& instance(){return Q_LIKELY(Instance) ? *Instance : *(Instance = new OOVHandler);}
    //RuleTable::clsRuleNode& getOrCreateNode(const QString& _vo)

private:
    OOVHandler();
    Q_DISABLE_COPY(OOVHandler)

private:
    QHash<QString, stuOOVItem>              HandledOOVs;
    QMap<Common::WordIndex_t,QString>       OOVocab;
    Common::WordIndex_t                     WordIndexOffset;
    QList<Common::WordIndex_t>              AvailableWordIndexes;
    static OOVHandler* Instance;
};

}
}
}

#endif // OOVHANDLER_H
