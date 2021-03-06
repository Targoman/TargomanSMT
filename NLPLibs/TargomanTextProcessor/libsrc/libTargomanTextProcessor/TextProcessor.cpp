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

#include "TextProcessor.h"

#include "Private/Normalizer.h"
#include "Private/SpellCorrector.h"
#include "Private/IXMLWriter.h"
#include "Private/Configs.h"
#include <QSettings>
using namespace Targoman::NLPLibs::TargomanTP::Private;

#include "ISO639.h"

namespace Targoman {
namespace NLPLibs {

bool Initialized = false;


/**
 * @brief TextProcessor::TextProcessor
 */
TargomanTextProcessor::TargomanTextProcessor()
{}

/**
 * @brief TextProcessor::init
 * @param _configFile
 * @return
 */
bool TargomanTextProcessor::init(const stuConfigs& _configs)
{
    if (Initialized)
        return true;
    Normalizer::instance().init(_configs.NormalizationFile);
    SpellCorrector::instance().init(_configs.SpellCorrectorBaseConfigPath, _configs.SpellCorrectorLanguageBasedConfigs);
    IXMLWriter::instance().init(_configs.AbbreviationsFile);
    ISO639init();
    Initialized = true;
    return true;
}

/**
 * @brief TextProcessor::init Initialization method used in combination with TargomanStyle configurations
 * @return
 */
bool TargomanTextProcessor::init(QSharedPointer<QSettings> _configSettings)
{
    stuConfigs MyConfigs;
    MyConfigs.AbbreviationsFile = TargomanTP::Private::Configs.AbbreviationFile.value();
    MyConfigs.NormalizationFile = TargomanTP::Private::Configs.NormalizationFile.value();
    MyConfigs.SpellCorrectorBaseConfigPath = TargomanTP::Private::Configs.SpellCorrectorBaseConfigPath.value();

    if (_configSettings.isNull() == false){
        _configSettings->beginGroup(TargomanTP::Private::Configs.SpellCorrectorLanguageBasedConfigs.configPath());
        foreach(const QString& Lang, _configSettings->childGroups()){
            foreach (const QString& Key, _configSettings->allKeys()){
                _configSettings->beginGroup(Lang);
                MyConfigs.SpellCorrectorLanguageBasedConfigs[Lang].insert(
                            Key,_configSettings->value(Key));
                _configSettings->endGroup();
            }
        }
        _configSettings->endGroup();
    }
    return this->init(MyConfigs);
}

/**
 * @brief TextProcessor::text2IXML
 * @param _inStr
 * @param _interactive
 * @param _useSpellCorrector
 * @param _removingTags
 * @return
 */
QString TargomanTextProcessor::text2IXML(const QString &_inStr,
                                         INOUT bool& _spellCorrected,
                                         const QString& _lang,
                                         quint32 _lineNo,
                                         bool _interactive,
                                         bool _useSpellCorrector,
                                         QList<enuTextTags::Type> _removingTags,
                                         QList<stuIXMLReplacement> _replacements) const
{
    if (!Initialized)
        throw exTextProcessor("Text Processor has not been initialized");
    TargomanDebug(7,"ConvertToIXML Process Started");

    const char* LangCode = ISO639getAlpha2(_lang.toLatin1().constData());

    QString IXML = IXMLWriter::instance().convert2IXML(
                _inStr,
                _spellCorrected,
                LangCode ? LangCode : "",
                _lineNo,
                _interactive,
                _useSpellCorrector);

    foreach(const stuIXMLReplacement& Replacement, _replacements)
        IXML.replace(Replacement.SearchRegExp, Replacement.AfterString);

    foreach(enuTextTags::Type Tag, _removingTags)
        IXML.remove(
                    QString("<%1>").arg(enuTextTags::toStr(Tag))).remove(
                    QString("</%1>").arg(enuTextTags::toStr(Tag)));

    TargomanDebug(6, "[REM-TAGS] |"<<IXML<<"|");
    TargomanDebug(7,"ConvertToIXML Process Finished");
    return IXML;
}



QStringList getIXMLLines(QString& _data)
{
    _data = _data.replace (". .", "..");
    _data = _data.replace (". .", "..");
    _data = _data.replace ("  ", " ");
    _data = _data.replace ("  ", " ");

    _data = _data.replace (" . \"", " TGMN_DOT\"");
    _data = _data.replace (" . )" , " TGMN_DOT)");
    _data = _data.replace (" . ]" , " TGMN_DOT]");
    _data = _data.replace (" . }" , " TGMN_DOT}");

    _data = _data.replace (" ? \"", " TGMN_QM\"");
    _data = _data.replace (" ? )" , " TGMN_QM)");
    _data = _data.replace (" ? ]" , " TGMN_QM]");
    _data = _data.replace (" ? }" , " TGMN_QM}");

    _data = _data.replace (" ! \"", " TGMN_EM\"\n");
    _data = _data.replace (" ! )" , " TGMN_EM)\n");
    _data = _data.replace (" ! ]" , " TGMN_EM]\n");
    _data = _data.replace (" ! }" , " TGMN_EM}\n");

    _data = _data.replace (" . &gt;", " .&gt;");
    _data = _data.replace ("  ", " ");
    _data = _data.replace ("  ", " ");
    _data = _data.replace (" . ", " .\n");
    _data = _data.replace (" ? ", " ?\n");
    _data = _data.replace (" ! ", " !\n");

    _data = _data.replace (" TGMN_DOT\"", " .\"\n");
    _data = _data.replace (" TGMN_DOT)" , " .)\n");
    _data = _data.replace (" TGMN_DOT]" , " .]\n");
    _data = _data.replace (" TGMN_DOT]" , " .}\n");

    _data = _data.replace (" TGMN_QM\"", " ? \"\n");
    _data = _data.replace (" TGMN_QM)", " ? )\n");
    _data = _data.replace (" TGMN_QM]", " ? ]\n");
    _data = _data.replace (" TGMN_QM}", " ? }\n");

    _data = _data.replace (" TGMN_EM\"\n", " ! \"\n");
    _data = _data.replace (" TGMN_EM)\n", " ! )\n");
    _data = _data.replace (" TGMN_EM]\n", " ! ]\n");
    _data = _data.replace (" TGMN_EM}\n", " ! }\n");

     return _data.split ('\n', QString::SkipEmptyParts);
}


/**
 * @brief TextProcessor::ixml2Text
 * @param _ixml
 * @return
 */
QString TargomanTextProcessor::ixml2Text(const QString &_ixml, const QString& _lang, bool _detokenize, bool _hinidiDigits, bool _breakSentences) const
{
    if (!Initialized)
        throw exTextProcessor("Text Processor has not been initialized");
    const char* LangCode = ISO639getAlpha2(_lang.toLatin1().constData());

    thread_local static QRegExp RxSuffixes = QRegExp(
                QString("(?: )('[%1])(?: )").arg(IXMLWriter::instance().supportedSuffixes()));

    thread_local static QRegExp RxDetokenDQuote = QRegExp("(?:(?: |^)\" )([^\"]+)(?: \"(?: |$))");
    thread_local static QRegExp RxDetokenQuote  = QRegExp("(?:(?: |^)\\' )([^\\']+)(?: \\'(?: |$))");
    thread_local static QRegExp RxAllIXMLTags =
        QRegExp(
                QString("<%1>").arg(enuTextTags::options().join(">|<")) +
                QString("|</%1>").arg(enuTextTags::options().join(">|</"))
                );


    QString IXML = _ixml;
    QStringList Lines = getIXMLLines (IXML);
    for (int i = 0; i < Lines.count (); ++i)
    {
        //remove first spaces
        while(Lines[i].size() && Lines[i].at(0) == ' ')
            Lines[i].remove(0,1);

        Lines[i] = Lines[i].replace (RxSuffixes, "\\1 ");
        Lines[i] = Lines[i].replace (RxAllIXMLTags,"");

        if (_detokenize){
            int Pos=0;
            while ((Pos=RxDetokenDQuote.indexIn(Lines[i], 0)) != -1) {
                Lines[i]=
                        Lines[i].mid(0,Pos) +
                        " \"" + RxDetokenDQuote.cap(1) + "\" " +
                        Lines[i].mid(Pos + RxDetokenDQuote.matchedLength());
            }

            Pos=0;
            while ((Pos=RxDetokenQuote.indexIn(Lines[i], 0)) != -1) {
                Lines[i]=
                        Lines[i].mid(0,Pos) +
                        " '" + RxDetokenQuote.cap(1) + "' " +
                        Lines[i].mid(Pos + RxDetokenQuote.matchedLength());
            }
        }
        Lines[i] = Lines[i].replace ("&gt;", ">");
        Lines[i] = Lines[i].replace ("&lt;", "<");
        Lines[i] = Lines[i].replace ("&amp;", "&");

        if (_detokenize){
            Lines[i] = Lines[i].replace ("  ", " ");
            Lines[i] = Lines[i].replace ("  ", " ");
            Lines[i] = Lines[i].replace (" .", ".");
            Lines[i] = Lines[i].replace (" ,", ",");
            Lines[i] = Lines[i].replace (" ;", ";");
            Lines[i] = Lines[i].replace (" :", ":");
            Lines[i] = Lines[i].replace (" ?", "?");
            Lines[i] = Lines[i].replace (" !", "!");
            Lines[i] = Lines[i].replace (" )", ")");
            Lines[i] = Lines[i].replace (") ", ")");
            Lines[i] = Lines[i].replace ("( ", "(");
        }

        if (_hinidiDigits && (LangCode && (!strcmp(LangCode, "fa") || !strcmp(LangCode,"ar")))){
            static QString ArabicCharacters=QStringLiteral("۰۱۲۳۴۵۶۷۸۹؟؛،");
            for (int j=0; j<Lines[i].size(); ++j){
                switch(Lines[i][j].unicode()){
                case '0': Lines[i][j]=ArabicCharacters.at(0);break;
                case '1': Lines[i][j]=ArabicCharacters.at(1);break;
                case '2': Lines[i][j]=ArabicCharacters.at(2);break;
                case '3': Lines[i][j]=ArabicCharacters.at(3);break;
                case '4': Lines[i][j]=ArabicCharacters.at(4);break;
                case '5': Lines[i][j]=ArabicCharacters.at(5);break;
                case '6': Lines[i][j]=ArabicCharacters.at(6);break;
                case '7': Lines[i][j]=ArabicCharacters.at(7);break;
                case '8': Lines[i][j]=ArabicCharacters.at(8);break;
                case '9': Lines[i][j]=ArabicCharacters.at(9);break;
                case '?': Lines[i][j]=ArabicCharacters.at(10);break;
                case ';': Lines[i][j]=ArabicCharacters.at(11);break;
                case ',': Lines[i][j]=ArabicCharacters.at(12);break;
                }
            }
        }
    }
    if (_breakSentences)
        return Lines.join("\n");
    else
        return Lines.join(" ");
}

/**
 * @brief TextProcessor::normalizeText Normalizes based on normalization rules. It will also correct miss-spells if
 *        _lang is provided
 * @param _input Input phrase to be normalized
 * @param _interactive In interactive mode new entries can be learnt
 * @param _lang An ISO639 Language code. If provided input phrase will be spell corrected
 * @return Normalized phrase
 */
QString TargomanTextProcessor::normalizeText(const QString _input,
                                             bool &_spellCorrected,
                                             bool _interactive,
                                             const QString &_lang) const
{
    QString Output = Normalizer::instance().normalize(_input, _interactive);
    if (_lang.size()){
        const char* LangCode = ISO639getAlpha2(_lang.toLatin1().constData());
        Output = SpellCorrector::instance().process(LangCode ? LangCode : "",
                                                    Output,
                                                    _spellCorrected,
                                                    _interactive);
    }

    return Normalizer::fullTrim(Output);
}

}
}

