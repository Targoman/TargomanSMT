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

#include "TextProcessor.h"

#include "Private/Normalizer.h"
#include "Private/SpellCorrector.h"
#include "Private/IXMLWriter.h"
using namespace Targoman::NLPLibs::Private;

#include "ISO639.h"

namespace Targoman {
namespace NLPLibs {

TextProcessor* TextProcessor::Instance = NULL;
QString ActorUUID;

/**
 * @brief TextProcessor::TextProcessor
 */
TextProcessor::TextProcessor()
{
    TARGOMAN_REGISTER_ACTOR("TextProcessor");
}

/**
 * @brief TextProcessor::init
 * @param _configFile
 * @return
 */
bool TextProcessor::init(const stuConfigs& _configs) const
{
    if (Normalizer::instance().init(_configs.NormalizationFile) &&
            SpellCorrector::instance().init(_configs.SpellCorrector) &&
            IXMLWriter::instance().init(_configs.AbbreviationsFile))
        return false;
    else
        return false;
}

/**
 * @brief TextProcessor::text2IXML
 * @param _inStr
 * @param _interactive
 * @param _useSpellCorrecter
 * @param _removingTags
 * @return
 */
QString TextProcessor::text2IXML(const QString &_inStr,
                                 const QString& _lang,
                                 quint32 _lineNo,
                                 bool _interactive,
                                 bool _useSpellCorrecter,
                                 QList<enuTextTags::Type> _removingTags) const
{
    TargomanLogDebug(5,"ConvertToIXML Process Started");

    if(ISO639isValid(_lang.toAscii().constData()) == false)
        throw exTextProcessor("Invalid language code. It must be in ISO639 format");

    QString IXML = IXMLWriter::instance().convert2IXML(
                _inStr,
                ISO639getAlpha3B(_lang.toAscii().constData()),
                _lineNo,
                _interactive,
                _useSpellCorrecter);

   /* foreach(const QString& Tag, _removingTags)
        IXML.remove("<" + Tag + ">").remove("</"+Tag+">");*/
    TargomanDebug(7, "[NO-TAGS]"<<IXML);
    TargomanLogDebug(5,"ConvertToIXML Process Finished");
    return IXML;
}

/**
 * @brief TextProcessor::text2RichIXML
 * @param _inStr
 * @return
 */
QString TextProcessor::text2RichIXML(const QString &_inStr, const QString &_lang) const
{
    TargomanLogDebug(5,"ConvertToRichIXML Process Started");

    if(ISO639isValid(_lang.toAscii().constData()) == false)
        throw exTextProcessor("Invalid language code. It must be in ISO639 format");

    QString IXML = IXMLWriter::instance().convert2IXML(_inStr, ISO639getAlpha3B(_lang.toAscii().constData()));

    //TODO convert tags to rich tags

    TargomanDebug(7, "[NO-TAGS]"<<IXML);
    TargomanLogDebug(5,"ConvertToRichIXML Process Finished");
    return IXML;
}

/**
 * @brief TextProcessor::ixml2Text
 * @param _ixml
 * @return
 */
QString TextProcessor::ixml2Text(const QString &_ixml) const
{
    //TODO detokenize
}

/**
 * @brief TextProcessor::richIXML2Text
 * @param _ixml
 * @return
 */
QString TextProcessor::richIXML2Text(const QString &_ixml) const
{
    //TODO detokenize
}

/**
 * @brief TextProcessor::normalizeText
 * @param _input
 * @return
 */
QString TextProcessor::normalizeText(const QString _input, bool _interactive, const QString &_lang) const
{
    QString Output = Normalizer::instance().normalize(_input, _interactive);
    if (_lang.size()){
        if(ISO639isValid(_lang.toAscii().constData()) == false)
            throw exTextProcessor("Invalid language code. It must be in ISO639 format");
        Output =
                SpellCorrector::instance().process(ISO639getAlpha3B(_lang.toAscii().constData()), Output, _interactive);
    }

    return Output;
}

}
}




