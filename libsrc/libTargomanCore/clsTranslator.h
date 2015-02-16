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

#ifndef TARGOMAN_CORE_CLSTRANSLATOR_H
#define TARGOMAN_CORE_CLSTRANSLATOR_H

#include <QMap>
#include <QStringList>
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman{
namespace Core {

TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanCore, Targoman::Common::exTargomanBase);

struct stuTranslationOutput{
    struct stuCharacterRange{
        quint16 Start;
        quint16 End;
        stuCharacterRange(quint16 _start = 0, quint16 _end = 0){
            Q_ASSERT(_start <= _end);
            this->Start = _start;
            this->End = _end;
        }
    };

    struct stuMetaInfo{
        QList<stuCharacterRange> SourceRanges;
        QList<stuCharacterRange> TargetRanges;
        QStringList              TranslationOptions;
    };

    QString Translation;
    QList<stuMetaInfo> MetaInfo;
};

namespace Private {
class clsTranslatorPrivate;
}
class clsTranslator
{
public:
    clsTranslator(const QString &_inputStr);
    ~clsTranslator();

    static void init(const QString _configFilePath);

    //Input language is predefined
    stuTranslationOutput translate();

private:
    QScopedPointer<Private::clsTranslatorPrivate> pPrivate;
};

}
}



#endif // TARGOMAN_CORE_CLSTRANSLATOR_H
