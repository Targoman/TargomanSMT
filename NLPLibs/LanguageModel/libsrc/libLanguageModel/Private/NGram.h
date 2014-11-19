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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_CLSNGRAM_H
#define TARGOMAN_NLPLIBS_PRIVATE_CLSNGRAM_H
#include <QHash>
#include <QFile>
#include "../Definitions.h"

#ifdef LM_USE_GOOGLE_SPARSE_HASH
#include <google/sparse_hash_map>
#endif

namespace Targoman {
namespace NLPLibs {
namespace Private {

typedef QVector<Targoman::Common::WordIndex_t> NGram_t;

}
}
}

//Overloading QHash hash methods in order to support NGram_t
uint qHash(const Targoman::NLPLibs::Private::NGram_t & _ngram);

namespace Targoman {
namespace NLPLibs {
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exNgramManager, exLanguageModel);

// see benchmarks http://attractivechaos.wordpress.com/2008/10/07/another-look-at-my-old-benchmark/
// and http://attractivechaos.wordpress.com/2008/08/28/comparison-of-hash-table-libraries/
template <class Weights_t> class tmplNGramHashTable
{
private:
#ifdef LM_USE_GOOGLE_SPARSE_HASH
    class clsHasher{
    public :
        uint operator()(const NGram_t& _key) const{
            return qHash(_key);
        }
    };
/*    class clsValueSerializer{
    public:

    };*/

    typedef google::sparse_hash_map<NGram_t, Weights_t, clsHasher> GSHM_t;
#endif

public:
    inline void insert(NGram_t _ngram, Weights_t _weights){
#ifdef LM_USE_GOOGLE_SPARSE_HASH
        this->HashTable[_ngram] = _weights;
#else
        this->HashTable.insert(_ngram, _weights);
#endif
    }

    inline Weights_t value(NGram_t _ngram) const{
#ifdef LM_USE_GOOGLE_SPARSE_HASH
        typename GSHM_t::const_iterator Iter = this->HashTable.find(_ngram);
        if (Iter != this->HashTable.end())
            return Iter->second;
        else
            return Weights_t();
#else
        return this->HashTable.value(_ngram);
#endif
    }

    inline void writeBinary(const QString& _filePath){
        QFile File(_filePath);
        if (!File.open(QFile::WriteOnly))
            throw exNgramManager("Unable to open file: <" + _filePath + "> for WRITING");
#ifdef LM_USE_GOOGLE_SPARSE_HASH
        //this->HashTable.serialize()
#else

#endif
    }

    inline void loadBinary(const QString& _filePath){
        QFile File(_filePath);
        if (!File.open(QFile::ReadOnly))
            throw exNgramManager("Unable to open file: <" + _filePath + "> for READING");

    }

private:
#ifdef LM_USE_GOOGLE_SPARSE_HASH
    GSHM_t HashTable;
#else
    QHash<NGram_t, Weights_t> HashTable;
#endif
};

}
}
}


#endif // TARGOMAN_NLPLIBS_PRIVATE_CLSNGRAM_H
