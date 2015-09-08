/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#ifndef INTFSPECIALTOKENHANDLER_HPP
#define INTFSPECIALTOKENHANDLER_HPP

#include "libTargomanCommon/tmplBoundedCache.hpp"
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
        clsExpirableSpecialToken ExpirableSpecialToken = this->SpecialTokens.value(_token);
        return ExpirableSpecialToken;
    }

    inline void insertExpirableSpecialToken(const QString& _token, const clsExpirableSpecialToken& _specialToken) {
        QMutexLocker Locker(&this->Lock);
        this->SpecialTokens.insert(_token, _specialToken);
    }

    inline Common::WordIndex_t obtainWordIndex() {
        QMutexLocker Locker(&this->Lock);
        Common::WordIndex_t WordIndex;
        if (this->AvailableWordIndexes.size())
            WordIndex = this->AvailableWordIndexes.takeFirst();
        else
            WordIndex = this->WordIndexOffset + this->SpecialTokens.keys().size();
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
        // NOTE: This mutex is not needed as the only caller of this function is the clsSpecialTokenData's
        // destructor and if you put this mutex here, it will cause DEADLOCK! Do not do it!
        //QMutexLocker Locker(&this->Lock);
        for(int i = 0; i < _wordIndexes.size(); ++i)
        {
            this->HandledSpecialTokens.remove(_wordIndexes[i]);
            if(_wordIndexes[i] >= this->WordIndexOffset) //if this word index is created by Special token handler.
                this->AvailableWordIndexes.append(_wordIndexes[i]);
        }
    }



    QHash<Common::WordIndex_t, RuleTable::clsRuleNode>                      HandledSpecialTokens;     /**< This hash, caches calculated rule nodes for each word index*/
    Common::tmplBoundedCache<QHash, QString, clsExpirableSpecialToken>      SpecialTokens;            /**< This expirable map, cashes calculated word indices and attributes for OOV words.*/
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
