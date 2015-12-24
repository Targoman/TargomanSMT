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

#ifndef TARGOMAN_CORE_PRIVATE_PROXIES_TRANSLITERATION_INTFNAMEDENTITYRECOGNIZER_HPP
#define TARGOMAN_CORE_PRIVATE_PROXIES_TRANSLITERATION_INTFNAMEDENTITYRECOGNIZER_HPP

#include "libTargomanCommon/Types.h"
#include "libTargomanCommon/Configuration/intfConfigurable.hpp"
#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"
#include "Private/GlobalConfigs.h"
#include "Private/InputDecomposer/clsInput.h"

#define NER_TAG_ATTR_KEY "NER_TAG"

namespace Targoman {
namespace SMT {
namespace Private {
/**
 *  @brief Namespace surrounding all classes and interfaces to external libraries
 */
namespace Proxies {
namespace NamedEntityRecognition {

class intfNamedEntityRecognizer : public Common::Configuration::intfModule
{
public:
    intfNamedEntityRecognizer()
    { }

    virtual ~intfNamedEntityRecognizer() { }
    TARGOMAN_DEFINE_MODULE_SCOPE(intfNamedEntityRecognizer)

    virtual void init(QSharedPointer<QSettings> _configSettings) = 0;
    virtual void tagNamedEntities(InputDecomposer::Sentence_t _sentence) = 0;

protected:
    void addTokenAttr(InputDecomposer::clsToken& _token, const QString& _key, const QVariant _value) {
        _token.Attrs.insert(_key, _value);
    }
};

}
}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_PROXIES_TRANSLITERATION_INTFNAMEDENTITYRECOGNIZER_HPP

#endif
