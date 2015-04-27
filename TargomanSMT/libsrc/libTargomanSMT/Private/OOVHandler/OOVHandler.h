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
#include "Private/GlobalConfigs.h"
#include "Private/Proxies/intfLMSentenceScorer.hpp"
#include "libTargomanCommon/Constants.h"
namespace Targoman{
namespace SMT {
namespace Private{
/**
 *  @brief Namespace surrounding all of the classes related to Out Of Vocabulary handling
 */
namespace OOV{

class intfOOVHandlerModule;

TARGOMAN_ADD_EXCEPTION_HANDLER(exOOVHandler, exTargomanCore);

/**
 * @brief The OOVHandler class is responsible for associating word index, string, caching and making rule node for OOV words.
 */
class OOVHandler : public Common::Configuration::intfModule
{
    class clsExpirableOOVWordData : public QSharedData {
    public:
        clsExpirableOOVWordData() :
            NotSet(true),
            WordIndex(Common::Constants::SrcVocabUnkWordIndex)
        { }

        clsExpirableOOVWordData(const clsExpirableOOVWordData& _other) :
            QSharedData(_other),
            NotSet(_other.NotSet),
            WordIndex(_other.WordIndex),
            Attributes(_other.Attributes)
        { }

        clsExpirableOOVWordData(bool _notSet, Common::WordIndex_t _wordIndex, QVariantMap _attributes) :
            NotSet(_notSet),
            WordIndex(_wordIndex),
            Attributes(_attributes)
        { }

        ~clsExpirableOOVWordData() {
            if (this->WordIndex){
               OOVHandler::instance().removeWordIndex(this->WordIndex);
            }
        }

    public:
        bool                    NotSet;
        Common::WordIndex_t     WordIndex;
        QVariantMap             Attributes;
    };

    class clsExpirableOOVWord{
    public:
        clsExpirableOOVWord() : Data(new clsExpirableOOVWordData())
        { }

        clsExpirableOOVWord(const clsExpirableOOVWord& _other) :
            Data(_other.Data)
        { }

        clsExpirableOOVWord(Common::WordIndex_t _windex, const QVariantMap& _attrs) :
            Data(new clsExpirableOOVWordData(false, _windex, _attrs))
        { }

        ~clsExpirableOOVWord(){
        }

    public:
        QExplicitlySharedDataPointer<clsExpirableOOVWordData> Data;
    };


public:
    void initialize();
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
    TARGOMAN_DEFINE_SINGLETONMODULE("OOVHandler", OOVHandler);

private:
    QHash<Common::WordIndex_t, RuleTable::clsRuleNode>              HandledOOVs;              /**< This hash, caches calculated rule nodes for each word index*/
    Common::tmplExpirableCache<QHash, QString, clsExpirableOOVWord> OOVWords;                 /**< This expirable map, cashes calculated word indices and attributes for OOV words.*/
    Common::WordIndex_t                                             WordIndexOffset;          /**< OOV word indices should be start from this number which is size of source vocab */
    QList<Common::WordIndex_t>                                      AvailableWordIndexes;     /**< #OOVWords removes not recently seen chashed OOVs, indices of those deleted OOVs will be stored in this variable to reuse them.*/
    QList<intfOOVHandlerModule*>                                    ActiveOOVHandlers;        /**< List of active special OOV handlers.*/
    QMutex                                                          Lock;                     /**< Used to prevent simultaneous access of threads to some part of code. */

private:
    static Targoman::Common::Configuration::tmplConfigurable<QString> OOVHandlerModules;
    static QMap<QString, intfOOVHandlerModule*>                       AvailableOOVHandlers; /**< List of available special OOV handlers*/

    friend class clsExpirableOOVWord;
    friend class intfOOVHandlerModule;
};

}
}
}
}

#endif // OOVHANDLER_H
