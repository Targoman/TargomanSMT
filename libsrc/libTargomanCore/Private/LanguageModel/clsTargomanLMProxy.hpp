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

#ifndef TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_CLSTARGOMANLMPROXY_HPP
#define TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_CLSTARGOMANLMPROXY_HPP

#include "intfLMSentenceScorer.hpp"
#include "libLanguageModel/clsLMSentenceScorer.h"
#include "libLanguageModel/clsLanguageModel.h"
#include "libTargomanCommon/Configuration/intfConfigurable.hpp"

namespace Targoman {
namespace Core {
namespace Private {
namespace LanguageModel{

class clsTargomanLMProxy :
        public intfLMSentenceScorer, public Targoman::NLPLibs::clsLMSentenceScorer{
public:
    clsTargomanLMProxy() :
        Targoman::NLPLibs::clsLMSentenceScorer(LM){}

private:
    static Common::Configuration::intfModule* instantiator(){return new clsTargomanLMProxy;}
    static Targoman::NLPLibs::clsLanguageModel LM;
    static Common::Configuration::clsModuleRegistrar Registrar;
};

Common::Configuration::clsModuleRegistrar clsTargomanLMProxy::Registrar("TargomanLM", clsTargomanLMProxy::instantiator());
}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_LANGUAGEMODEL_CLSTARGOMANLMPROXY_HPP
