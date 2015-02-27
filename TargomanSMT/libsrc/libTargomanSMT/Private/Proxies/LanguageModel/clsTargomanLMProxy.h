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

#ifndef TARGOMAN_CORE_PRIVATE_PROXIES_LANGUAGEMODEL_CLSTARGOMANLMPROXY_HPP
#define TARGOMAN_CORE_PRIVATE_PROXIES_LANGUAGEMODEL_CLSTARGOMANLMPROXY_HPP

#include "Private/Proxies/intfLMSentenceScorer.hpp"
#include "libTargomanLM/clsLMSentenceScorer.h"
#include "libTargomanLM/clsLanguageModel.h"
#include "libTargomanCommon/Configuration/intfConfigurable.hpp"

namespace Targoman {
namespace Core {
namespace Private {
namespace Proxies {
namespace LanguageModel{

/**
 * @brief This class is a proxy for using sentenceScorer class.
 */

class clsTargomanLMProxy : public intfLMSentenceScorer
{
public:
    clsTargomanLMProxy(quint64 _instanceID);
    ~clsTargomanLMProxy();

    /**
     * @brief resets history of sentence scorer.
     * @param _withStartOfSentence Initialize with StartOfSentence or not
     */
    inline void reset(bool _withStartOfSentence){
        this->LMSentenceScorer->reset(_withStartOfSentence);
    }

    /**
     * @brief Initializes language model based on configuration values (which may come from config file or argument of program).
     */

    void init(bool _justVocab){
        clsTargomanLMProxy::LM.init(_justVocab); // LM must check if it needs to be initialized
    }

    /**
     * @brief returns probablity of a given word index using previous words.
     * @param _wordIndex input word index.
     * @return probablity of word index ( on the hand probablity of ngram constructed from word index and history word indices).
     */

    inline Common::LogP_t wordProb(const Common::WordIndex_t& _wordIndex) {
        quint8 Dummy;
        return this->LMSentenceScorer->wordProb(_wordIndex, Dummy);
    }

    /**
     * @brief returns probablity of whole sentence based on previous words.
     * @return probablity of sentence based on the history.
     */
    inline Common::LogP_t endOfSentenceProb(){
        quint8 Dummy;
        return this->LMSentenceScorer->endOfSentenceProb(Dummy);
    }
    /**
     * @brief gives word index of input word string.
     * @param _word input word string.
     * @return return word index.
     */

    inline Common::WordIndex_t getWordIndex(const QString& _word){return this->LM.getID(_word);}

    /**
     * @brief getWordIndex
     * @param _word
     * @return
     */
    inline QString getWordByIndex(Common::WordIndex_t _wordIndex){return this->LM.getWordByID(_wordIndex);}

    /**
     * @brief Initializes history of language model with history of input sentence scorer.
     * @param _oldScorer input sentence scorer.
     */
    inline void initHistory(const intfLMSentenceScorer& _oldScorer){
        this->LMSentenceScorer->initHistory(*(dynamic_cast<const clsTargomanLMProxy&>(_oldScorer).LMSentenceScorer));
    }

    /**
     * @brief checks wethere our sentence scorer and input scorer have same history list or not.
     * @param _oldScorer input sentence scorer.
     */

    bool haveSameHistoryAs(const intfLMSentenceScorer& _otherScorer) const{
        return this->LMSentenceScorer->haveSameHistoryAs(
                    *(dynamic_cast<const clsTargomanLMProxy&>(_otherScorer).LMSentenceScorer));
    }


private:
    static Targoman::NLPLibs::TargomanLM::clsLanguageModel LM;                  /** < static data member of clsLanguageModel. */

    QScopedPointer<Targoman::NLPLibs::TargomanLM::clsLMSentenceScorer> LMSentenceScorer;    /** < clsLMSentenceScorer data pointer. */
    TARGOMAN_DEFINE_MODULE("TargomanLMProxy", clsTargomanLMProxy)
};

}
}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_PROXIES_LANGUAGEMODEL_CLSTARGOMANLMPROXY_HPP
