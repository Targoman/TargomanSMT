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
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#ifndef TARGOMAN_CORE_TYPES_H
#define TARGOMAN_CORE_TYPES_H

#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Types.h"
#include <QPair>

namespace UnitTestNameSpace {
class clsUnitTest;
}

class clsDummyFeatureFunctionForInsertion;

namespace Targoman{
namespace SMT {

using namespace Targoman::Common;

TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanCore, Targoman::Common::exTargomanBase);

struct stuTranslationOutput{

    struct stuMetaInfo{
        stuPos      SourceWordsPos;
        stuPos      TargetWordsPos;
        QStringList TranslationOptions;

        stuMetaInfo(const stuPos& _sourceWordsPos,
                    const stuPos& _targetWordsPos,
                    const QStringList&  _translationOptions){
            this->SourceWordsPos     = _sourceWordsPos;
            this->TargetWordsPos     = _targetWordsPos;
            this->TranslationOptions = _translationOptions;
        }
    };

    QString             Translation;
    QString             TaggedSource;
    QString             OriginalText;
    bool                SpellCorrected;
    QList<stuMetaInfo>  MetaInfo;
    QList<QString>      NBestTranslations;
};

typedef stuTranslationOutput::stuMetaInfo               TranslationMetaInfo_t;

}
}
#endif // TARGOMAN_CORE_TYPES_H
