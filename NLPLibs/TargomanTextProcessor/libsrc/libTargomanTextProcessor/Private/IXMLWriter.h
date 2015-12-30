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

#ifndef TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_CLSIXMLWRITER_H
#define TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_CLSIXMLWRITER_H

#include <QTextStream>
#include <QRegExp>

#include "../TextProcessor.h"
#include "Normalizer.h"
#include "SpellCorrector.h"
#include <functional>

namespace Targoman {
namespace NLPLibs {
namespace TargomanTP{
namespace Private {


static std::function<QString(const QString& _lang,
                          const QString& _token)> defaultTransformationFunction = [] (const QString&, const QString& ) {
    return QString();
};

TARGOMAN_ADD_EXCEPTION_HANDLER(exIXMLWriter, exTextProcessor);

/**
 * @brief The IXMLWriter class, provides some functions to convert input text to inline XML format.
 *
 * This function detects special contents of a text (like email adresses, abbreviations, dates, ...) and tags them in ixml format.
 * The main goal of this class is to distinguish stop-word dots from dots that are between letters of abreviations or dots that are after numbers in ordered lists.
 * Other functionalities and tags have lower importance for us.
 */
class IXMLWriter
{
public:

    /**
     * @brief Makes (if needed) and return an initialized instance of this class.
     */
    static IXMLWriter& instance(){return *(Q_LIKELY(Instance) ? Instance : (Instance = new IXMLWriter));}
    void init(const QString &_configFile);

    QString convert2IXML(const QString& _inStr,
                         INOUT bool& _spellCorrected,
                         const QString& _lang = "",
                         quint32 _lineNo = 0,
                         bool _interactive = false,
                         bool _useSpellCorrector = true);
    QString supportedSuffixes() const;

private:



    QString markByRegex(const QString &_phrase,
                        QRegExp _regex,
                        const QString &_mark,
                        QStringList *_listOfMatches,
                        quint8 _capID = 0);

    inline void replaceTag(QString& _output, enuTextTags::Type _type, const QString& _value, const QString& translationAttribte = ""){
        if(translationAttribte.isEmpty())
            _output.append(QString("<%1>%2</%1>").arg(enuTextTags::toStr(_type)).arg(_value));
        else
            _output.append(QString("<%1 Translation=%3>%2</%1>").arg(enuTextTags::toStr(_type)).arg(_value).arg(translationAttribte));
    }






private:
    IXMLWriter();
    Q_DISABLE_COPY(IXMLWriter)


private:
    static IXMLWriter*      Instance;       /**< static instance of this class */

    QTextStream* TempStream;
    QTextStream* InStream;
    QTextStream* FinalOutStream;

    QRegExp RxURL;                          /** A Regular expression to detect URLs*/
    QRegExp RxEmail;                        /** A Regular expression to detect email addresses*/

    QRegExp RxAbbr;
    QRegExp RxAbbrDotless;
    QRegExp RxAbbrDic;                      /** A set of Abbriviation which will be loaded from dictionaty */

    QRegExp RxMultiDots;

    QRegExp RxSuffix;

    QRegExp RxDate;
    QRegExp RxTime;
    QRegExp RxOrdinalNumber;
    QRegExp RxSpecialNumber;
    QRegExp RxDashSeparator;
    QRegExp RxUnderlineSeparator;
    QRegExp RxNumberLeft;
    QRegExp RxNumberRight;
    QRegExp RxNumbering;                    /** A Regular expression to detect any kind of number. It helps to see wethere it is number or not. More specific RegExp will be used to detect variation of numbers.*/
    QRegExp RxPersianLatin;
    QRegExp RxPersianNumber;
    QRegExp RxLatinPersian;
    QRegExp RxNumberValidator;              /** A Regular expression to detect URLs*/
    QRegExp RxURLValidator;                 /** A Regular expression to detect a line containing a URL*/

    Normalizer& NormalizerInstance;         /** An instance of Normalizer class for faster access */
    SpellCorrector& SpellCorrectorInstance; /** An instance of SpellCorrector class for faster access */
};

}
}
}
}

#endif // TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_CLSIXMLWRITER_H
