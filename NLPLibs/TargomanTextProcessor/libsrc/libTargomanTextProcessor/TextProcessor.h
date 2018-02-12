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

#ifndef TARGOMAN_NLPLIBS_TARGOMANTP_TEXTPROCESSOR_H
#define TARGOMAN_NLPLIBS_TARGOMANTP_TEXTPROCESSOR_H

#include <QString>
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Logger.h"
#include "libTargomanCommon/Types.h"
#include "ISO639.h" //From https://github.com/softnhard/ISO639

namespace Targoman {
namespace NLPLibs {

TARGOMAN_DEFINE_ENHANCED_ENUM(enuTextTags,
                              Number,
                              SpecialNumber,
                              Email,
                              URL,
                              Abbreviation,
                              OrderedListItem,
                              Time,
                              Date,
                              Ordinals,
                              Symbol
                              );

TARGOMAN_ADD_EXCEPTION_HANDLER(exTextProcessor, Targoman::Common::exTargomanBase);

struct stuIXMLReplacement{
    QRegExp SearchRegExp;
    QString AfterString;

    stuIXMLReplacement(const QRegExp& _searchRegExp = QRegExp(),
                       const QString& _afterString = QString()):
        SearchRegExp(_searchRegExp),
        AfterString(_afterString)
    {}
};

class TargomanTextProcessor
{
public:
    struct stuConfigs{
        QString NormalizationFile;
        QString AbbreviationsFile;
        QString SpellCorrectorBaseConfigPath;
        QHash<QString, QVariantHash> SpellCorrectorLanguageBasedConfigs;
    };

public:
    static inline TargomanTextProcessor& instance(){
        static TargomanTextProcessor* Instance = NULL;
        return *(Q_LIKELY(Instance) ? Instance : (Instance = new TargomanTextProcessor));
    }

    bool init(const stuConfigs &_configs);
    bool init(QSharedPointer<QSettings> _configSettings = QSharedPointer<QSettings>());

    QString text2IXML(const QString& _inStr,
                      INOUT bool &_spellCorrected,
                      const QString& _lang = "",
                      quint32 _lineNo = 0,
                      bool _interactive = true,
                      bool _useSpellCorrector = true,
                      QList<enuTextTags::Type> _removingTags = QList<enuTextTags::Type>(),
                      QList<stuIXMLReplacement> _replacements = QList<stuIXMLReplacement>()) const;

    QString ixml2Text(const QString& _ixml,
                      const QString& _lang = "",
                      bool _detokenize = true,
                      bool _hinidiDigits = true,
                      bool _breakSentences = true) const;

    inline QString normalizeText(const QString _input,
                                 bool _interactive,
                                 const QString &_lang) const{
        bool SpellCorrected;
        return this->normalizeText(_input, SpellCorrected, _interactive, _lang);
    }

    QString normalizeText(const QString _input,
                          INOUT bool &_spellCorrected,
                          bool _interactive = false,
                          const QString& _lang = "") const;

private:
    TargomanTextProcessor();
    Q_DISABLE_COPY(TargomanTextProcessor)
};


}
}

#endif // TARGOMAN_NLPLIBS_TARGOMANTP_TEXTPROCESSOR_H
