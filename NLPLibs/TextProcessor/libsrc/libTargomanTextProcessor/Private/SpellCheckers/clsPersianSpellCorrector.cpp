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

namespace Targoman {
namespace NLPLibs {
namespace Private {

const QString PERSIAN_Mi         = QString::fromUtf8("می");
const QString PERSIAN_Nemi       = QString::fromUtf8("نمی");
const QString PERSIAN_Tar        = QString::fromUtf8("تر");
const QString PERSIAN_Tarin      = QString::fromUtf8("ترین");
const QString PERSIAN_Ba         = QString::fromUtf8("با");
const QString PERSIAN_Na         = QString::fromUtf8("نا");
const QString PERSIAN_Bi         = QString::fromUtf8("بی");
const QString PERSIAN_HeYe       = QString::fromUtf8("ه‌ی");
const QString PERSIAN_He         = QString::fromUtf8("ه");
const QString PERSIAN_An         = QString::fromUtf8("ان");

clsPersianSpellCorrector::clsPersianSpellCorrector()
{
    this->Lang                  = "Persian";
    this->ConfigSteps.insert("AutoCorrectTerms",stuConfigStep(&this->AutoCorrectTerms));
    this->ConfigSteps.insert("StartWith_Bi_Ba",stuConfigStep(&this->CanStartWithBi_Ba));
    this->ConfigSteps.insert("StartWith_Na",stuConfigStep(&this->CanStartWithNa));
    this->ConfigSteps.insert("Space2ZWNJ",stuConfigStep(&this->Space2ZWNJ));
    this->ConfigSteps.insert("Nouns",stuConfigStep(&this->Nouns));
    this->ConfigSteps.insert("Adjectives",stuConfigStep(&this->Adjectives));
    this->ConfigSteps.insert("VerbStemPresent",stuConfigStep(&this->VerbStemPresent));
    this->ConfigSteps.insert("VerbStemPast",stuConfigStep(&this->VerbStemPast));
    this->ConfigSteps.insert("HamzeAllowed",stuConfigStep(&this->HamzeAllowed));

    this->RxInteractiveChars    = QRegExp(QString::fromUtf8("[ؤئإأآ]"));
    this->RxPresentImperfect = QRegExp(QString::fromUtf8("(م|ی|د|یم|ید|ند)$"));
    this->RxPastImperfect    = QRegExp(QString::fromUtf8("(م|ی|یم|ید|ند)$"));
    this->RxVerbPerfect         = QRegExp(QString::fromUtf8("(ام|ای|است|ایم|اید|اند)$"));
    this->RxHa                  = QRegExp(QString::fromUtf8("ها)ی|یم|یت|یش|یمان|یتان|یشان(?"));
    this->RxAn                  = QRegExp(QString::fromUtf8("ان)ی|م|ت|ش|مان|تان|شان(?"));
    this->RxEndWithHa           = QRegExp(".*" + this->RxHa.pattern() + "$");
    this->RxEndWithAn           = QRegExp(".*" + this->RxAn.pattern() + "$");
    this->RxEndPresentImperfect = QRegExp(".*" + this->RxPresentImperfect.pattern());
    this->RxEndPastImperfect    = QRegExp(".*" + this->RxPastImperfect.pattern());
    this->RxEndVerbPerfect      = QRegExp(".*" + this->RxVerbPerfect.pattern());
}

bool clsPersianSpellCorrector::postInit(const QVariantHash _settings)
{
    Q_UNUSED(_settings)
    return true;
}

QString clsPersianSpellCorrector::process(const QStringList &_tokens)
{
    QString Buffer, Prefix, Postfix;

    Buffer = this->AutoCorrectTerms.value(_tokens.join(" "));
    if (Buffer.size())
        return Normalizer::instance().normalize(Buffer);

    if (this->Space2ZWNJ.contains(_tokens.join(" ")))
        return Normalizer::instance().normalize(_tokens.join(ARABIC_ZWNJ));

    switch(_tokens.size()){
    case 1:
        if(_tokens.first().endsWith(PERSIAN_HeYe))
                return _tokens.first().mid(0, _tokens.first().length() - 2);
        else if (_tokens.first().startsWith(PERSIAN_Mi)){
            //جداسازی «می» چسبیده به فعل
            return this->processVerbs(_tokens.first().mid(0,2), _tokens.first().mid(2));
        }else if (_tokens.first().startsWith(PERSIAN_Nemi)){
            //جداسازی «نمی» چسبیده به فعل
            return this->processVerbs(_tokens.first().mid(0,3), _tokens.first().mid(3));
        }else if(_tokens.first().startsWith(PERSIAN_Bi)){
            //جداسازی «بی» چسبیده به صفت
            Buffer = _tokens.first();
            Prefix = Buffer.mid(0,2);
            Buffer.remove(0,2); //Remove Bi

            return this->processStartingWithBi_Ba_Na(this->CanStartWithBi_Ba, Prefix, Buffer);
        }else if (this->RxEndVerbPerfect.exactMatch(_tokens.first())){
            Buffer = _tokens.first();
            Postfix = Buffer;
            Buffer.remove(RxVerbPerfect);
            Postfix.remove(0, Postfix.length() - Buffer.length());
            if(Postfix.size() &&
                    Buffer.endsWith(PERSIAN_He) &&
                    this->VerbStemPast.contains(Buffer.mid(0, Buffer.size() - 1)))
                return Normalizer::instance().normalize(Buffer + ARABIC_ZWNJ + Postfix);
        }else{
            Buffer = _tokens.first();
            Postfix = Buffer;
            Buffer.remove(RxHa);
            Postfix.remove(0, Postfix.length() - Buffer.length());
            if(Postfix.isEmpty()){
                Buffer = _tokens.first();
                Postfix = Buffer;
                Buffer.remove(RxAn);
                Postfix.remove(0, Postfix.length() - Buffer.length());
            }
            if(Postfix.size()){
                //جداسازی ترکیبات «ها» در صورت وجود
                if (this->Nouns.contains(Normalizer::fullTrim(Buffer)) ||
                    this->Adjectives.contains(Normalizer::fullTrim(Buffer)))
                    return Normalizer::instance().normalize(Buffer + ARABIC_ZWNJ + Postfix);
                if (Buffer.endsWith(PERSIAN_Tar)){
                    //جداسازی صفت تفضیلی جمع
                    Buffer.truncate(Buffer.size() - 2);
                    if (this->Adjectives.contains(Normalizer::fullTrim(Buffer)))
                        return Normalizer::instance().normalize(
                                    Buffer + ARABIC_ZWNJ + PERSIAN_Tar + ARABIC_ZWNJ + Postfix);
                }else if (Buffer.endsWith(PERSIAN_Tarin)){
                    //جداسازی صفت افضل جمع
                    Buffer.truncate(Buffer.size() - 4);
                    if (this->Adjectives.contains(Normalizer::fullTrim(Buffer)))
                        return Normalizer::instance().normalize(
                                    Buffer + ARABIC_ZWNJ + PERSIAN_Tar + ARABIC_ZWNJ + Postfix);
                }
            }else if (Buffer.endsWith(PERSIAN_Tar)){
                //جداسازی صفت تفضیلی مفرد
                Buffer.truncate(Buffer.size() - 2);
                if (this->Adjectives.contains(Normalizer::fullTrim(Buffer)))
                    return Normalizer::instance().normalize(Buffer + ARABIC_ZWNJ + PERSIAN_Tar);
            }else if (Buffer.endsWith(PERSIAN_Tarin)){
                //جداسازی صفت افضل مفرد
                Buffer.truncate(Buffer.size() - 4);
                if (this->Adjectives.contains(Normalizer::fullTrim(Buffer)))
                    return Normalizer::instance().normalize(Buffer + ARABIC_ZWNJ + PERSIAN_Tar);
            }
        }
    case 2:
        if (_tokens.first() == PERSIAN_Mi){
            //چسباندن «می» قبل از افعال

            return this->processVerbs(_tokens.first(), _tokens.last());
        }else if (_tokens.first() == PERSIAN_Nemi){
            //چسباندن «نمی» قبل از افعال
            return this->processVerbs(_tokens.first(), _tokens.last());
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
            if(this->Nouns.contains(_tokens.first()) ||
                    this->Adjectives.contains(_tokens.first()))
                return Normalizer::instance().normalize(_tokens.first() + ARABIC_ZWNJ + _tokens.last());

            if (_tokens.first().startsWith(PERSIAN_Bi) || _tokens.first().startsWith(PERSIAN_Ba)){
                Buffer = this->processStartingWithBi_Ba_Na(
                            this->CanStartWithBi_Ba, _tokens.first().mid(0,2), _tokens.first().mid(2));
                if (Buffer.size())
                    return Normalizer::instance().normalize(Buffer + ARABIC_ZWNJ + _tokens.last());
            }
        }else if (this->RxAn.exactMatch(Normalizer::fullTrim(_tokens.last()))){
            if(this->Nouns.contains(_tokens.first()) ||
                    this->Adjectives.contains(_tokens.first()))
                return Normalizer::instance().normalize(_tokens.first() + ARABIC_ZWNJ + _tokens.last());

            if (_tokens.first().startsWith(PERSIAN_Bi) || _tokens.first().startsWith(PERSIAN_Ba)){
                Buffer = this->processStartingWithBi_Ba_Na(
                            this->CanStartWithBi_Ba, _tokens.first().mid(0,2), _tokens.first().mid(2));
                if (Buffer.size())
                    return Normalizer::instance().normalize(Buffer + ARABIC_ZWNJ + _tokens.last());
            }
        }
    }

    return "";
}

bool clsPersianSpellCorrector::canBeCheckedInteractive(const QString &_inputWord) const
{
    return _inputWord.contains(this->RxInteractiveChars) &&
            this->HamzeAllowed.contains(_inputWord) == false &&
            this->Nouns.contains(_inputWord) == false &&
            this->Adjectives.contains(_inputWord)  == false &&
            this->AutoCorrectTerms.values().contains(_inputWord) == false;
}

void clsPersianSpellCorrector::storeAutoCorrectTerm(const QString &_from, const QString &_to)
{
    this->AutoCorrectTerms.insert(_from, _to);
    //TODO save to file
}

QString clsPersianSpellCorrector::processStartingWithBi_Ba_Na(const QSet<QString>& _set,
                                                              const QString &_prefix,
                                                              const QString &_postfix)
{
    QString Buffer = _postfix;
    QString Postfix = Buffer;
    if (RxEndWithHa.exactMatch(Normalizer::fullTrim(Buffer)) ||
            RxEndWithAn.exactMatch(Normalizer::fullTrim(Buffer))){
        //الحاق «با» و «بی» چسبیده به صفت و ترکیبات «نا» جمع
        Buffer.remove(RxHa); //Remove combinations of Ha
        Postfix.remove(0, Postfix.length() - Buffer.length());
        if(Postfix.isEmpty()){
            Buffer = _postfix;
            Postfix = Buffer;
            Buffer.remove(RxAn); //Remove combinations of An
            Postfix.remove(0, Postfix.length() - Buffer.length());
        }

        if (_set.contains(Normalizer::fullTrim(Buffer)))
            return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + Postfix);

        if (Buffer.endsWith(PERSIAN_Tar)){
            //الحاق «با» و «بی» چسبیده به  صفت و ترکیبات «نا» تفضیلی جمع
            Buffer.truncate(Buffer.size() - 2);
            if (this->Adjectives.contains(Normalizer::fullTrim(Buffer)))
                return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + PERSIAN_Tar + ARABIC_ZWNJ + Postfix);
        }else if (Buffer.endsWith(PERSIAN_Tarin)){
            //الحاق «با» و «بی» چسبیده به  صفت و ترکیبات «نا» افضل جمع
            Buffer.truncate(Buffer.size() - 4);
            if (this->Adjectives.contains(Normalizer::fullTrim(Buffer)))
                return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + PERSIAN_Tarin + ARABIC_ZWNJ + Postfix);
        }
    }else if (RxEndWithAn.exactMatch(Normalizer::fullTrim(Buffer))){
        //الحاق «با» و «بی» چسبیده به صفت و ترکیبات «نا» جمع
        Buffer.remove(RxAn); //Remove combinations of Ha
        Postfix.remove(0, Postfix.length() - Buffer.length());

        if (_set.contains(Normalizer::fullTrim(Buffer)))
            return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + Postfix);

        if (Buffer.endsWith(PERSIAN_Tar)){
            //الحاق «با» و «بی» چسبیده به  صفت و ترکیبات «نا» تفضیلی جمع
            Buffer.truncate(Buffer.size() - 2);
            if (this->Adjectives.contains(Normalizer::fullTrim(Buffer)))
                return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + PERSIAN_Tar + ARABIC_ZWNJ + Postfix);
        }else if (Buffer.endsWith(PERSIAN_Tarin)){
            //الحاق «با» و «بی» چسبیده به  صفت و ترکیبات «نا» افضل جمع
            Buffer.truncate(Buffer.size() - 4);
            if (this->Adjectives.contains(Normalizer::fullTrim(Buffer)))
                return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + PERSIAN_Tarin + ARABIC_ZWNJ + Postfix);
        }
    }else if (Buffer.endsWith(PERSIAN_Tar)){
        //الحاق «با» و «بی» چسبیده به  صفت و ترکیبات «نا» تفضیلی مفرد
        Buffer.truncate(Buffer.size() - 2);
        if (this->Adjectives.contains(Normalizer::fullTrim(Buffer)))
            return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + PERSIAN_Tar);
    }else if (Buffer.endsWith(PERSIAN_Tarin)){
        //الحاق «با» و «بی» چسبیده به  صفت و ترکیبات «نا» افضل مفرد
        Buffer.truncate(Buffer.size() - 4);
        if (this->Adjectives.contains(Normalizer::fullTrim(Buffer)))
            return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + PERSIAN_Tarin);
    }
    return "";
}

QString clsPersianSpellCorrector::processVerbs(const QString &_prefix, const QString _postfix)
{
    //افعال حال : می‌خورم
    QString Buffer = _postfix;
    Buffer.remove(this->RxPresentImperfect);
    if(this->VerbStemPresent.contains(Normalizer::fullTrim(Buffer)))
        return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + _postfix);

    //افعال ماضی : می‌خوردم
    Buffer = _postfix;
    Buffer.remove(this->RxPastImperfect);
    if(this->VerbStemPast.contains(Normalizer::fullTrim(Buffer)))
        return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + _postfix);

    //افعال ماضی  نقلی و استمراری : می‌خورده‌ام
    Buffer = _postfix;
    Buffer.remove(this->RxVerbPerfect);
    if (Buffer.size()){
        Buffer.remove(Buffer.size() - 1,1); // remove final "He" from stem
        if(this->VerbStemPresent.contains(Normalizer::fullTrim(Buffer)))
            return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + _postfix);
    }

    //افعال ماضی بعید : می‌خورده بودم و می‌خورده باشم
    Buffer = _postfix;
    if (Buffer.endsWith(PERSIAN_He)){
        Buffer.remove(Buffer.length() - 1, 1);
        if(this->VerbStemPast.contains(Normalizer::fullTrim(Buffer)))
            return Normalizer::instance().normalize(_prefix + ARABIC_ZWNJ + _postfix);
    }

    return "";
}

}
}
}
