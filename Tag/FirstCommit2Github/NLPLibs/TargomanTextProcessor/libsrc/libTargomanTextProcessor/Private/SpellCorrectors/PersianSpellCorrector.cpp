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

#include <QStringList>

#include "PersianSpellCorrector.h"

namespace Targoman {
namespace NLPLibs {
namespace TargomanTP{
namespace Private {
namespace SpellCorrectors {

//Essential global instance in order to be registered in SpellCorrectors
PersianSpellCorrector Instance;

const QString PERSIAN_Mi         = QStringLiteral("می");
const QString PERSIAN_Nemi       = QStringLiteral("نمی");
const QString PERSIAN_Tar        = QStringLiteral("تر");
const QString PERSIAN_Tarin      = QStringLiteral("ترین");
const QString PERSIAN_Ba         = QStringLiteral("با");
const QString PERSIAN_Na         = QStringLiteral("نا");
const QString PERSIAN_Bi         = QStringLiteral("بی");
const QString PERSIAN_HeYe       = QStringLiteral("ه‌ی");
const QString PERSIAN_He         = QStringLiteral("ه");
const QString PERSIAN_An         = QStringLiteral("ان");
const QString PERSIAN_AlefYe     = QStringLiteral("ای");
const QString PERSIAN_Noon       = QStringLiteral("ن");
const QString PERSIAN_Ye         = QStringLiteral("ی");
//const QChar Fathatan             = QChar(0x64B);

PersianSpellCorrector::PersianSpellCorrector() :
    intfSpellCorrector("fa")
{
    this->Lang = "Persian";
    this->ConfigTypes.append(stuConfigType("AutoCorrectTerms",&this->AutoCorrectTerms));
    this->ConfigTypes.append(stuConfigType("StartWith_Bi_Ba",&this->CanStartWithBi_Ba));
    this->ConfigTypes.append(stuConfigType("StartWith_Na",&this->CanStartWithNa));
    this->ConfigTypes.append(stuConfigType("Space2ZWNJ",&this->Space2ZWNJ));
    this->ConfigTypes.append(stuConfigType("Nouns",&this->Nouns));
    this->ConfigTypes.append(stuConfigType("Adjectives",&this->Adjectives));
    this->ConfigTypes.append(stuConfigType("VerbStemPresent",&this->VerbStemPresent));
    this->ConfigTypes.append(stuConfigType("VerbStemPast",&this->VerbStemPast));
    this->ConfigTypes.append(stuConfigType("HamzeOrMadAllowed",&this->HamzeAllowed));
//    this->ConfigTypes.append(stuConfigType("AdverbsEndWithFathatan",&this->AdverbsEndWithFathatan));
}

/**
 * @brief Initializes ConfigTypes list and RegExp data members.
 */
bool PersianSpellCorrector::postInit(const QVariantHash _settings)
{
    Q_UNUSED(_settings)
    this->RxInteractiveChars    = QRegExp(QStringLiteral("[ؤئإأآ]"));
    this->RxPresentImperfect    = QRegExp(QStringLiteral("(م|ی|د|یم|ید|ند)$"));
    this->RxPastImperfect       = QRegExp(QStringLiteral("(م|ی|یم|ید|ند)$"));
    this->RxPastPerfect         = QRegExp(QStringLiteral("(بودم|بودی|بود|بودیم|بودید|بودند|باشم|باشی|باشد|باشیم|باشید|باشند)$"));
    this->RxVerbPerfect         = QRegExp(QStringLiteral("(ام|ای|است|ایم|اید|اند)$"));
    this->RxHa                  = QRegExp(QStringLiteral("ها‌?(یی?|یم|یت|یش|یمان|یتان|یشان)?$"));
    this->RxAn                  = QRegExp(QStringLiteral("ی?ان(ی|م|ت|ش|مان|تان|شان)?$"));
    this->RxPossesive           = QRegExp(QStringLiteral("[ای]?(م|ت|ش|مان|تان|شان)$"));
    this->RxEndWithHa           = QRegExp(".*" + this->RxHa.pattern());
    this->RxEndWithAn           = QRegExp(".*" + this->RxAn.pattern());
    this->RxEndPresentImperfect = QRegExp(".*" + this->RxPresentImperfect.pattern());
    this->RxEndPastImperfect    = QRegExp(".*" + this->RxPastImperfect.pattern());
    this->RxEndPastPerfect      = QRegExp(".*" + this->RxPastPerfect.pattern());
    this->RxEndVerbPerfect      = QRegExp(".*" + this->RxVerbPerfect.pattern());
    this->RxEndWithPossesive    = QRegExp(".*" + this->RxPossesive.pattern());
    return true;
}

/**
 * @brief This function process a list of tokens.
 * @param _tokens list of tokens.
 * @return returns a unified normalized string or an empty string.
 */

QString PersianSpellCorrector::process(const QStringList &_tokens)
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

    ComplexWord.truncate(ComplexWord.size() - 1); // to remove last extra ZWNJ.

    if(this->Nouns.contains(ComplexWord) || this->Adjectives.contains(ComplexWord))
        return ComplexWord;

    //removing final double qoute if it is written instead of fathatan.
    if(ComplexWord.endsWith('"')){
        Buffer = ComplexWord.mid(0, ComplexWord.length() - 1);
        if(this->AdverbsEndWithFathatan.contains(Buffer))
            return Buffer;
    }

    if(ComplexWord.endsWith(PERSIAN_HeYe))
        return ComplexWord.mid(0, ComplexWord.length() - 2); //zhnDebug: why do we do this?
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
        //جداسازی «بی،با،نا» چسبیده به صفت
        Buffer = ComplexWord;
        Prefix = Buffer.mid(0,2);
        Buffer.remove(0,2); //Remove Bi

        if (Prefix == PERSIAN_Na){
            Buffer = this->processStartingWithBi_Ba_Na(this->CanStartWithNa, Prefix, Buffer);
            if (Buffer.size())
                return Buffer;
        }else {
            Buffer = this->processStartingWithBi_Ba_Na(this->CanStartWithBi_Ba, Prefix, Buffer);
            if (Buffer.size())
                return Buffer;
        }
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
        if (Buffer.endsWith(PERSIAN_He) &&
             (this->Nouns.contains(Buffer) ||
            this->Adjectives.contains(Buffer))&&
                Postfix.startsWith(ARABIC_ALEF))
            return Normalizer::fullTrim(Buffer + ARABIC_ZWNJ + Postfix);

        QString NewBuffer = this->processHa("", Buffer, Postfix);
        if (NewBuffer.size())
            return NewBuffer;

        NewBuffer = this->processTar_Tarin(this->Adjectives, "", Buffer, Postfix);
        if(NewBuffer.size())
            return NewBuffer;
    }

    Buffer = this->processHa("",ComplexWord,"");
    if (Buffer.size())
        return Buffer;

    return this->processTar_Tarin(this->Adjectives, "", ComplexWord, "");
}

/**
 * @brief this function decide huristicly wethere input word is suspious for interactivly checking or not.
 * @param _inputWord input word.
 * @return true if suspicious else false.
 */

bool PersianSpellCorrector::canBeCheckedInteractive(const QString &_inputWord) const
{
    return false; //Temporarily ignored

    return _inputWord.contains(this->RxInteractiveChars) &&
            this->HamzeAllowed.contains(_inputWord) == false &&
            this->Nouns.contains(_inputWord) == false &&
            this->Adjectives.contains(_inputWord)  == false &&
            this->AutoCorrectTerms.values().contains(_inputWord) == false;
}

/**
 * @brief This function adds new terms to #AutoCorrectTerms variable.
 * @param _from wrong word that we want to be corrected automatically.
 * @param _to correct word.
 */
void PersianSpellCorrector::storeAutoCorrectTerm(const QString &_from, const QString &_to)
{
    this->AutoCorrectTerms.insert(_from, _to);
    /// @todo save to file
}


QString PersianSpellCorrector::processStartingWithBi_Ba_Na(const QSet<QString>& _set,
                                                              const QString &_prefix,
                                                              const QString &_postfix)
{
    QString Buffer = Normalizer::sidesTrim(_postfix);
    if (Buffer.isEmpty())
        return "";
    QString Postfix = Buffer;
    bool EndsWithHa = false;
    if (this->RxEndWithHa.exactMatch(Buffer) /*||
        this->RxEndWithAn.exactMatch(Buffer)*/){
        Buffer.remove(this->RxHa); //Remove combinations of Ha
        Postfix.remove(0,  Buffer.length());
        Buffer = Normalizer::sidesTrim(Buffer);
//        if(Postfix.isEmpty()){
//            Buffer = Normalizer::sidesTrim(_postfix);
//            Postfix = Buffer;
//            Buffer = Buffer.remove(this->RxAn); //Remove combinations of An
//            Postfix.remove(0, Buffer.length());
//            Buffer = Normalizer::sidesTrim(Buffer);
//        }else
//            EndsWithHa = true;

        if(!Postfix.isEmpty())
            EndsWithHa = true;

        Buffer = Normalizer::sidesTrim(Buffer);
        if (Postfix.size() &&
                _set.contains(Buffer)) //zhnDebug: ye nakareh should be removed first. has bug with بی شعوریهایشان
                                       //zhnDebug: why just match with _set? makes bugs for نامردهایشان
            return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + Postfix);

        QString OldBuffer = Buffer;
        Buffer = this->processTar_Tarin(_set, _prefix + ARABIC_ZWNJ, Buffer, Postfix, false);
        if(Buffer.size())
            return Buffer;
        else{
            if (EndsWithHa && OldBuffer.endsWith(ARABIC_YE)){
                Buffer = Normalizer::sidesTrim(OldBuffer.mid(0, OldBuffer.length() - 1));
                if (Postfix.size() &&
                        _set.contains(Buffer))
                    return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_YE + ARABIC_ZWNJ + Postfix);
            }
            Buffer = Normalizer::sidesTrim(_postfix);
            Postfix = Buffer;
            if (this->RxEndWithPossesive.exactMatch(Buffer)){
                Buffer.remove(this->RxPossesive); //Remove combinations of Possesive
                Postfix.remove(0,  Buffer.length());
                Buffer = Normalizer::sidesTrim(Buffer);
                if (Postfix.size() &&
                        _set.contains(Buffer))
                    return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + Buffer + ARABIC_ZWNJ + Postfix);
                Buffer = this->processTar_Tarin(_set, _prefix + ARABIC_ZWNJ, Buffer, Postfix, false);
                if(Buffer.size())
                    return Buffer;
            }
        }

    }

    Buffer = this->processTar_Tarin(_set, _prefix + ARABIC_ZWNJ, _postfix, "", false);
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
    if(_set.contains(Buffer))
        return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);

    return "";
}

QString PersianSpellCorrector::processVerbs(const QString &_prefix, const QString _postfix)
{
    QString Buffer = Normalizer::sidesTrim(_postfix);
    if (Buffer.isEmpty())
        return "";
    QString Postfix = Buffer;

    Buffer = Normalizer::sidesTrim(Buffer.remove(this->RxPresentImperfect));

    if(Buffer != Postfix && this->VerbStemPresent.contains(Buffer))
        return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
    Postfix = Normalizer::sidesTrim(Postfix.remove(0,Buffer.size()));

    if(Postfix.size())
    {
        // special if for verbs like می زند
        if (Postfix.startsWith(PERSIAN_Noon)){
            Buffer = Normalizer::sidesTrim(Buffer.append(PERSIAN_Noon));
            if(Buffer != Postfix && this->VerbStemPresent.contains(Buffer))
                return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
        }
        // special if for verbs like می گوید
        else if (Postfix.startsWith(PERSIAN_Ye)){
            Buffer = Normalizer::sidesTrim(Buffer.append(PERSIAN_Ye));
            if(Buffer != Postfix && this->VerbStemPresent.contains(Buffer))
                return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
        }
    }

    //افعال ماضی : می‌خوردم
    Buffer = Postfix = Normalizer::sidesTrim(_postfix);
    Buffer = Normalizer::sidesTrim(Buffer.remove(this->RxPastImperfect));
    if(this->VerbStemPast.contains(Buffer))
        return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);

    Postfix = Normalizer::sidesTrim(Postfix.remove(0,Buffer.size()));

    if(Postfix.size())
    {
        // special if for verbs like می زند
        if (Postfix.startsWith(PERSIAN_Noon)){
            Buffer = Normalizer::sidesTrim(Buffer.append(PERSIAN_Noon));
            if(this->VerbStemPresent.contains(Buffer))
                return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
        }
        // special if for verbs like می گوید
        else if (Postfix.startsWith(PERSIAN_Ye)){
            Buffer = Normalizer::sidesTrim(Buffer.append(PERSIAN_Ye));
            if(this->VerbStemPresent.contains(Buffer))
                return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
        }
    }



    //افعال ماضی  نقلی و استمراری : می‌خورده‌ام
    Buffer = Postfix = Normalizer::sidesTrim(_postfix);
    Buffer = Normalizer::sidesTrim(Buffer.remove(this->RxVerbPerfect));
    if (Buffer != Postfix && Buffer.size()){
        Buffer = Normalizer::sidesTrim(Buffer.remove(Buffer.size() - 1,1)); // remove final "He" from stem
        if(this->VerbStemPast.contains(Buffer))
            return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
    }

    //افعال ماضی بعید : می‌خورده بودم و می‌خورده باشم
    Buffer = Postfix = Normalizer::sidesTrim(_postfix);
    Buffer = Normalizer::sidesTrim(Buffer.remove(this->RxPastPerfect));
    if (Buffer != Postfix && Buffer.size()){
        Buffer = Normalizer::sidesTrim(Buffer.remove(Buffer.size() - 1,1)); // remove final "He" from stem
        if(this->VerbStemPast.contains(Buffer))
            return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
    }


    //حالت  خاص ماضی نقلی و استمراری : می خورده
    Buffer = Normalizer::sidesTrim(_postfix);
    if (Buffer.endsWith(PERSIAN_He)){
        Buffer = Normalizer::sidesTrim(Buffer.remove(Buffer.length() - 1, 1));
        if(this->VerbStemPast.contains(Buffer))
            return Normalizer::fullTrim(_prefix + ARABIC_ZWNJ + _postfix);
    }

    return "";
}

QString PersianSpellCorrector::processHa(const QString &_prefix,
                                            const QString &_complexWord,
                                            const QString &_postfix)
{
    QString Buffer = Normalizer::sidesTrim(_complexWord);
    if (Buffer.isEmpty())
        return "";
    QString Postfix = Buffer;
    Buffer.remove(this->RxHa);
    Postfix.remove(0, Buffer.length());
//    if(Postfix.isEmpty()){
//        Buffer = _complexWord;
//        Postfix = Buffer;
//        Buffer.remove(this->RxAn);
//        Postfix.remove(0, Buffer.length());
//    }
    if(Postfix.size()){
        Buffer = Normalizer::sidesTrim(Buffer);
        if (this->Nouns.contains(Buffer) ||
                this->Adjectives.contains(Buffer))
            return Normalizer::fullTrim(_prefix + Buffer + ARABIC_ZWNJ + Postfix + ARABIC_ZWNJ + _postfix);

        if (Buffer.endsWith(PERSIAN_He) && this->VerbStemPast.contains(Buffer.mid(0,Buffer.size() - 1)))
            return Normalizer::fullTrim(_prefix + Buffer + ARABIC_ZWNJ + Postfix + ARABIC_ZWNJ + _postfix);

        return this->processTar_Tarin(this->Adjectives, _prefix,Buffer,Postfix + ARABIC_ZWNJ + _postfix);
    }
    return "";
}

QString PersianSpellCorrector::processTar_Tarin(const QSet<QString>& _set,
                                                   const QString& _prefix,
                                                   const QString& _complexWord,
                                                   const QString& _postfix,
                                                   bool _checkVerb)
{
    QString Buffer = Normalizer::sidesTrim(_complexWord);
    if (Buffer.isEmpty())
        return "";
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
    if (_set.contains(Buffer))
        return Normalizer::fullTrim(_prefix + Buffer + ARABIC_ZWNJ +
                                    Type + ARABIC_ZWNJ + _postfix);
    if (_checkVerb &&  Buffer.endsWith(PERSIAN_He) && this->VerbStemPast.contains(Buffer.mid(0,Buffer.size() - 1)))
        return Normalizer::fullTrim(_prefix + Buffer + ARABIC_ZWNJ +
                                    Type + ARABIC_ZWNJ + _postfix);

    return "";
}

}
}
}
}
}
