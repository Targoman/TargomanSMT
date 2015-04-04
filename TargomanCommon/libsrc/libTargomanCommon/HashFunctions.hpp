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
 */


#ifndef TARGOMAN_COMMON_HASHFUNCTIONS_H
#define TARGOMAN_COMMON_HASHFUNCTIONS_H

#include <QtCore>
#include "libTargomanCommon/Types.h"

namespace Targoman {
namespace Common {

extern const int TargomanHashKeys[];
extern const int TargomanHashKeysCount;
extern const quint32 TargomanGoodHashTableSizes[];
extern const quint16 TargomanGoodHashTableSizesCount;

class HashFunctions
{
public:


    // MurmurHash3, 64-bit versions, by Austin Appleby

    // The same caveats as 32-bit MurmurHash2 apply here - beware of alignment
    // and endian-ness issues if used across multiple platforms.
    // 64-bit hash for 64-bit platforms

    static quint64 murmurHash64(const void* _buff, size_t _len, int _level = 0){
        const quint64 Constant1 = 0xc6a4a7935bd1e995LLU;
        const int Remain1 = 47;

        quint64 Hash = TargomanHashKeys[_level % TargomanHashKeysCount] ^ ((_len + _level) * Constant1);

        const quint64 * Data = (const quint64 *)_buff;
        const quint64 * End = (_len >> 3) + Data;

        while(Data != End)
        {
          quint64 K = *Data++;

          K *= Constant1;
          K ^= K >> Remain1;
          K *= Constant1;

          Hash ^= K;
          Hash *= Constant1;
        }

        const unsigned char * Data2 = (const unsigned char *)Data;

        switch(_len & 7)
        {
        case 7: Hash ^= (quint64)(Data2[6]) << 48;
        case 6: Hash ^= (quint64)(Data2[5]) << 40;
        case 5: Hash ^= (quint64)(Data2[4]) << 32;
        case 4: Hash ^= (quint64)(Data2[3]) << 24;
        case 3: Hash ^= (quint64)(Data2[2]) << 16;
        case 2: Hash ^= (quint64)(Data2[1]) << 8;
        case 1: Hash ^= (quint64)(Data2[0]);
                Hash *= Constant1;
        };

        Hash ^= Hash >> Remain1;
        Hash *= Constant1;
        Hash ^= Hash >> Remain1;

        return Hash;
    }

    //Modified version of MurmurHash3 to work on list of integers
    static quint64 murmurHash64(const QList<Common::WordIndex_t>& _data, int _level = 0){
        const quint64 Constant1 = 0xc6a4a7935bd1e995LLU;
        const int Remain1 = 47;

        quint64 Hash = TargomanHashKeys[_level % TargomanHashKeysCount] ^ ((_data.length() + _level) * Constant1);

        foreach (Common::WordIndex_t WIndex , _data){
          quint64 K = WIndex;

          K *= Constant1;
          K ^= K >> Remain1;
          K *= Constant1;

          Hash ^= K;
          Hash *= Constant1;
        }

        Hash *= Constant1;

        Hash ^= Hash >> Remain1;
        Hash *= Constant1;
        Hash ^= Hash >> Remain1;

        return Hash;
    }

    // MurmurHash3, 32-bit versions, by Austin Appleby

    // The same caveats as 32-bit MurmurHash2 apply here - beware of alignment
    // and endian-ness issues if used across multiple platforms.
    // 32-bit hash for 32-bit platforms
    static quint32 murmurHash32(const void* _buff, size_t _len, int _level = 0)
    {
        static const quint32 Const1 = 0xcc9e2d51;
        static const quint32 Const2 = 0x1b873593;
        static const quint32 Round1 = 15;
        static const quint32 Round2 = 13;
        static const quint32 M = 5;
        static const quint32 Const3 = 0xe6546b64;

        quint32 Hash = TargomanHashKeys[_level % TargomanHashKeysCount];

        const int BlockCount = _len / 4;
        const quint32 *Blocks = (const quint32 *) _buff;
        int i;
        for (i = 0; i < BlockCount; i++) {
            quint32 Buffer = Blocks[i];
            Buffer *= Const1;
            Buffer = (Buffer << Round1) | (Buffer >> (32 - Round1));
            Buffer *= Const2;

            Hash ^= Buffer;
            Hash = ((Hash << Round2) | (Hash >> (32 - Round2))) * M + Const3;
        }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"
        const quint8 *Tail = (const quint8 *) (_buff + BlockCount * 4);
#pragma GCC diagnostic pop

        quint32 Buffer = 0;

        switch (_len & 3) {
        case 3:
            Buffer ^= Tail[2] << 16;
        case 2:
            Buffer ^= Tail[1] << 8;
        case 1:
            Buffer ^= Tail[0];

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
};

}
}

#endif // TARGOMAN_COMMON_HASHFUNCTIONS_H
