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
#include <QFile>
#include "../Definitions.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

typedef quint64 Hash_t;

typedef std::vector<std::string> NGram_t;

}
}
}

//Overloading QHash hash methods in order to support NGram_t
//uint qHash(const Targoman::NLPLibs::Private::NGram_t & _ngram, _gram);

namespace Targoman {
namespace NLPLibs {
namespace Private {

/**********************************************************************************/

static const int HashKeys[]={29,71,113,173,229,31,73,127,179,37,79,131,181,41,83,137};
/*http://planetmath.org/goodhashtableprimes*/
static const quint32 PrimeNumbers[]={
    53,
    97,
    193,
    389,
    769,
    1543,
    3079,
    6151,
    12289,
    24593,
    49157,
    98317,
    196613,
    393241,
    786433,
    1572869,
    3145739,
    6291469,
    12582917,
    25165843
};

class TargomanHasher
{
public:



public:
    static Hash_t hash(const void* _buff, size_t _len, int _level){
        static const Hash_t Const1 = 0xcc9e2d51;
        static const Hash_t Const2 = 0x1b873593;
        static const Hash_t Round1 = 15;
        static const Hash_t Round2 = 13;
        static const Hash_t M = 5;
        static const Hash_t Const3 = 0xe6546b64;

        Hash_t Hash = HashKeys[_level];

        const int nblocks = _len / 4;
        const quint32 *blocks = (const quint32 *) _buff;
        int i;
        for (i = 0; i < nblocks; i++) {
            Hash_t Buffer = blocks[i];
            Buffer *= Const1;
            Buffer = (Buffer << Round1) | (Buffer >> (32 - Round1));
            Buffer *= Const2;

            Hash ^= Buffer;
            Hash = ((Hash << Round2) | (Hash >> (32 - Round2))) * M + Const3;
        }

        const uint8_t *tail = (const uint8_t *) (_buff + nblocks * 4);
        Hash_t Buffer = 0;

        switch (_len & 3) {
        case 3:
            Buffer ^= tail[2] << 16;
        case 2:
            Buffer ^= tail[1] << 8;
        case 1:
            Buffer ^= tail[0];

            Buffer *= Const1;
            Buffer = (Buffer << Round1) | (Buffer >> (32 - Round1));
            Buffer *= Const2;
            Hash ^= Buffer;
        }

        Hash ^= _len;
        Hash ^= (Hash >> 16);
        Hash *= 0x85ebca6b;
        Hash ^= (Hash >> 13);
        Hash *= 0xc2b2ae35;
        Hash ^= (Hash >> 16);

        return Hash;
    }

    static inline Hash_t hashVal(const void* _buff, size_t _len, int _level)
    {return hash(_buff, _len, _level) + 1;}
    static inline Hash_t hashLoc(const void* _buff, size_t _len, int _level, Hash_t _maxtable)
    {return (hash(_buff, _len, _level) % _maxtable) + 1;}
};

/**********************************************************************************/


TARGOMAN_ADD_EXCEPTION_HANDLER(exNgramManager, exLanguageModel);

// see benchmarks http://attractivechaos.wordpress.com/2008/10/07/another-look-at-my-old-benchmark/
// and http://attractivechaos.wordpress.com/2008/08/28/comparison-of-hash-table-libraries/
template <class Weights_t> class tmplNGramHashTable
{
private:

public:
 /*   inline void insert(NGram_t _ngram, Weights_t _weights){
  //      this->HashTable.insert(_ngram, _weights);
    }

    inline Weights_t value(NGram_t _ngram) const{
//        return this->HashTable.value(_ngram);
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
   //  HashTable;*/

};

}
}
}


#endif // TARGOMAN_NLPLIBS_PRIVATE_CLSNGRAM_H
