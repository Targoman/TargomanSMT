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
 */

#ifndef TEST_H
#define TEST_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

TARGOMAN_DEFINE_ENHANCED_ENUM(enuTestEnum,
                              A,B);
ENUM_CONFIGURABLE(enuTestEnum);

extern Targoman::Common::Configuration::tmplConfigurable<enuTestEnum::Type> TestE;
extern Targoman::Common::Configuration::tmplConfigurable<QWildCard>      WildPattern;
extern Targoman::Common::Configuration::tmplConfigurable<QRegExp>     RegexPattern;

#endif // TEST_H
