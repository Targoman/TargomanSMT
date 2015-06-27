#ifndef INTFSPECIALTOKENHANDLER_HPP
#define INTFSPECIALTOKENHANDLER_HPP

#include "libTargomanCommon/tmplExpirableCache.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Private/RuleTable/clsRuleNode.h"
#include "Private/GlobalConfigs.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {


class SpecialTokensRegistry
{

public:
    class clsExpirableSpecialTokenData : public QSharedData {
    public:
        clsExpirableSpecialTokenData() :
            NotSet(true)
        { }

        clsExpirableSpecialTokenData(const clsExpirableSpecialTokenData& _other) :
            QSharedData(_other),
            NotSet(_other.NotSet),
            WordIndexes(_other.WordIndexes),
            Attributes(_other.Attributes)
        { }

        clsExpirableSpecialTokenData(bool _notSet, QList<Common::WordIndex_t> _wordIndexes, QVariantMap _attributes) :
            NotSet(_notSet),
            WordIndexes(_wordIndexes),
            Attributes(_attributes)
        { }

        clsExpirableSpecialTokenData(bool _notSet, Common::WordIndex_t _wordIndex, QVariantMap _attributes) :
            NotSet(_notSet),
            WordIndexes(QList<Common::WordIndex_t>() << _wordIndex),
            Attributes(_attributes)
        { }

        ~clsExpirableSpecialTokenData() {
            if (this->WordIndexes.size()){
               SpecialTokensRegistry::instance().removeWordIndexes(this->WordIndexes);
            }
        }

    public:
        bool                            NotSet;
        QList<Common::WordIndex_t>      WordIndexes;
        QVariantMap                     Attributes;
    };

    class clsExpirableSpecialToken{
    public:
        clsExpirableSpecialToken() : Data(new clsExpirableSpecialTokenData())
        { }

        clsExpirableSpecialToken(const clsExpirableSpecialToken& _other) :
            Data(_other.Data)
        { }

        clsExpirableSpecialToken(Common::WordIndex_t _windex, const QVariantMap& _attrs) :
            Data(new clsExpirableSpecialTokenData(false, _windex, _attrs))
        { }

        clsExpirableSpecialToken(QList<Common::WordIndex_t> _windexes, const QVariantMap& _attrs) :
            Data(new clsExpirableSpecialTokenData(false, _windexes, _attrs))
        { }

        ~clsExpirableSpecialToken(){
        }

    public:
        QExplicitlySharedDataPointer<clsExpirableSpecialTokenData> Data;
    };


    inline RuleTable::clsRuleNode      getRuleNode(Common::WordIndex_t _wordIndex) const {
        return this->HandledSpecialTokens.value(_wordIndex);
    }

    static SpecialTokensRegistry& instance() {
       static SpecialTokensRegistry* Instance = NULL;
       return *(Q_LIKELY(Instance) ? Instance :( Instance = new SpecialTokensRegistry));
    }

    inline clsExpirableSpecialToken getExpirableSpecialToken(const QString& _token) {
        QMutexLocker Locker(&this->Lock);
        clsExpirableSpecialToken ExpirableSpecialToken = this->specialTokens.value(_token);
        return ExpirableSpecialToken;
    }

    inline void insertExpirableSpecialToken(const QString& _token, const clsExpirableSpecialToken& _specialToken) {
        QMutexLocker Locker(&this->Lock);
        this->specialTokens.insert(_token, _specialToken);
    }

    inline Common::WordIndex_t obtainWordIndex() {
        QMutexLocker Locker(&this->Lock);
        Common::WordIndex_t WordIndex;
        if (this->AvailableWordIndexes.size())
            WordIndex = this->AvailableWordIndexes.takeFirst();
        else
            WordIndex = this->WordIndexOffset + this->specialTokens.keys().size();
        Locker.unlock();
        return WordIndex;
    }

    inline void insertRuleNode(Common::WordIndex_t _wordIndex,
                                              const RuleTable::clsRuleNode& _ruleNode) {
        QMutexLocker Locker(&this->Lock);
        this->HandledSpecialTokens.insert(_wordIndex, _ruleNode);
    }

private:
    SpecialTokensRegistry(){
        this->WordIndexOffset = gConfigs.SourceVocab.size() + 1;
    }


    /**
     * @brief OOVHandler::removeWordIndex When an special token exipres this function  will be called to removes that word index from #HandledSpecialTokens and add that word index to #AvailableWordIndexes.
     * @param _wordIndex input word index.
     */
    inline void removeWordIndexes(QList<Common::WordIndex_t> _wordIndexes) {
        QMutexLocker Locker(&this->Lock);
        for(int i = 0; i < _wordIndexes.size(); ++i)
        {
            this->HandledSpecialTokens.remove(_wordIndexes[i]);
            if(_wordIndexes[i] >= this->WordIndexOffset) //if this word index is created by Special token handler.
                this->AvailableWordIndexes.append(_wordIndexes[i]);
        }
    }



    QHash<Common::WordIndex_t, RuleTable::clsRuleNode>                      HandledSpecialTokens;     /**< This hash, caches calculated rule nodes for each word index*/
    Common::tmplExpirableCache<QHash, QString, clsExpirableSpecialToken>    specialTokens;            /**< This expirable map, cashes calculated word indices and attributes for OOV words.*/
    Common::WordIndex_t                                                     WordIndexOffset;          /**< OOV word indices should be start from this number which is size of source vocab */
    QList<Common::WordIndex_t>                                              AvailableWordIndexes;     /**< #OOVWords removes not recently seen chashed OOVs, indices of those deleted OOVs will be stored in this variable to reuse them.*/
    QMutex                                                                  Lock;                     /**< Used to prevent simultaneous access of threads to some part of code. */


    friend class clsExpirableSpecialToken;

};

}
}
}
}



#endif // INTFSPECIALTOKENHANDLER_HPP
