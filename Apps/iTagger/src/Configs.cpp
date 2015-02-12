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

#include "Configs.h"

namespace Targoman {

using namespace Common;
using namespace Common::Configuration;

tmplConfigurable<enuAppMode::Type> gConfigs::Mode(gConfigs::appConfig("Mode"),
                                                  "Working Mode of the application. Can be " + enuAppMode::listOptions(),
                                                  "Text2IXML",
                                                  NULL,
                                                  "m",
                                                  "ApplicationMode"
                                                  "mode"
                                                  );

tmplConfigurable<QString>     gConfigs::InputFile(gConfigs::appConfig("InputFile"),
                                                  "Input file path to convert",
                                                  "",
                                                  new Validators::clsPathValidator(PathAccess::File | PathAccess::Readable),
                                                  "f",
                                                  "FilePath",
                                                  "input-file");

tmplConfigurable<QString>     gConfigs::InputDir(gConfigs::appConfig("InputDir"),
                                                 "Input directory to convert",
                                                 "",
                                                 new Validators::clsPathValidator(PathAccess::Dir | PathAccess::Readable),
                                                 "d",
                                                 "Path",
                                                 "input-dir");

tmplConfigurable<QString>     gConfigs::Output(gConfigs::appConfig("Output"),
                                               "output path to write converted file/files",
                                               "",
                                               new Validators::clsPathValidator(
                                                   PathAccess::Dir | PathAccess::File | PathAccess::Writeatble),
                                               "o",
                                               "Path",
                                               "output");

tmplConfigurable<QRegExp,true>     gConfigs::IncludePattern(gConfigs::appConfig("IncludePattern"),
                                                            "FilePatterns to include when converting (WildCard format)",
                                                            "",
                                                            NULL,
                                                            "p",
                                                            "Pattern",
                                                            "pattern");

tmplConfigurable<QString>     gConfigs::DTDFilePath(gConfigs::appConfig("DTDFilePath"),
                                                "DTD file used to validate XML Files",
                                                "",
                                                new Validators::clsPathValidator(PathAccess::File | PathAccess::Readable),
                                                "",
                                                "DTD_Path",
                                                "dtd");


/**************************************************************************************************************/
//////enuAppMode
template <>
bool tmplConfigurable<enuAppMode::Type>::validate(const QVariant& _value, QString& _errorMessage) const{
    if (enuAppMode::toEnum(_value.toString().toLatin1().constData()) == enuAppMode::Unknown) {
        _errorMessage = "Invalid AppMode: " + _value.toString() + " Selected";
        return false;
    }else
        return true;
}

template <>
void tmplConfigurable<enuAppMode::Type>::setFromVariant(const QVariant& _value){
    QString ErrorMessage;
    if (this->validate(_value, ErrorMessage)) this->Value = enuAppMode::toEnum(_value.toString().toLatin1().constData());
    else throw exConfiguration(this->ConfigPath + ": " + ErrorMessage);
}

}
