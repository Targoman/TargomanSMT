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

QString ActorUUID;
bool Initialized = false;


/**
 * @brief TextProcessor::TextProcessor
 */
TargomanTextProcessor::TargomanTextProcessor()
{
    TARGOMAN_REGISTER_ACTOR("TextProcessor");
}

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
                                         QList<enuTextTags::Type> _removingTags) const
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

    foreach(enuTextTags::Type Tag, _removingTags)
        IXML.remove(
                    QString("<%1>").arg(enuTextTags::toStr(Tag))).remove(
                    QString("</%1>").arg(enuTextTags::toStr(Tag)));

    TargomanDebug(6, "[REM-TAGS] |"<<IXML<<"|");
    TargomanDebug(7,"ConvertToIXML Process Finished");
    return IXML;
}




/**
 * @brief TextProcessor::ixml2Text
 * @param _ixml
 * @return
 */
QString TargomanTextProcessor::ixml2Text(const QString &_ixml) const
{
    if (!Initialized)
        throw exTextProcessor("Text Processor has not been initialized");

    Q_UNUSED(_ixml)
    //TODO detokenize
    return "";
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

