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

#include "GlobalConfigs.h"
#include "Private/Proxies/intfLMSentenceScorer.hpp"

namespace Targoman {
namespace SMT {
/**
 * @brief Private section of TargomanSMT library which obfuscates all internal namespaces and classes
 */
namespace Private{

using namespace Common;
using namespace Common::Configuration;

QString ActorUUID;

stuGlobalConfigs gConfigs;

tmplConfigurable<QString> stuGlobalConfigs::Separator(
        "/Common/Separator",
        "Separator used on multi field configurations",
        ";:;");
tmplConfigurable<QString> stuGlobalConfigs::SourceLanguage(
        "/Common/Language/Source",
        "Source language from which translating",
        "en");
tmplConfigurable<QString> stuGlobalConfigs::TargetLanguage(
        "/Common/Language/Destination",
        "Destination Language to which translating",
        "fa");

tmplConfigurable<enuWorkingModes::Type> stuGlobalConfigs::WorkingMode(
        "/Common/WorkingMode",
        "WorkingModes can be (" + enuWorkingModes::options().join("|") + ")",
        enuWorkingModes::toStr(enuWorkingModes::Decode));

QMap<QString, FeatureFunction::intfFeatureFunction*>       stuGlobalConfigs::ActiveFeatureFunctions;

clsModuleConfig         stuGlobalConfigs::LM("/Modules/LM",
                                             "TODO Desc",
                                             "TargomanLM");
clsModuleConfig         stuGlobalConfigs::RuleTable("/Modules/RuleTable",
                                                    "TODO Desc",
                                                    "JanePTPlain");
QScopedPointer<Targoman::SMT::Private::Proxies::intfLMSentenceScorer>  stuGlobalConfigs::EmptyLMScorer;
QHash<QString, Common::WordIndex_t>                            stuGlobalConfigs::SourceVocab;



}
}
namespace Common {
namespace Configuration {

using namespace SMT::Private;

template <>
bool tmplConfigurable<enuWorkingModes::Type>::validate(const QVariant& _value, QString& _errorMessage) const{

    if(enuWorkingModes::toEnum(_value.toString().toLatin1().constData()) != enuWorkingModes::Unknown)
        return true;
    _errorMessage = "Unrecognized option: " + _value.toString() + " for: " + this->configPath();
    return false;
}

template <>
void tmplConfigurable<enuWorkingModes::Type>::setFromVariant(const QVariant& _value){
    QString ErrorMessage;
    if (this->validate(_value, ErrorMessage)) this->Value =
            enuWorkingModes::toEnum(_value.toString().toLatin1().constData());
    else throw exConfiguration(this->ConfigPath + ": " + ErrorMessage);
}

}
}
}
