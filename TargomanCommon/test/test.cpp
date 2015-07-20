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

#include "test.h"

using namespace Targoman::Common::Configuration;

ENUM_CONFIGURABLE_IMPL(/*Targoman::Common::Configuration::*/enuTestEnum);

tmplConfigurable<enuTestEnum::Type> TestE(
        "asds",
        "sdd",
        /*Targoman::Common::Configuration::*/enuTestEnum::B);

tmplConfigurable<QRegExp,true>     WildPattern(
        "IncludePattern",
        "FilePatterns to include when converting (WildCard format)",
        "",
        ReturnTrueCrossValidator,
        "w",
        "PTRN",
        "wildcard",
        (enuConfigSource::Type)(
            enuConfigSource::Arg  |
            enuConfigSource::File));

tmplConfigurable<QRegExp,false>     RegexPattern(
        "RegexPattern",
        "FilePatterns to include when converting (WildCard format)",
        "",
        ReturnTrueCrossValidator,
        "p",
        "PTRN",
        "pattern",
        (enuConfigSource::Type)(
            enuConfigSource::Arg  |
            enuConfigSource::File));
