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

// There is no transliteration for anything but Statistical Machine Translation!
#ifndef SMT

#include "TargomanTransliteratorProxy.h"

#ifndef TARGOMAN_CORE_TRANSLITERATOR_H
#define TARGOMAN_CORE_TRANSLITERATOR_H
#ifdef TARGOMAN_CORE_CLSTRANSLATOR_H
#undef TARGOMAN_CORE_CLSTRANSLATOR_H
#endif
#ifdef TARGOMAN_CORE_TYPES_H
#undef TARGOMAN_CORE_TYPES_H
#endif
#define SMT SWT
#include "Translator.h"
#undef SMT
#endif

namespace Targoman {
namespace SMT {
namespace Private {
namespace Proxies {
namespace Transliteration {

TARGOMAN_REGISTER_SINGLETON_MODULE(TargomanTransliteratorProxy);

TargomanTransliteratorProxy::TargomanTransliteratorProxy()
{ }

void TargomanTransliteratorProxy::init(QSharedPointer<QSettings> _configSettings)
{
    Targoman::SWT::Translator::init(_configSettings);
}

QString TargomanTransliteratorProxy::transliterate(QString _word)
{
    Q_UNUSED(_word);
    return QString();
}

}
}
}
}
}

#endif
