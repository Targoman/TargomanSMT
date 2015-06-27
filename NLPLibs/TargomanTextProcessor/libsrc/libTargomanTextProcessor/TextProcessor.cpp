/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 *@author S. Mohammad M. Ziabary <smm@ziabary.com>
 *@author Saeed Torabzadeh <Saeed.torabzadeh@gmail.com>
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
    Initialized = true;
    return true;
}

/**
 * @brief TextProcessor::init Initialization method used in combination with TargomanStyle configurations
 * @return
 */
bool TargomanTextProcessor::init(const QString _configFile)
{
    stuConfigs MyConfigs;
    MyConfigs.AbbreviationsFile = TargomanTP::Private::Configs.AbbreviationFile.value();
    MyConfigs.NormalizationFile = TargomanTP::Private::Configs.NormalizationFile.value();
    MyConfigs.SpellCorrectorBaseConfigPath = TargomanTP::Private::Configs.SpellCorrectorBaseConfigPath.value();

    if (_configFile.size()){
        QSettings ConfigFile(_configFile, QSettings::IniFormat);

        ConfigFile.beginGroup(TargomanTP::Private::Configs.SpellCorrectorLanguageBasedConfigs.configPath());
        foreach(const QString& Lang, ConfigFile.childGroups()){
            ConfigFile.beginGroup(Lang);
            foreach (const QString& Key, ConfigFile.allKeys())
                MyConfigs.SpellCorrectorLanguageBasedConfigs[Lang].insert(Key, ConfigFile.value(Key));
            ConfigFile.endGroup();
        }
        ConfigFile.endGroup();
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
                                 const QString& _lang,
                                 quint32 _lineNo,
                                 bool _interactive,
                                 bool _useSpellCorrector,
                                 QList<enuTextTags::Type> _removingTags) const
{
    if (!Initialized)
        throw exTextProcessor("Text Processor has not been initialized");
    TargomanLogDebug(7,"ConvertToIXML Process Started");

    const char* LangCode = ISO639getAlpha2(_lang.toLatin1().constData());

    QString IXML = IXMLWriter::instance().convert2IXML(
                _inStr,
                LangCode ? LangCode : "",
                false,
                _lineNo,
                _interactive,
                _useSpellCorrector);

    foreach(enuTextTags::Type Tag, _removingTags)
        IXML.remove(
                    QString("<%1>").arg(enuTextTags::toStr(Tag))).remove(
                    QString("</%1>").arg(enuTextTags::toStr(Tag)));

    TargomanDebug(7, "[REM-TAGS] |"<<IXML<<"|");
    TargomanLogDebug(7,"ConvertToIXML Process Finished");
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
 * @param _lang An ISO639 Language code. If provided input phrase will be spell checked
 * @return Normalized phrase
 */
QString TargomanTextProcessor::normalizeText(const QString _input, bool _interactive, const QString &_lang) const
{
    QString Output = Normalizer::instance().normalize(_input, _interactive);
    if (_lang.size()){
        const char* LangCode = ISO639getAlpha2(_lang.toLatin1().constData());
        Output = SpellCorrector::instance().process(LangCode ? LangCode : "", Output, _interactive);
    }

    return Normalizer::fullTrim(Output);
}

}
}

