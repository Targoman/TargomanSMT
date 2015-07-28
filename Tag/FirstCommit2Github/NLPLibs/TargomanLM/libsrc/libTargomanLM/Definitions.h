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

#ifndef TARGOMAN_NLPLIBS_LANGUAGEMODEL_DEFINITIONS_H
#define TARGOMAN_NLPLIBS_LANGUAGEMODEL_DEFINITIONS_H

#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/Types.h"

namespace Targoman {
namespace NLPLibs {
namespace TargomanLM {

TARGOMAN_ADD_EXCEPTION_HANDLER(exLanguageModel, Targoman::Common::exTargomanBase);

TARGOMAN_DEFINE_ENHANCED_ENUM(enuMemoryModel,
                              Auto,           //Auto-detected based on LM file
                              Plain,          //Plain ARPA with no binarization
                              Probing,        //probing hash tables
                              RestProbing,    //probing hash tables with rest costs
                              );

//Definitions for special string in ARPA file
extern const char* LM_UNKNOWN_WORD;
extern const char* LM_BEGIN_SENTENCE;
extern const char* LM_END_SENTENCE;
extern Common::WordIndex_t LM_UNKNOWN_WINDEX;
extern Common::WordIndex_t LM_BEGIN_SENTENCE_WINDEX;
extern Common::WordIndex_t LM_END_SENTENCE_WINDEX;

//This constant is used to quickly check whether input file is valid ARPA plain text file or not.
//Total lenght of an ARPA must be less than indicated number
const quint16   LM_MAX_VALID_ARPA_LINE = 1000;
//Maximum order supported by LM
const quint8    LM_MAX_ORDER = 6;

struct stuLMConfigs
{
    struct{
        Targoman::Common::LogP_t Prob;
        Targoman::Common::LogP_t Backoff;
    }UnknownWordDefault;

    bool    UseIndexBasedModel;
    bool    VerifyBinaryCheckSum;

    stuLMConfigs(Targoman::Common::LogP_t _unkProb = 0,
                 Targoman::Common::LogP_t _unkBackoff = 0,
                 bool _useIdexBasedModel = true,
                 bool _verifyBinaryCheckSum = true){
        this->UnknownWordDefault.Prob = _unkProb;
        this->UnknownWordDefault.Backoff = _unkBackoff;
        this->UseIndexBasedModel = _useIdexBasedModel;
        this->VerifyBinaryCheckSum = _verifyBinaryCheckSum;
    }
};

struct stuLMResult{
    Targoman::Common::LogP_t  Prob;
    quint8  NGram;
    inline stuLMResult(Targoman::Common::LogP_t _prob, quint8 _ngram){
        this->NGram = _ngram;
        this->Prob = _prob;
    }
};

}
}
}

#endif // TARGOMAN_NLPLIBS_LM_DEFINITIONS_H
