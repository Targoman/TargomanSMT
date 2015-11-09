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
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#ifndef TARGOMAN_CORE_PRIVATE_PROXIES_LANGUAGEMODEL_CLSKENLMProxy_HPP
#define TARGOMAN_CORE_PRIVATE_PROXIES_LANGUAGEMODEL_CLSKENLMProxy_HPP

#define KENLM_MAX_ORDER 6

#include "Private/Proxies/intfLMSentenceScorer.hpp"
#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libKenLM/lm/model.hh"

namespace Targoman {
namespace SMT {
namespace Private {
namespace Proxies {
namespace LanguageModel{

/**
 * @brief This class is a proxy for using sentenceScorer class.
 */
class clsKenLMProxy : public intfLMSentenceScorer
{
private:
    class clsVocabEnumerator : public lm::EnumerateVocab {
      public:
        virtual ~clsVocabEnumerator() { }
        void Add(lm::WordIndex _index, const StringPiece &_str) {
            this->DirectVocab.insert(
                        QString::fromUtf8(_str.data(), _str.length()),
                        _index
                        );
            this->ReverseVocab.insert(
                        _index,
                        QString::fromUtf8(_str.data(), _str.length())
                        );
        }

        QHash<QString, Common::WordIndex_t> DirectVocab;
        QMap<Common::WordIndex_t, QString> ReverseVocab;
    };

public:
    explicit clsKenLMProxy(quint64 _instanceID);
    ~clsKenLMProxy();

    /**
     * @brief resets history of sentence scorer.
     * @param _withStartOfSentence Initialize with StartOfSentence or not
     */
    inline void reset(bool _withStartOfSentence){
        if(_withStartOfSentence)
            this->State = clsKenLMProxy::LM->BeginSentenceState();
        else
            this->State = clsKenLMProxy::LM->NullContextState();
    }

    /**
     * @brief Initializes language model based on configuration values (which may come from config file or argument of program).
     */

    void init(bool _justVocab){
        Q_UNUSED(_justVocab);
        TargomanLogInfo(5,"Initializing KenLM from " + clsKenLMProxy::FilePath.value());
        lm::ngram::Config Config;
        clsKenLMProxy::Vocab.DirectVocab.clear();
        clsKenLMProxy::Vocab.ReverseVocab.clear();
        Config.enumerate_vocab = &clsKenLMProxy::Vocab;
        clsKenLMProxy::LM.reset(
                    new lm::ngram::ProbingModel(
                            clsKenLMProxy::FilePath.value().toUtf8().constData(),
                            Config
                        )
                    );
        this->UnknownWordIndex = clsKenLMProxy::LM->GetVocabulary().NotFound();
    }

    /**
     * @brief returns probablity of a given word index using previous words.
     * @param _wordIndex input word index.
     * @return probablity of word index ( on the hand probablity of ngram constructed from word index and history word indices).
     */

    inline Common::LogP_t wordProb(const Common::WordIndex_t& _wordIndex) {
        lm::ngram::State Dummy = this->State;
        return clsKenLMProxy::LM->FullScore(Dummy, _wordIndex, this->State).prob;
    }

    /**
     * @brief returns probablity of whole sentence based on previous words.
     * @return probablity of sentence based on the history.
     */
    inline Common::LogP_t endOfSentenceProb(){
        lm::ngram::State Dummy = this->State;
        return clsKenLMProxy::LM->FullScore(
                                            Dummy,
                                            clsKenLMProxy::LM->GetVocabulary().EndSentence(),
                                            this->State
                                            ).prob;
    }
    /**
     * @brief gives word index of input word string.
     * @param _word input word string.
     * @return return word index.
     */

    inline Common::WordIndex_t getWordIndex(const QString& _word){
        return clsKenLMProxy::Vocab.DirectVocab[_word];
    }

    /**
     * @brief getWordIndex
     * @param _word
     * @return
     */
    inline QString getWordByIndex(Common::WordIndex_t _wordIndex){
        return clsKenLMProxy::Vocab.ReverseVocab[_wordIndex];
    }

    /**
     * @brief Initializes history of language model with history of input sentence scorer.
     * @param _oldScorer input sentence scorer.
     */
    inline void initHistory(const intfLMSentenceScorer& _oldScorer){
        this->State = dynamic_cast<const clsKenLMProxy&>(_oldScorer).State;
    }

    /**
     * @brief checks wethere our sentence scorer and input scorer have same history list or not.
     * @param _oldScorer input sentence scorer.
     */

    int compareHistoryWith(const intfLMSentenceScorer& _otherScorer) const{
        // Dynamic cast costs! a lot
//        const clsKenLMProxy& OtherScorer = dynamic_cast<const clsKenLMProxy&>(_otherScorer);
        const clsKenLMProxy& OtherScorer = (const clsKenLMProxy&)_otherScorer;
        int delta = this->State.length - OtherScorer.State.length;
        if(delta == 0)
            return std::memcmp(this->State.words, OtherScorer.State.words, sizeof(lm::WordIndex) * this->State.length);
        else if(delta > 0)
            return 1;
        else
            return -1;
    }

    void updateFutureStateHash(QCryptographicHash& _hash) const {
        _hash.addData((char*)this->State.words, sizeof(lm::WordIndex) * this->State.length);
    }

private:
    lm::ngram::State State;

    static QScopedPointer<lm::ngram::ProbingModel> LM;
    static clsVocabEnumerator Vocab;
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t> FilePath;

    TARGOMAN_DEFINE_MODULE("KenLMProxy", clsKenLMProxy);
};

}
}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_PROXIES_LANGUAGEMODEL_CLSKENLMProxy_HPP
