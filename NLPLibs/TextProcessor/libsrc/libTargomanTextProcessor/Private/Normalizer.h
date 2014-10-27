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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_NORMALIZER_H
#define TARGOMAN_NLPLIBS_PRIVATE_NORMALIZER_H

#include <QHash>
#include <QSet>
#include <QStringList>
#include <QVariant>

#include "../TextProcessor.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exNormalizer, exTextProcessor);

TARGOMAN_DEFINE_ENHANCED_ENUM_BEGIN(enuDicType)
    WhiteList,
    ReplacingCharacter,
    RemovingCharcters,
    SpaceCharacters,
    ZeroWidthSpaceCharacters,
    NotSure,
    EndOfFile
TARGOMAN_DEFINE_ENHANCED_ENUM_STRINGS
    "WhiteList",
    "ReplacingCharacters",
    "RemovingCharacters",
    "SpaceCharacters",
    "ZeroWidthSpaceCharacters",
    "NotSure",
    "EOF"
TARGOMAN_DEFINE_ENHANCED_ENUM_END

#define ARABIC_ZWNJ                QChar(0x200C)
#define ARABIC_YE                  QChar(0x06CC)
#define ARABIC_YE_HAMZA            QChar(0x0626)
#define ARABIC_ALEF_HAMZA_DOWN     QChar(0x0625)
#define ARABIC_ALEF_HAMZA_UP       QChar(0x0622)
#define ARABIC_ALEF                QChar(0x0627)

#define MULTI_DOT       QString::fromUtf8("…")
#define SYMBOL_REMOVED  QChar(0x2205)

class Normalizer
{
public:
    void init(const QString& _configFile, bool _binaryMode = false);
    static Normalizer& instance(){return *(Q_LIKELY(Instance) ? Instance : (Instance = new Normalizer));}

    QString normalize(const QChar& _char,
                      const QChar& _nextChar,
                      bool _interactive,
                      quint32 _line,
                      const QString& _phrase,
                      size_t _charPos,
                      bool _skipRecheck = false);

    QString normalize(const QString& _string, qint32 _line = -1, bool _interactive = false);

    void updateBinTable(const QString& _binFilePath, bool _interactive = false);

    static QString fullTrim(const QString& _str);


private:
    Normalizer();
    Q_DISABLE_COPY(Normalizer)

    void add2Configs(enuDicType::Type _type, QChar _originalChar, QChar _replacement = QChar());
    QString char2Str(const QChar &_char, bool _hexForced = false);
    QList<QChar> str2QChar(QString _str, quint16 _line, bool _allowRange = true);

private:
    static Normalizer*      Instance;
    QHash<QChar,QString>    ReplacingTable;
    QList<QVariant>         BinTable;
    QSet<QChar>             WhiteList;
    QSet<QChar>             RemovingList;
    QSet<QChar>             SpaceCharList;
    QSet<QChar>             ZeroWidthSpaceCharList;
    QString                 ConfigFile;
    bool                    BinaryMode;
    QChar                   LastChar;
    QStringList             NormalizedDiacritics;
    QString                 PersianChars;
    QRegExp                 RxDetokenDQuote;
    QRegExp                 RxDetokenQuote;
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_NORMALIZER_H
