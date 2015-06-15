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
#include <QPair>

namespace UnitTestNameSpace {
class clsUnitTest;
}

class clsDummyFeatureFunctionForInsertion;

namespace Targoman{
namespace SMT {

TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanCore, Targoman::Common::exTargomanBase);

struct stuPhrasePos : public QPair<qint32, qint32>{
    stuPhrasePos(qint32 _start = -1, qint32 _end = -1) :
       QPair<qint32, qint32>(_start, _end) {
        Q_ASSERT((_start < _end) || (_start == -1 && _end == -1));
    }

    size_t start()const         {return this->first;}
    size_t end()const           {return this->second;}
    bool   isValid() const      {return this->first >= 0;}
    bool   isSingleWord() const {return this->second - this->first == 1; }
};

struct stuTranslationOutput{

    struct stuMetaInfo{
        stuPhrasePos     SourceWordsPos;
        stuPhrasePos     TargetWordsPos;
        QStringList      TranslationOptions;

        stuMetaInfo(const stuPhrasePos& _sourceWordsPos,
                    const stuPhrasePos& _targetWordsPos,
                    const QStringList&  _translationOptions){
            this->SourceWordsPos     = _sourceWordsPos;
            this->TargetWordsPos     = _targetWordsPos;
            this->TranslationOptions = _translationOptions;
        }
    };

    QString            Translation;
    QString            NormalizedSource;
    QString            OriginalText;
    bool               SpellCorrected;
    QList<stuMetaInfo> MetaInfo;
};

typedef stuTranslationOutput::stuMetaInfo               TranslationMetaInfo_t;

}
}
#endif // TARGOMAN_CORE_TYPES_H
