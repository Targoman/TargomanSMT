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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_CLSIXMLWRITER_H
#define TARGOMAN_NLPLIBS_PRIVATE_CLSIXMLWRITER_H

#include <QTextStream>
#include <QRegExp>

#include "../TextProcessor.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exIXMLWriter, exTextProcessor);

class IXMLWriter
{
public:
    static IXMLWriter& instance(){return *(Q_LIKELY(Instance) ? Instance : (Instance = new IXMLWriter));}
    void init(const QString &_configFile);

    QString convert2IXML(const QString& _inStr,
                         const QString& _lang = "",
                         quint32 _lineNo = 0,
                         bool _interactive = false,
                         bool _useSpellCorrector = true);

private:
    QString markByRegex(const QString &_phrase,
                        QRegExp &_regex,
                        const QString &_mark,
                        QStringList *_listOfMatches,
                        quint8 _capID = 0);

    inline void replaceTag(QString& _output, enuTextTags::Type _type, const QString& _value){
        _output.append(QString("<%1>%2</%1>").arg(enuTextTags::toStr(_type)).arg(_value));
    }


private:
    IXMLWriter();

private:
    static IXMLWriter*      Instance;

    QTextStream* TempStream;
    QTextStream* InStream;
    QTextStream* FinalOutStream;

    QRegExp RxURL;
    QRegExp RxEmail;

    QRegExp RxAbbr;
    QRegExp RxAbbrDotless;
    QRegExp RxAbbrDic;

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
    QRegExp RxNumbering;
    QRegExp RxPersianLatin;
    QRegExp RxPersianNumber;
    QRegExp RxLatinPersian;
    QRegExp RxNumberValidator;
    QRegExp RxURLValidator;
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_CLSIXMLWRITER_H
