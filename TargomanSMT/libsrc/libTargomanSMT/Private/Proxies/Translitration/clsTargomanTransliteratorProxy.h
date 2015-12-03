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

// There is no transliteration for anything but Statistical Machine Translation!
#ifndef SMT

#ifndef TARGOMAN_CORE_PRIVATE_PROXIES_TRANSLITERATION_CLSTARGOMANTRANSLITERATORPROXY_H
#define TARGOMAN_CORE_PRIVATE_PROXIES_TRANSLITERATION_CLSTARGOMANTRANSLITERATORPROXY_H

#include "Private/Proxies/intfTransliterator.h"

namespace Targoman {
namespace SMT {
namespace Private {
namespace Proxies {
namespace Transliteration {

class clsTargomanTransliteratorProxy : public intfTransliterator
{
public:
    explicit clsTargomanTransliteratorProxy();

    ~clsTargomanTransliteratorProxy(){}

    void init(QSharedPointer<QSettings> _configSettings);

    QString transliterate(QString _word);

private:
    TARGOMAN_DEFINE_SINGLETONMODULE("TargomanTransliteratorProxy", clsTargomanTransliteratorProxy);
};

}
}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_PROXIES_TRANSLITERATION_CLSTARGOMANTRANSLITERATORPROXY_H

#endif
