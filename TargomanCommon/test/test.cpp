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
 */

#include "test.h"

using namespace Targoman::Common::Configuration;

ENUM_CONFIGURABLE_IMPL(/*Targoman::Common::Configuration::*/enuTestEnum);

tmplConfigurable<enuTestEnum::Type> TestE(
        clsConfigPath("asds"),
        "sdd",
        /*Targoman::Common::Configuration::*/enuTestEnum::B);

tmplConfigurable<QWildCard>     WildPattern(
        clsConfigPath("IncludePattern"),
        "FilePatterns to include when converting (WildCard format)",
        "",
        ReturnTrueCrossValidator,
        "w",
        "PTRN",
        "wildcard",
        (enuConfigSource::Type)(
            enuConfigSource::Arg  |
            enuConfigSource::File));

tmplConfigurable<QRegExp>     RegexPattern(
        clsConfigPath("RegexPattern"),
        "FilePatterns to include when converting (WildCard format)",
        "",
        ReturnTrueCrossValidator,
        "p",
        "PTRN",
        "pattern",
        (enuConfigSource::Type)(
            enuConfigSource::Arg  |
            enuConfigSource::File));
