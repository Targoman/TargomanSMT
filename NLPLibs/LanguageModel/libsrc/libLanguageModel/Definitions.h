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

#ifndef TARGOMAN_NLPLIBS_LM_DEFINITIONS_H
#define TARGOMAN_NLPLIBS_LM_DEFINITIONS_H

#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Macros.h"

namespace Targoman {
namespace NLPLibs {

TARGOMAN_ADD_EXCEPTION_HANDLER(exLanguageModel, Targoman::Common::exTargomanBase);

TARGOMAN_DEFINE_ENHANCED_ENUM_BEGIN(enuMemoryModel)
    Auto,           //Auto-detected based on LM file
    Plain,          //Plain ARPA with no binarization
    Probing,        //probing hash tables
    RestProbing,    //probing hash tables with rest costs
    Trie,           //trie
    QuantTrie,      //trie with quantization
    ArrayTrie,      //trie with array-compressed pointers
    QuantArrayTrie  //trie with quantization and array-compressed pointers
TARGOMAN_DEFINE_ENHANCED_ENUM_STRINGS
    "Auto",
    "Plain",
    "Probing",
    "RestProbing",
    "Trie",
    "QuantTrie",
    "ArrayTrie",
    "QuantArrayTrie"
TARGOMAN_DEFINE_ENHANCED_ENUM_END

#ifdef TARGOMAN_ARCHITECTURE_64
typedef qint64 WordIndex_t;
#else
typedef qint32 WordIndex_t;
#endif

//Definitions for special string in ARPA file
const QString LM_UNKNOWN_WORD = "<unk>";
const QString LM_BEGIN_SENTENCE = "<s>";
const QString LM_END_SENTENCE = "</s>";

//Fast access to special predefined words indexes
static WordIndex_t LM_UNKNOWN_WINDEX;
static WordIndex_t LM_BEGIN_SENTENCE_WINDEX;
static WordIndex_t LM_END_SENTENCE_WINDEX;

static void dummy(){
    Q_UNUSED(dummy);
    Q_UNUSED(LM_UNKNOWN_WINDEX);
    Q_UNUSED(LM_BEGIN_SENTENCE_WINDEX);
    Q_UNUSED(LM_END_SENTENCE_WINDEX);
}
//This constant is used to quickly check whether input file is valid ARPA plain text file or not.
//Total lenght of an ARPA must be less than indicated number
const quint16   LM_MAX_VALID_ARPA_LINE = 1000;
//Maximum order supported by LM
const quint8    LM_MAX_ORDER = 6;

#ifndef LogP_t
typedef float LogP_t;
#endif

extern const LogP_t LogP_Zero;            /* log(0) = -Infinity */
extern const LogP_t LogP_Inf;             /* log(Inf) = Infinity */
extern const LogP_t LogP_One;             /* log(1) = 0 */

struct stuLMConfigs
{

};

}
}

#endif // TARGOMAN_NLPLIBS_LM_DEFINITIONS_H
