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

#ifndef TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_WEIGHTS_HPP
#define TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_WEIGHTS_HPP

#include "libTargomanCommon/Types.h"

namespace Targoman {
namespace NLPLibs {
namespace TargomanLM {
namespace Private {

struct stuProb {
    float Prob;
};

/**
 * @struct A structure for encapsulating data from hash table.
 */
struct stuProbAndBackoffWeights {
  Common::WordIndex_t ID;       /**< index of NGram in hash table */
  float Prob;                   /**< probablity of NGram */
  float Backoff;                /**< backoff weights of NGram */
  inline stuProbAndBackoffWeights(Common::WordIndex_t _id = 0, float _prob = 0, float _backoff = 0){
      this->Prob = _prob;
      this->Backoff = _backoff;
      this->ID = _id;
  }
};


struct stuProbBackoffAndRestWeights {
  float Prob;
  float Backoff;
  float Rest;
};

}
}
}
}

inline QDataStream& operator << (QDataStream& _stream, const Targoman::NLPLibs::TargomanLM::Private::stuProbAndBackoffWeights& _value){
    return _stream<<_value.ID<<_value.Prob<<_value.Backoff;
}

inline QDataStream& operator >> (QDataStream& _stream, Targoman::NLPLibs::TargomanLM::Private::stuProbAndBackoffWeights& _value){
    return _stream>>_value.ID>>_value.Prob>>_value.Backoff;
}

#endif // TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_WEIGHTS_HPP
