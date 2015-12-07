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
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include "clsKenLMProxy.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "Private/GlobalConfigs.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace Proxies {
/**
 *  @brief Proxy to all supported external LanguageModel libraries including TargomanLM
 */
namespace LanguageModel {

using namespace Common;
using namespace Common::Configuration;

tmplConfigurable<FilePath_t> clsKenLMProxy::FilePath(
        MAKE_CONFIG_PATH("FilePath"),
        "File path of ARPA or binary models",
        "",
        ConditionalPathValidator(
            gConfigs.LM.toVariant().toString() == clsKenLMProxy::moduleName(),
            enuPathAccess::File | enuPathAccess::Readable)
        );

QScopedPointer<lm::ngram::ProbingModel> clsKenLMProxy::LM;

clsKenLMProxy::clsVocabEnumerator clsKenLMProxy::Vocab;

TARGOMAN_REGISTER_MODULE(clsKenLMProxy);

/**
 * @brief Constructor of this class initializes its parrent class with its module name and instantiates #LMSentenceScorer with #clsKenLMProxy::LM
 */
clsKenLMProxy::clsKenLMProxy() {
    if(clsKenLMProxy::LM.data())
        this->State = clsKenLMProxy::LM->BeginSentenceState();
}

clsKenLMProxy::~clsKenLMProxy()
{
}

}
}
}
}
}
