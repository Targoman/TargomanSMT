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

#include "clsTargomanLMProxy.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace LanguageModel{

Targoman::NLPLibs::clsLanguageModel clsTargomanLMProxy::LM;
TARGOMAN_REGISTER_MODULE(clsTargomanLMProxy)

/**
 * @brief Constructor of this class initializes its parrent class with its module name and instantiates #LMSentenceScorer with #clsTargomanLMProxy::LM
 */

clsTargomanLMProxy::clsTargomanLMProxy() :
    intfLMSentenceScorer(clsTargomanLMProxy::moduleName()),
    LMSentenceScorer(new Targoman::NLPLibs::clsLMSentenceScorer(clsTargomanLMProxy::LM)){

}

clsTargomanLMProxy::~clsTargomanLMProxy()
{
  //Just to suppress compiler error on using QScoppedPointer
}

}
}
}
}
