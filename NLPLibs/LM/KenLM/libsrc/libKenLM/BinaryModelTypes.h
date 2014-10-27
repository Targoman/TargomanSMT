/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of both GPL vrsion 3 and
 * TCRL(Targoman Community Research License)
 * You can find a copy of TCRL license file with distributed source or
 * download them from http://targoman.com/License.txt
 *
 * This file is part of KenLM implementation in Targoman Project.
 *
 *   KenLM is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   KenLM is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with KenLM.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   KenLM code is derived from Kenneth's code.
 *   Find origianl code at: https://kheafield.com/code/kenlm/
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include "libTargomanCommon/Macros.h"

#ifndef TARGOMAN_NLPLIBS_LM_KENLM_MODELTYPES_H
#define TARGOMAN_NLPLIBS_LM_KENLM_MODELTYPES_H

namespace Targoman {
namespace NLPLibs {
namespace LM {

TARGOMAN_DEFINE_ENHANCED_ENUM_BEGIN(enuBinaryModel)
    Probing,
    RestProbing,
    Trie,
    QuantTrie,
    ArrayTrie,
    QuantArrayTrie
TARGOMAN_DEFINE_ENHANCED_ENUM_STRINGS
    "probing hash tables",
    "probing hash tables with rest costs",
    "trie",
    "trie with quantization",
    "trie with array-compressed pointers",
    "trie with quantization and array-compressed pointers"
TARGOMAN_DEFINE_ENHANCED_ENUM_END
}
}
}
/*
// Historical names.
const ModelType HASH_PROBING = PROBING;
const ModelType TRIE_SORTED = TRIE;
const ModelType QUANT_TRIE_SORTED = QUANT_TRIE;
const ModelType ARRAY_TRIE_SORTED = ARRAY_TRIE;
const ModelType QUANT_ARRAY_TRIE_SORTED = QUANT_ARRAY_TRIE;

const static ModelType kQuantAdd = static_cast<ModelType>(QUANT_TRIE - TRIE);
const static ModelType kArrayAdd = static_cast<ModelType>(ARRAY_TRIE - TRIE);
*/

#endif // TARGOMAN_NLPLIBS_LM_KENLM_MODELTYPES_H
