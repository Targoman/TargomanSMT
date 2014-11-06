/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 * MurmurHash function was derived from Murmurhash3 by Austin Appleby.
 * See more at: http://code.google.com/p/smhasher/
 * MurmurHash3 is released to the public domain.
 * The author hereby disclaims copyright to MurmurHash3 source code.
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include "NGram.h"

#include "libTargomanCommon/FastOperations.hpp"

using namespace Targoman::Common;

namespace Targoman {
namespace NLPLibs {
namespace Private {

const uint PrimeNumbers[] = { 13, 5, 53, 17, 25, 19, 31, 37, 41, 43, 19, 53, 11, 3, 7 };


}
}
}

uint qHash(const Targoman::NLPLibs::Private::NGram_t &_ngram)
{
    uint Hash = 0;
#ifdef LM_USE_MURMUR_HASH
    //Following code is derived from MurmurHash3.cpp
    //Original code: http://code.google.com/p/smhasher/source/browse/trunk/MurmurHash3.cpp

    Hash = 17; //17 is chosen as seed

    //----------
    // body

    foreach(WordIndex_t Key, _ngram)
    {
        Key *= 0xcc9e2d51;
        Key =  Targoman::Common::rotl32(Key,15);
        Key *= 0x1b873593;

        Hash ^= Key;
        Hash = Targoman::Common::rotl32(Hash,13);
        Hash = Hash*5+0xe6546b64;
    }

    //----------
    // finalization

    Hash ^= _ngram.length();

    //force all bits of a hash block to avalanche
    Hash ^= Hash >> 16;
    Hash *= 0x85ebca6b;
    Hash ^= Hash >> 13;
    Hash *= 0xc2b2ae35;
    Hash ^= Hash >> 16;
#else
    for (int i=0; i<_key.size(); i++)
        Hash+=_key.at(i) * Targoman::NLPLibs::Private::PrimeNumbers[i%15];
#endif
    return Hash;
}



