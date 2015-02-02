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

clsTargomanLMProxy::clsTargomanLMProxy() :
    intfLMSentenceScorer(clsTargomanLMProxy::moduleName()), Targoman::NLPLibs::clsLMSentenceScorer(LM){

}

}
}
}
}
