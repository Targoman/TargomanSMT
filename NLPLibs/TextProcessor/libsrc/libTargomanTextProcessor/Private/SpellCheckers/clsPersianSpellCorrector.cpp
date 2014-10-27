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
const QString PERSIAN_AlefYe     = QString::fromUtf8("ای");
const QString PERSIAN_Noon       = QString::fromUtf8("ن");

clsPersianSpellCorrector::clsPersianSpellCorrector()
{
    this->Lang                  = "Persian";
    this->ConfigTypes.append(stuConfigType("AutoCorrectTerms",&this->AutoCorrectTerms));
    this->ConfigTypes.append(stuConfigType("StartWith_Bi_Ba",&this->CanStartWithBi_Ba));
    this->ConfigTypes.append(stuConfigType("StartWith_Na",&this->CanStartWithNa));
    this->ConfigTypes.append(stuConfigType("Space2ZWNJ",&this->Space2ZWNJ));
    this->ConfigTypes.append(stuConfigType("Nouns",&this->Nouns));
    this->ConfigTypes.append(stuConfigType("Adjectives",&this->Adjectives));
    this->ConfigTypes.append(stuConfigType("VerbStemPresent",&this->VerbStemPresent));
    this->ConfigTypes.append(stuConfigType("VerbStemPast",&this->VerbStemPast));
    this->ConfigTypes.append(stuConfigType("HamzeOrMadAllowed",&this->HamzeAllowed));

    this->RxInteractiveChars    = QRegExp(QString::fromUtf8("[ؤئإأآ]"));
    this->RxPresentImperfect    = QRegExp(QString::fromUtf8("(م|ی|د|یم|ید|ند)$"));
    this->RxPastImperfect       = QRegExp(QString::fromUtf8("(م|ی|یم|ید|ند)$"));
    this->RxVerbPerfect         = QRegExp(QString::fromUtf8("(ام|ای|است|ایم|اید|اند)$"));
    this->RxHa                  = QRegExp(QString::fromUtf8("ها(یی?|یم|یت|یش|یمان|یتان|یشان)?$"));
    this->RxAn                  = QRegExp(QString::fromUtf8("ی?ان(ی|م|ت|ش|مان|تان|شان)?$"));
    this->RxPossesive           = QRegExp(QString::fromUtf8("[ای]?(م|ت|ش|مان|تان|شان)$"));
    this->RxEndWithHa           = QRegExp(".*" + this->RxHa.pattern());
    this->RxEndWithAn           = QRegExp(".*" + this->RxAn.pattern());
    this->RxEndPresentImperfect = QRegExp(".*" + this->RxPresentImperfect.pattern());
    this->RxEndPastImperfect    = QRegExp(".*" + this->RxPastImperfect.pattern());
    this->RxEndVerbPerfect      = QRegExp(".*" + this->RxVerbPerfect.pattern());
    this->RxEndWithPossesive    = QRegExp(".*" + this->RxPossesive.pattern());
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
        return Normalizer::fullTrim(Buffer);

    if (this->Space2ZWNJ.contains(_tokens.join(" ")))
        return Normalizer::fullTrim(_tokens.join(ARABIC_ZWNJ));

    QString ComplexWord;

    for (int i=0; i<_tokens.size(); i++){
        ComplexWord += _tokens.at(i) + ARABIC_ZWNJ;
    }

    ComplexWord.truncate(ComplexWord.size() - 1);

    if(ComplexWord.endsWith(PERSIAN_HeYe))
        return ComplexWord.mid(0, ComplexWord.length() - 2);
    if (ComplexWord.startsWith(PERSIAN_Mi)){
        //جداسازی «می» چسبیده به فعل
        Buffer = this->processVerbs(ComplexWord.mid(0,2), ComplexWord.mid(2));
        if (Buffer.size())
            return Buffer;
    }

    if (ComplexWord.startsWith(PERSIAN_Nemi)){
        //جداسازی «نمی» چسبیده به فعل
        Buffer = this->processVerbs(ComplexWord.mid(0,3), ComplexWord.mid(3));
        if (Buffer.size())
            return Buffer;
    }

    if(ComplexWord.startsWith(PERSIAN_Bi) ||
       ComplexWord.startsWith(PERSIAN_Ba) ||
       ComplexWord.startsWith(PERSIAN_Na)){
        //جداسازی «بی» چسبیده به صفت
        Buffer = ComplexWord;
        Prefix = Buffer.mid(0,2);
        Buffer.remove(0,2); //Remove Bi

        Buffer = this->processStartingWithBi_Ba_Na(this->CanStartWithBi_Ba, Prefix, Buffer);
        if (Buffer.size())
            return Buffer;
    }

    if (this->RxEndVerbPerfect.exactMatch(ComplexWord)){
        Buffer = ComplexWord;
        Postfix = Buffer;
        Buffer.remove(RxVerbPerfect);
        Postfix.remove(0, Buffer.length());
        Buffer = Normalizer::sidesTrim(Buffer);
        if(Postfix.size()){
            if(Buffer.endsWith(PERSIAN_He) &&
                    this->VerbStemPast.contains(Buffer.mid(0, Buffer.size() - 1)))
                return Normalizer::fullTrim(Buffer + ARABIC_ZWNJ + Postfix);
            if (this->VerbStemPast.contains(Buffer))
                return Normalizer::fullTrim(Buffer + ARABIC_ZWNJ + Postfix);
        }

        if (ComplexWord.endsWith(PERSIAN_AlefYe)){
            Buffer = ComplexWord;
            Buffer.truncate(Buffer.size() - 2);
            Buffer = Normalizer::sidesTrim(Buffer);
            if (Buffer.endsWith(PERSIAN_He) && (
                        this->Adjectives.contains(Buffer) ||
                        this->Nouns.contains(Buffer)) &&
                    this->Nouns.contains(Buffer + ARABIC_ALEF) == false &&
                    this->Adjectives.contains(Buffer + ARABIC_ALEF) == false)
                return Normalizer::fullTrim(Buffer + ARABIC_ZWNJ + PERSIAN_AlefYe);
        }

        Buffer = this->processHa("",ComplexWord,"");
        if (Buffer.size())
            return Buffer;
    }

    Buffer = ComplexWord;
    Postfix = Buffer;
    Buffer.remove(this->RxPossesive);
    Postfix.remove(0, Buffer.length());
    Buffer = Normalizer::sidesTrim(Buffer);
    if(Postfix.size()){
        //جداسازی ضمیر مالکیت مفرد
        if (this->Nouns.contains(Buffer) ||
            this->Adjectives.contains(Buffer))
            return Normalizer::fullTrim(Buffer + ARABIC_ZWNJ + Postfix);

        QString NewBuffer = this->processHa("", Buffer, Postfix);
        if (NewBuffer.size())
            return NewBuffer;

        NewBuffer = this->processTar_Tarin("", Buffer, Postfix);
        if(NewBuffer.size())
            return NewBuffer;
    }

    Buffer = this->processHa("",ComplexWord,"");
    if (Buffer.size())
        return Buffer;

    return this->processTar_Tarin("", ComplexWord, "");
}

bool clsPersianSpellCorrector::canBeCheckedInteractive(const QString &_inputWord) const
{
    return false; //Temporarily ignored

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
    QString Buffer = Normalizer::sidesTrim(_postfix);
    QString Postfix = Buffer;
    Buffer = Normalizer::sidesTrim(Buffer);
    if (RxEndWithHa.exactMatch(Buffer) ||
            RxEndWithAn.exactMatch(Buffer)){
        Buffer.remove(RxHa); //Remove combinations of Ha
        Postfix.remove(0,  Buffer.length());
        if(Postfix.isEmpty()){
            Buffer = _postfix;
            Postfix = Buffer;
            Buffer.remove(RxAn); //Remove combinations of An
            Postfix.remove(0, Buffer.length());
        }

        Buffer = Normalizer::sidesTrim(Buffer);
        if (Postfix.size() &&
                _set.contains(Buffer))
            return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + Postfix);

        Buffer = this->processTar_Tarin(_prefix + ARABIC_ZWNJ, Buffer, Postfix);
        if(Buffer.size())
            return Buffer;
    }

    Buffer = this->processTar_Tarin(_prefix + ARABIC_ZWNJ, _postfix, "");
    if(Buffer.size())
        return Buffer;

    Buffer = Normalizer::sidesTrim(_postfix);
    if (Buffer.endsWith(ARABIC_YE)){
        //نکره صفت
        Buffer.truncate(Buffer.size() - 1);
        if (this->CanStartWithBi_Ba.contains(Buffer))
            return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_YE);
    }

    Buffer = Normalizer::sidesTrim(_postfix);
    if (_prefix == PERSIAN_Na){
        if (this->CanStartWithNa.contains(Buffer))
            return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
    }else if (this->CanStartWithBi_Ba.contains(Buffer))
        return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);

    return "";
}

QString clsPersianSpellCorrector::processVerbs(const QString &_prefix, const QString _postfix)
{
    QString Buffer = Normalizer::sidesTrim(_postfix);
    QString Postfix = Buffer;
    Buffer = Normalizer::sidesTrim(Buffer.remove(this->RxPresentImperfect));

    if(this->VerbStemPresent.contains(Buffer))
        return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
    Postfix = Normalizer::sidesTrim(Postfix.remove(0,Buffer.size()));
    if (Postfix.size() && Postfix.startsWith(PERSIAN_Noon)){
        Buffer = Normalizer::sidesTrim(Buffer.append(PERSIAN_Noon));
        if(this->VerbStemPresent.contains(Buffer))
            return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
    }

    //افعال ماضی : می‌خوردم
    Buffer = Postfix = _postfix;
    Buffer = Normalizer::sidesTrim(Buffer.remove(this->RxPastImperfect));
    if(this->VerbStemPast.contains(Buffer))
        return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);

    Postfix.remove(0,Buffer.size());
    if (Postfix.size() && Postfix.startsWith(PERSIAN_Noon)){
        Buffer = Normalizer::sidesTrim(Buffer.append(PERSIAN_Noon));
        if(this->VerbStemPast.contains(Buffer))
            return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
    }

    //افعال ماضی  نقلی و استمراری : می‌خورده‌ام
    Buffer = Postfix = _postfix;
    Buffer.remove(this->RxVerbPerfect);
    if (Buffer.size()){
        Buffer = Normalizer::sidesTrim(Buffer.remove(Buffer.size() - 1,1)); // remove final "He" from stem
        if(this->VerbStemPresent.contains(Buffer))
            return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
    }

    //افعال ماضی بعید : می‌خورده بودم و می‌خورده باشم
    Buffer = _postfix;
    if (Buffer.endsWith(PERSIAN_He)){
        Buffer = Normalizer::sidesTrim(Buffer.remove(Buffer.length() - 1, 1));
        if(this->VerbStemPast.contains(Buffer))
            return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
    }

    return "";
}

QString clsPersianSpellCorrector::processHa(const QString &_prefix, const QString &_complexWord, const QString &_postfix)
{
    QString Buffer = Normalizer::sidesTrim(_complexWord);
    QString Postfix = Buffer;
    Buffer.remove(this->RxHa);
    Postfix.remove(0, Buffer.length());
    if(Postfix.isEmpty()){
        Buffer = _complexWord;
        Postfix = Buffer;
        Buffer.remove(this->RxAn);
        Postfix.remove(0, Buffer.length());
    }
    if(Postfix.size()){
        Buffer = Normalizer::sidesTrim(Buffer);
        if (this->Nouns.contains(Buffer) ||
                this->Adjectives.contains(Buffer))
            return Normalizer::fullTrim(_prefix + Buffer + ARABIC_ZWNJ + Postfix + ARABIC_ZWNJ + _postfix);

        if (Buffer.endsWith(PERSIAN_He) && this->VerbStemPast.contains(Buffer.mid(0,Buffer.size() - 1)))
            return Normalizer::fullTrim(_prefix + Buffer + ARABIC_ZWNJ + Postfix + ARABIC_ZWNJ + _postfix);

        return this->processTar_Tarin(_prefix,Buffer,Postfix + ARABIC_ZWNJ + _postfix);
    }
    return "";
}

QString clsPersianSpellCorrector::processTar_Tarin(const QString& _prefix,
                                                   const QString& _complexWord,
                                                   const QString& _postfix)
{
    QString Buffer = Normalizer::sidesTrim(_complexWord);
    QString Type;
    if (Buffer.endsWith(PERSIAN_Tar)){
        Buffer.truncate(Buffer.size() - 2);
        Type = PERSIAN_Tar;
    } else if (Buffer.endsWith(PERSIAN_Tarin)){
        Buffer.truncate(Buffer.size() - 4);
        Type = PERSIAN_Tarin;
    }
    else
        return "";


    Buffer = Normalizer::sidesTrim(Buffer);
    if (this->Adjectives.contains(Buffer))
        return Normalizer::fullTrim(_prefix + Buffer + ARABIC_ZWNJ +
                                    Type + ARABIC_ZWNJ + _postfix);
    if (Buffer.endsWith(PERSIAN_He) && this->VerbStemPast.contains(Buffer.mid(0,Buffer.size() - 1)))
        return Normalizer::fullTrim(_prefix + Buffer + ARABIC_ZWNJ +
                                    Type + ARABIC_ZWNJ + _postfix);

    return "";
}

}
}
}
