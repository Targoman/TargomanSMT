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
bool TextProcessor::init(const QString &_configFile) const
{
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
                                 quint32 _lineNo,
                                 bool _interactive,
                                 bool _useSpellCorrecter,
                                 QList<enuTextTags::Type> _removingTags) const
{
}

/**
 * @brief TextProcessor::text2RichIXML
 * @param _inStr
 * @return
 */
QString TextProcessor::text2RichIXML(const QString &_inStr) const
{
}

/**
 * @brief TextProcessor::ixml2Text
 * @param _ixml
 * @return
 */
QString TextProcessor::ixml2Text(const QString &_ixml) const
{
}

/**
 * @brief TextProcessor::richIXML2Text
 * @param _ixml
 * @return
 */
QString TextProcessor::richIXML2Text(const QString &_ixml) const
{
}

/**
 * @brief TextProcessor::normalizeText
 * @param _input
 * @return
 */
QString TextProcessor::normalizeText(const QString _input) const
{

}

}
}




