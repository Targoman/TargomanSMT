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
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
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
                              Translation,
                              DefaultTranslation
                              );

typedef QList<clsToken> Sentence_t;

/**
 * @brief class clsInputDecomposer This class is defined to manage input string tokens.
 * This input string can be in ixml or plain format.
 */
class clsInput
{
public:
    clsInput(const QString& _inputStr, bool _isIXML);
    ~clsInput(){}

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
    // TODO: Add a configuration that selects wether to keep the inside of IXML tags in translation or whatever!

    friend class UnitTestNameSpace::clsUnitTest;
};

}
}
}
}

inline QTextStream& operator << (QTextStream& _outputStream, const Targoman::SMT::Private::InputDecomposer::Sentence_t& _sentence)
{
    if(_sentence.size() == 0)
        return _outputStream;
    _outputStream << _sentence.at(0);
    for(int i = 0; i < _sentence.size(); ++i)
        _outputStream << " " << _sentence.at(i);
    return _outputStream;
}

#endif // TARGOMAN_CORE_PRIVATE_INPUTDECOMPOSER_CLSINPUT_H
