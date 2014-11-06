/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_NLPLIBS_PRIVATE_VALUES_HPP
#define TARGOMAN_NLPLIBS_PRIVATE_VALUES_HPP

#include "libTargomanCommon/BitManager.hpp"
#include "Weights.hpp"
#include "../Definitions.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

// Template for probing unigrams and middle.
template <class clsWeights_t> class tmplGenericProbing {
public:
    explicit tmplGenericProbing(const clsWeights_t* _weights) : Weight(_weights) {}

    tmplGenericProbing() : Weights(NULL) {}

    inline bool found() const { return this->Weights !=  NULL; }

    inline float prob() const {
        this->Weights.Prob = ((quint32)this->Weights.Prob) | Targoman::Common::INT32_SIGN_BIT;
        return this->Weights.Prob;
    }

    inline float backoff() const {
        return this->Weights->Backoff;
    }

    inline bool independentLeft() const {
        return ((quint32)this->Weights.Prob) & Targoman::Common::INT32_SIGN_BIT;
    }

protected:
    const clsWeights_t *Weights;
};

// Basic template for trie unigrams.
template <class clsWeights_t> class tmplGenericTrieUnigram {
public:
    explicit tmplGenericTrieUnigram(const clsWeights_t* _weights) : Weights(_weights) {}
    tmplGenericTrieUnigram() : Weights(NULL) {}

    inline bool  found()   const { return this->Weights != NULL; }
    inline float prob()    const { return this->Weights->Prob; }
    inline float backoff() const { return this->Weights->Backoff; }
    inline float rest()    const { return this->prob(); }

protected:
    const clsWeights_t *Weights;
};

struct stuBackoffValue {
    typedef stuProbAndBackoffWeights Weights_t;
    static const enuMemoryModel::Type ProbingModel = enuMemoryModel::Probing;
    const static bool hasDifferentRest = false;

    class clsProbing : public tmplGenericProbing<Weights_t> {
    public:
        explicit clsProbing(const Weights_t* _weights) : tmplGenericProbing<Weights_t>(_weights) {}
        clsProbing() {}
        float rest() const { return this->prob(); }
    };

    class clsTrieUnigram : public tmplGenericTrieUnigram<Weights_t> {
    public:
        explicit clsTrieUnigram(const Weights_t *_weights) : tmplGenericTrieUnigram<Weights_t>(_weights) {}
        clsTrieUnigram() {}
        float rest() const { return this->prob(); }
    };

    struct stuProbingEntry {
        WordIndex_t Key;
        Weights_t   Value;
    };

    /*  template <class Model, class C> void Callback(const Config &, unsigned int, typename Model::Vocabulary &, C &callback) {
      NoRestBuild build;
      callback(build);
    }*/
};

struct stuRestValue {
    typedef stuProbBackoffAndRestWeights Weights_t;
    static const enuMemoryModel::Type ProbingModel = enuMemoryModel::RestProbing;
    const static bool hasDifferentRest = true;

    class clsProbing : public tmplGenericProbing<Weights_t> {
    public:
        explicit clsProbing(const Weights_t* _weights) : tmplGenericProbing<Weights_t>(_weights) {}
        clsProbing() {}
        float rest() const { return this->Weights.Rest; }
    };

    class clsTrieUnigram : public tmplGenericTrieUnigram<Weights_t> {
    public:
        explicit clsTrieUnigram(const Weights_t *_weights) : tmplGenericTrieUnigram<Weights_t>(_weights) {}
        clsTrieUnigram() {}
        float rest() const { return this->Weights.Rest; }
    };

    struct stuProbingEntry {
        WordIndex_t Key;
        Weights_t   Value;
    };

    /*    template <class Model, class C> void Callback(const Config &config, unsigned int order, typename Model::Vocabulary &vocab, C &callback) {
    switch (config.rest_function) {
      case Config::REST_MAX:
        {
          MaxRestBuild build;
          callback(build);
        }
        break;
      case Config::REST_LOWER:
        {
          LowerRestBuild<Model> build(config, order, vocab);
          callback(build);
        }
        break;
    }
  }
*/
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_VALUES_HPP
