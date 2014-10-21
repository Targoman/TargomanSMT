/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 * 
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include <QStringList>

#include "clsPersianSpellCorrector.h"
#include "libTargomanTextProcessor/Private/Normalizer.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

const QString PERSIAN_Mi         = QString::fromUtf8("می");
const QString PERSIAN_Tar        = QString::fromUtf8("تر");
const QString PERSIAN_Tarin      = QString::fromUtf8("ترین");
const QString PERSIAN_Ha         = QString::fromUtf8("ها");


clsPersianSpellCorrector::clsPersianSpellCorrector()
{
    this->RxInteractiveChars = QRegExp(QString::fromUtf8("[ؤئإأآ]"));
    this->RxVerbFinisher     = QRegExp(QString::fromUtf8("(م|ی|یم|ید|ند)$"));
    this->RxHa               = QRegExp(QString::fromUtf8("ها)ی|یم|یت|یش|یمان|یتان|یشان(?"));
}

bool clsPersianSpellCorrector::init(const QVariantHash _settings)
{
    return true;
}

QString clsPersianSpellCorrector::process(const QStringList &_tokens)
{
    QString Buffer;
    if (_tokens.size() == 2){
        if (_tokens.first() == PERSIAN_Mi){
            Buffer = _tokens.last();
            Buffer.remove(this->RxVerbFinisher);
            if(this->CanStartWithMi.contains(Buffer)){
                return PERSIAN_Mi + ARABIC_ZWNJ + _tokens.last();
            }
        }else if (_tokens.first().startsWith(PERSIAN_Mi)){
            Buffer = _tokens.first();
            Buffer.remove(0,2);
            Buffer.remove(this->RxVerbFinisher);
            if(this->CanStartWithMi.contains(Buffer)){
                return PERSIAN_Mi + ARABIC_ZWNJ + _tokens.first().mid(2);
            }
        }else if (this->RxHa.exactMatch(_tokens.last())){
         //   if(this->CanAppendHa.contains())
        }

    }
    return _tokens.join(" ");
}

bool clsPersianSpellCorrector::canBeCheckedInteractive(const QString &_inputWord)
{
    return _inputWord.contains(this->RxInteractiveChars);
}

}
}
}
