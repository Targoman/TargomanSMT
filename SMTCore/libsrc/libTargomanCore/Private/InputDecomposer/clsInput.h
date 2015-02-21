/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSINPUT_H
#define TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSINPUT_H

#include <QList>
#include <QSet>

#include "Private/GlobalConfigs.h"
#include "libTargomanCommon/Types.h"
#include "Private/LanguageModel/intfLMSentenceScorer.hpp"
#include "Private/InputDecomposer/clsToken.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace InputDecomposer {

TARGOMAN_ADD_EXCEPTION_HANDLER(exInput, exTargomanCore);

typedef QList<clsToken> Sentence_t;

/**
 * @brief This class is defined to manage input string tokens.
 * This input string can be in ixml or plain format.
 */

class clsInput
{
public:
    clsInput(const QString& _inputStr);
    ~clsInput(){}

    static void init();

    inline const Sentence_t& tokens(){return this->Tokens;}

private:
    void parsePlain(const QString &_inputStr, const QString& _lang);
    void parseRichIXML(const QString& _inputIXML);
    void parseRichIXML(const QString& _inputIXML, bool _normalize, const QString &_lang = "");

    inline bool isSpace(const QChar& _ch){
        return _ch == ' ';
    }

private:
    Sentence_t Tokens;                                                                  /**< A list of clsToken class. Input string will be parsed and recorded in this structure. */
    static QSet<QString>    SpecialTags;                                                /**< List of valid tags. */

    //Configuration
    static Targoman::Common::Configuration::tmplConfigurable<QString> UserDefinedTags;  /**< Users can add their defined iXML tags to list of valid tags (#SpecialTags). */
    static Targoman::Common::Configuration::tmplConfigurable<bool>    IsIXML;           /**< A configurable to specify whether input string is xml or not. */
    static Targoman::Common::Configuration::tmplConfigurable<bool>    DoNormalize;      /**< A configurable to specify whether input string should be normalized or not. */
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSINPUT_H
