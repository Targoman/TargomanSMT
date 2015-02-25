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

#include "libTargomanCommon/tmplExpirableCache.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Private/RuleTable/clsRuleNode.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace OOV{

class intfOOVHandlerModule;

TARGOMAN_ADD_EXCEPTION_HANDLER(exOOVHandler, exTargomanCore);

class OOVHandler : public Common::Configuration::intfModule
{
    class clsExpiranbleOOVWord{
    public:
        clsExpiranbleOOVWord(){
            this->WordIndex = 0;
            this->NotSet = true;
        }

        clsExpiranbleOOVWord(Common::WordIndex_t _windex, const QVariantMap& _attrs){
            this->WordIndex = _windex;
            this->NotSet = false;
            this->Attributes = _attrs;
        }

        ~clsExpiranbleOOVWord(){
            if (this->WordIndex){
               OOVHandler::instance().removeWordIndex(this->WordIndex);
            }
        }

    public:
        bool                    NotSet;
        Common::WordIndex_t     WordIndex;
        QVariantMap             Attributes;
    };

    void initialize();

public:
    static OOVHandler& instance(){return *((OOVHandler*)moduleInstance());}
    Common::WordIndex_t                getWordIndex(const QString& _token, QVariantMap &_attrs);
    inline RuleTable::clsRuleNode      getRuleNode(Common::WordIndex_t _wordIndex) const {
        return this->HandledOOVs.value(_wordIndex);
    }

private:
    OOVHandler():
        intfModule(this->moduleName())
    {}

    void removeWordIndex(Common::WordIndex_t _wordIndex);
    TARGOMAN_DEFINE_SINGLETONMODULE("OOVHandler", OOVHandler)

private:
    QHash<Common::WordIndex_t, RuleTable::clsRuleNode>            HandledOOVs;
    Common::tmplExpirableCache<QString, clsExpiranbleOOVWord>     OOVWords;
    Common::WordIndex_t                                           WordIndexOffset;
    QList<Common::WordIndex_t>                                    AvailableWordIndexes;
    QList<intfOOVHandlerModule*>                                  ActiveOOVHandlers;
    QMutex                                                        Lock;

private:
    static Targoman::Common::Configuration::tmplConfigurable<QString> OOVHandlerModules;
    static QMap<QString, intfOOVHandlerModule*>                       AvailableOOVHandlers;

    friend class clsExpiranbleOOVWord;
    friend class intfOOVHandlerModule;
};

}
}
}
}

#endif // OOVHANDLER_H
