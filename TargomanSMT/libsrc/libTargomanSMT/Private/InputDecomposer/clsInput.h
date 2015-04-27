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
#include "Private/Proxies/intfLMSentenceScorer.hpp"
#include "Private/InputDecomposer/clsToken.h"

#ifdef TARGOMAN_SHOW_DEBUG
#include <QTextStream>
#endif


namespace Targoman {
namespace SMT {
namespace Private {
/**
 *  @brief Input Decomposition module
 */
namespace InputDecomposer {

TARGOMAN_ADD_EXCEPTION_HANDLER(exInput, exTargomanCore);

TARGOMAN_DEFINE_ENHANCED_ENUM(enuDefaultAttrs,
                              NoShow,
                              NoDecode,
                              ShowSource,
                              Translation
                              );

typedef QList<clsToken> Sentence_t;

/**
 * @brief class clsInputDecomposer This class is defined to manage input string tokens.
 * This input string can be in ixml or plain format.
 */
class clsInputDecomposer
{
public:
    clsInputDecomposer(const QString& _inputStr);
    ~clsInputDecomposer(){}

    static void init(const QString &_configFilePath);


    static QString moduleName(){return "Input";}

public:
    inline const Sentence_t& tokens() const {return this->Tokens;}
    inline const QString& normalizedString() const {return this->NormalizedString;}


private:
    void parsePlain(const QString &_inputStr);
    void parseRichIXML(const QString& _inputIXML);
    void parseRichIXML(const QString& _inputIXML, bool _normalize);
    void newToken(const QString& _token,
                  const QString &_tagStr = "",
                  const QVariantMap &_attrs = QVariantMap());

    inline bool isSpace(const QChar& _ch){
        return _ch == ' ';
    }

private:
    Sentence_t Tokens;                                                                  /**< A list of clsToken class. Input string will be parsed and recorded in this structure. */
    static QSet<QString>    SpecialTags;                                                /**< List of valid tags. */
    QString                 NormalizedString;                                           /**< Normalized String when using plain text */

    //Configuration
    static Targoman::Common::Configuration::tmplConfigurable<QString> UserDefinedTags;  /**< Users can add their defined iXML tags to list of valid tags (#SpecialTags). */
    static Targoman::Common::Configuration::tmplConfigurable<bool>    IsIXML;           /**< A configurable to specify whether input string has inline xml or not. */
    static Targoman::Common::Configuration::tmplConfigurable<bool>    DoNormalize;      /**< A configurable to specify whether input string should be normalized or not. */
    static Targoman::Common::Configuration::tmplConfigurable<bool>    TagNameEntities;  /**< Use NER to tag name entities */

    friend class UnitTestNameSpace::clsUnitTest;
};

}
}
}
}

#ifdef TARGOMAN_SHOW_DEBUG
inline QTextStream& operator << (QTextStream& _outputStream, const Targoman::SMT::Private::InputDecomposer::Sentence_t& _sentence)
{
    if(_sentence.size() == 0)
        return _outputStream;
    _outputStream << _sentence.at(0);
    for(int i = 0; i < _sentence.size(); ++i)
        _outputStream << " " << _sentence.at(i);
    return _outputStream;
}
#endif

#endif // TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSINPUT_H
