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
 @author Behrooz Vedadian <vedadian@gmail.com>
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
        "KenLM/FilePath",
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
clsKenLMProxy::clsKenLMProxy(quint64 _instanceID) :
    intfLMSentenceScorer(this->moduleName(), _instanceID) {
    if(clsKenLMProxy::LM.data())
        this->State = clsKenLMProxy::LM->BeginSentenceState();
}

clsKenLMProxy::~clsKenLMProxy()
{
    this->unregister();
}

}
}
}
}
}
