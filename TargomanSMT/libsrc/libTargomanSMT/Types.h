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

TARGOMAN_DEFINE_ENHANCED_ENUM(enuOutputFormat,
    JustBestTranslation,
    BestTranslationAndPhraseSuggestions,
    NBestTranslations
);

struct stuTranslationOutput{

    struct stuPhraseAlternatives {
        stuPos      SourceWordsPos;
        stuPos      TargetWordsPos;
        QStringList Alternatives;

        stuPhraseAlternatives(const stuPos& _sourceWordsPos,
                    const stuPos& _targetWordsPos,
                    const QStringList&  _translationOptions){
            this->SourceWordsPos     = _sourceWordsPos;
            this->TargetWordsPos     = _targetWordsPos;
            this->Alternatives = _translationOptions;
        }
    };

    struct stuCostElement {
        QString FeatureName;
        Cost_t  Cost;
        stuCostElement(const QString _featureName, const Cost_t _cost) {
            this->FeatureName = _featureName;
            this->Cost = _cost;
        }
    };

    struct stuCostElements {
        QList<stuCostElement> Elements;
        Cost_t Total;
        int size() const { return this->Elements.size(); }
    };

    QStringList                     Translations;
    QString                         TaggedSource;
    QString                         OriginalSource;
    bool                            SpellCorrected;
    QList<stuPhraseAlternatives>    BestTranslationPhraseAlternatives;
    QList<stuCostElements>           TranslationsCostElements;
};

typedef stuTranslationOutput::stuPhraseAlternatives               PhraseAlternatives_t;

}
}
#endif // TARGOMAN_CORE_TYPES_H
