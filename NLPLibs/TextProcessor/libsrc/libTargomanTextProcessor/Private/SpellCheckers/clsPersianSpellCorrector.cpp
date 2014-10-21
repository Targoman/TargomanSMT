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
const QString PERSIAN_Nemi       = QString::fromUtf8("نمی");
const QString PERSIAN_Tar        = QString::fromUtf8("تر");
const QString PERSIAN_Tarin      = QString::fromUtf8("ترین");
const QString PERSIAN_Ha         = QString::fromUtf8("ها");
const QString PERSIAN_Ba         = QString::fromUtf8("با");
const QString PERSIAN_Na         = QString::fromUtf8("نا");
const QString PERSIAN_Bi         = QString::fromUtf8("بی");


clsPersianSpellCorrector::clsPersianSpellCorrector()
{
    this->RxInteractiveChars = QRegExp(QString::fromUtf8("[ؤئإأآ]"));
    this->RxVerbFinisher     = QRegExp(QString::fromUtf8("(م|ی|یم|ید|ند)$"));
    this->RxHa               = QRegExp(QString::fromUtf8("ها)ی|یم|یت|یش|یمان|یتان|یشان(?"));
    this->RxEndWithHa        = QRegExp(".*" + this->RxHa.pattern() + "$");
}

bool clsPersianSpellCorrector::init(const QVariantHash _settings)
{
    //TODO Load from file and set     MaxAutoCorrectTokens
    this->MaxAutoCorrectTokens = qMax((uint)4, this->MaxAutoCorrectTokens);
}

QString clsPersianSpellCorrector::process(const QStringList &_tokens)
{
    QString Buffer, Prefix, Postfix;

    Buffer = this->AutoCorrectTerms.value(_tokens.join(" "));
    if (Buffer.size())
        return Normalizer::instance().normalize(Buffer);

    switch(_tokens.size()){
    case 1:
        if (_tokens.first().startsWith(PERSIAN_Mi)){
            //جداسازی «می» چسبیده به فعل
            Buffer = _tokens.first();
            Buffer.remove(0,2);
            Buffer.remove(this->RxVerbFinisher);
            if(this->CanStartWithMi_Nemi.contains(Normalizer::fullTrim(Buffer)))
                return Normalizer::instance().normalize(PERSIAN_Mi + ARABIC_ZWNJ + _tokens.first().mid(2));
        }else if (_tokens.first().startsWith(PERSIAN_Nemi)){
            //جداسازی «نمی» چسبیده به فعل
            Buffer = _tokens.first();
            Buffer.remove(0,3);
            Buffer.remove(this->RxVerbFinisher);
            if(this->CanStartWithMi_Nemi.contains(Normalizer::fullTrim(Buffer)))
                return Normalizer::instance().normalize(PERSIAN_Nemi + ARABIC_ZWNJ + _tokens.first().mid(3));
        }else if(_tokens.first().startsWith(PERSIAN_Bi)){
            //جداسازی «بی» چسبیده به صفت و اسم
            Buffer = _tokens.first();
            Prefix = Buffer.mid(0,2);
            Buffer.remove(0,2); //Remove Bi

            return this->processStartingWithBi_Ba_Na(this->CanStartWithBi_Ba, Prefix, Buffer);
        }else{
            Buffer = _tokens.first();
            Postfix = Buffer;
            Buffer.remove(RxHa);
            Postfix.remove(0, Postfix.length() - Buffer.length());
            if(Postfix.size()){
                //جداسازی ترکیبات «ها» در صورت وجود
                if (this->CanAppendHa.contains(Normalizer::fullTrim(Buffer)))
                    return Normalizer::instance().normalize(Buffer + ARABIC_ZWNJ + Postfix);
                if (Buffer.endsWith(PERSIAN_Tar)){
                    //جداسازی صفت تفضیلی جمع
                    Buffer.truncate(Buffer.size() - 2);
                    if (this->CanAppendTar_Tarin.contains(Normalizer::fullTrim(Buffer)))
                        return Normalizer::instance().normalize(Buffer + ARABIC_ZWNJ + PERSIAN_Tar + ARABIC_ZWNJ + Postfix);
                }else if (Buffer.endsWith(PERSIAN_Tarin)){
                    //جداسازی صفت افضل جمع
                    Buffer.truncate(Buffer.size() - 4);
                    if (this->CanAppendTar_Tarin.contains(Normalizer::fullTrim(Buffer)))
                        return Normalizer::instance().normalize(Buffer + ARABIC_ZWNJ + PERSIAN_Tar + ARABIC_ZWNJ + Postfix);
                }
            }else if (Buffer.endsWith(PERSIAN_Tar)){
                //جداسازی صفت تفضیلی مفرد
                Buffer.truncate(Buffer.size() - 2);
                if (this->CanAppendTar_Tarin.contains(Normalizer::fullTrim(Buffer)))
                    return Normalizer::instance().normalize(Buffer + ARABIC_ZWNJ + PERSIAN_Tar);
            }else if (Buffer.endsWith(PERSIAN_Tarin)){
                //جداسازی صفت افضل مفرد
                Buffer.truncate(Buffer.size() - 4);
                if (this->CanAppendTar_Tarin.contains(Normalizer::fullTrim(Buffer)))
                    return Normalizer::instance().normalize(Buffer + ARABIC_ZWNJ + PERSIAN_Tar);
            }
        }
    case 2:
        if (_tokens.first() == PERSIAN_Mi){
            //چسباندن «می» قبل از افعال
            Buffer = _tokens.last();
            Buffer.remove(this->RxVerbFinisher);
            if(this->CanStartWithMi_Nemi.contains(Normalizer::fullTrim(Buffer)))
                return Normalizer::instance().normalize(PERSIAN_Mi + ARABIC_ZWNJ + _tokens.last());
        }else if (_tokens.first() == PERSIAN_Mi){
            //چسباندن «نمی» قبل از افعال
            Buffer = _tokens.last();
            Buffer.remove(this->RxVerbFinisher);
            if(this->CanStartWithMi_Nemi.contains(Normalizer::fullTrim(Buffer)))
                return Normalizer::instance().normalize(PERSIAN_Nemi + ARABIC_ZWNJ + _tokens.last());
        }else if (_tokens.first() == PERSIAN_Ba || _tokens.first() == PERSIAN_Bi){
            //از آنجایی که لغت «بی‌معرفت» دارای معنی متفاوت با ترکیب «بی معرفت» است لازم است برای اطمینان از چسبیدن «بی» و «با» به کلمه بعدی انتهای کلمه جمع یا تفضیلی باشد
            /*  if(this->CanStartWithBi_Ba.contains(Normalizer::fullTrim(Buffer)))
                return Normalizer::instance().normalize(_tokens.first() + ARABIC_ZWNJ + _tokens.last());
            */

            return this->processStartingWithBi_Ba_Na(this->CanStartWithBi_Ba, _tokens.first(), _tokens.last());
        }else if (_tokens.first() == PERSIAN_Na){
            //چسباندن «نا» قبل از صفت
            if(this->CanStartWithNa.contains(Normalizer::fullTrim(Buffer)))
                return Normalizer::instance().normalize(_tokens.first() + ARABIC_ZWNJ + _tokens.last());

            return this->processStartingWithBi_Ba_Na(this->CanStartWithNa, _tokens.first(), _tokens.last());
        }else if (this->RxHa.exactMatch(Normalizer::fullTrim(_tokens.last()))){
            if(this->CanAppendHa.contains(_tokens.first()))
                return Normalizer::instance().normalize(_tokens.first() + ARABIC_ZWNJ + _tokens.last());

            if (_tokens.first().startsWith(PERSIAN_Bi) || _tokens.first().startsWith(PERSIAN_Ba)){
                Buffer = this->processStartingWithBi_Ba_Na(this->CanStartWithBi_Ba, _tokens.first().mid(0,2), _tokens.first().mid(2));
                if (Buffer.size())
                    return Normalizer::instance().normalize(Buffer + ARABIC_ZWNJ + _tokens.last());
            }
        }

    }
    //TODO He hamze and He Ye

    return "";
}

bool clsPersianSpellCorrector::canBeCheckedInteractive(const QString &_inputWord)
{
    return _inputWord.contains(this->RxInteractiveChars);
}

QString clsPersianSpellCorrector::processStartingWithBi_Ba_Na(const QSet<QString>& _set, const QString &_prefix, const QString &_postfix)
{
    QString Buffer = _postfix;
    if (RxEndWithHa.exactMatch(Normalizer::fullTrim(Buffer))){
        //الحاق «با» و «بی» چسبیده به صفت و ترکیبات «نا» جمع
        QString Postfix = Buffer;
        Buffer.remove(RxHa); //Remove combinations of Ha
        Postfix.remove(0, Postfix.length() - Buffer.length());

        if (_set.contains(Normalizer::fullTrim(Buffer)))
            return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + Postfix);

        if (Buffer.endsWith(PERSIAN_Tar)){
            //الحاق «با» و «بی» چسبیده به  صفت و ترکیبات «نا» تفضیلی جمع
            Buffer.truncate(Buffer.size() - 2);
            if (this->CanAppendTar_Tarin.contains(Normalizer::fullTrim(Buffer)))
                return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + PERSIAN_Tar + ARABIC_ZWNJ + Postfix);
        }else if (Buffer.endsWith(PERSIAN_Tarin)){
            //الحاق «با» و «بی» چسبیده به  صفت و ترکیبات «نا» افضل جمع
            Buffer.truncate(Buffer.size() - 4);
            if (this->CanAppendTar_Tarin.contains(Normalizer::fullTrim(Buffer)))
                return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + PERSIAN_Tarin + ARABIC_ZWNJ + Postfix);
        }
    }else if (Buffer.endsWith(PERSIAN_Tar)){
        //الحاق «با» و «بی» چسبیده به  صفت و ترکیبات «نا» تفضیلی مفرد
        Buffer.truncate(Buffer.size() - 2);
        if (this->CanAppendTar_Tarin.contains(Normalizer::fullTrim(Buffer)))
            return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + PERSIAN_Tar);
    }else if (Buffer.endsWith(PERSIAN_Tarin)){
        //الحاق «با» و «بی» چسبیده به  صفت و ترکیبات «نا» افضل مفرد
        Buffer.truncate(Buffer.size() - 4);
        if (this->CanAppendTar_Tarin.contains(Normalizer::fullTrim(Buffer)))
            return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + PERSIAN_Tarin);
    }
    return "";
}

}
}
}
