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
};

typedef stuTranslationOutput::stuMetaInfo               TranslationMetaInfo_t;

}
}
#endif // TARGOMAN_CORE_TYPES_H
