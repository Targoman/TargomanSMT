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

#include "clsTargomanLMProxy.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/Validators.hpp"

using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;

namespace Targoman {
namespace SMT {
namespace Private {
namespace Proxies {
/**
 *  @brief Proxy to all supported external LanguageModel libraries including TargomanLM
 */
namespace LanguageModel {

Targoman::NLPLibs::TargomanLM::clsLanguageModel clsTargomanLMProxy::LM;
TARGOMAN_REGISTER_MODULE(clsTargomanLMProxy);

/**
 * @brief Constructor of this class initializes its parrent class with its module name and instantiates #LMSentenceScorer with #clsTargomanLMProxy::LM
 */

clsTargomanLMProxy::clsTargomanLMProxy() :
    LMSentenceScorer(new Targoman::NLPLibs::TargomanLM::clsLMSentenceScorer(clsTargomanLMProxy::LM)){}

clsTargomanLMProxy::~clsTargomanLMProxy()
{}

}
}
}
}
}
