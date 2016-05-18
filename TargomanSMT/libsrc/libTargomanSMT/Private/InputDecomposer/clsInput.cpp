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

#include <QStringList>
#include "libTargomanTextProcessor/TextProcessor.h"
#include "clsInput.h"
#include "Private/SpecialTokenHandler/OOVHandler/OOVHandler.h"
#include "Private/SpecialTokenHandler/IXMLTagHandler/IXMLTagHandler.h"
#include "Private/Proxies/NamedEntityRecognition/intfNamedEntityRecognizer.h"

using Targoman::NLPLibs::TargomanTextProcessor;

namespace Targoman {
namespace SMT {
namespace Private {
namespace InputDecomposer {

using namespace Common;
using namespace SpecialTokenHandler::OOV;
using namespace SpecialTokenHandler::IXMLTagHandler;
using namespace Targoman::SMT::Private::Proxies::NamedEntityRecognition;

QSet<QString>    clsInput::SpecialTags;

Configuration::tmplConfigurable<QString>  clsInput::UserDefinedTags(
        MAKE_CONFIG_PATH("UserDefinedTags"),
        "Valid user defined XML tags that must be stored with their attributes."
        "These must not overlap with predefined XML Tags",
        ""
        /* TODO add lambda to check overlap*/);
Configuration::tmplConfigurable<QString>  clsInput::TagSeparator(
        MAKE_CONFIG_PATH("TagSeparator"),
        "Separator string between tags.",
        ";:;");

Configuration::tmplConfigurable<bool>    clsInput::IsIXML(
        MAKE_CONFIG_PATH("IsIXML"),
        "Input is in Plain text(default) or IXML format",
        false);
Configuration::tmplConfigurable<bool>    clsInput::DoNormalize(
        MAKE_CONFIG_PATH("DoNormalize"),
        "Normalize Input(default) or let it unchanged",
        true);

Configuration::tmplConfigurable<bool>    clsInput::TagNamedEntities(
        MAKE_CONFIG_PATH("TagNamedEntities"),
        "Use NER to tag name entities",
        false);

/**
 * @brief clsInput::clsInput Instructor of this class gets input string and based on input arguments parses that.
 * @param _inputStr input string.
 */
clsInput::clsInput(const QString &_inputStr, bool _isIXML)
{
    if (_isIXML || clsInput::IsIXML.value()) {
        if (clsInput::DoNormalize.value())
            this->parseRichIXML(_inputStr,true);
        else
            this->parseRichIXML(_inputStr);
    }else{
        this->parsePlain(_inputStr);
    }
#ifndef SMT
    if(clsInput::TagNamedEntities.value()) {
        intfNamedEntityRecognizer* NER = gConfigs.NER.getInstance<intfNamedEntityRecognizer>();
        NER->tagNamedEntities(this->TokenInfoList);
    }
#endif
    this->makeSentence();
}
/**
 * @brief clsInput::init This function inserts userdefined and default tags to #SpecialTags.
 */
void clsInput::init(QSharedPointer<QSettings> _configSettings)
{
    if (clsInput::IsIXML.value() == false || clsInput::DoNormalize.value())
        TargomanTextProcessor::instance().init(_configSettings);

    if (UserDefinedTags.value().size())
        foreach(const QString& Tag, UserDefinedTags.value().split(clsInput::TagSeparator.value()))
            clsInput::SpecialTags.insert(Tag);
    for (int i=0; i<Targoman::NLPLibs::enuTextTags::getCount(); i++)
        clsInput::SpecialTags.insert(Targoman::NLPLibs::enuTextTags::toStr((Targoman::NLPLibs::enuTextTags::Type)i));

#ifndef SMT
    if (clsInput::TagNamedEntities.value() == true){
        Proxies::NamedEntityRecognition::intfNamedEntityRecognizer* NER =
                gConfigs.NER.getInstance<Proxies::NamedEntityRecognition::intfNamedEntityRecognizer>();
        NER->init(_configSettings);
    }
#endif

}

/**
 * @brief clsInput::parsePlain This function fisrt converts plain text to iXML format and then parses that.
 * @param _inputStr Input string
 */

void clsInput::parsePlain(const QString &_inputStr)
{
    this->NormalizedString =
            TargomanTextProcessor::instance().normalizeText(_inputStr, false, gConfigs.SourceLanguage.value());
    bool SpellCorrectorChanges;
    this->parseRichIXML(
                TargomanTextProcessor::instance().text2IXML(_inputStr,
                                                            SpellCorrectorChanges,
                                                            gConfigs.SourceLanguage.value(), 0, false), false);
}


void clsInput::parseRichIXML(const QString &_inputIXML, bool _normalize)
{
    if (_normalize){
        this->parseRichIXML(
                    TargomanTextProcessor::instance().normalizeText(_inputIXML,
                                                                    false,
                                                                    gConfigs.SourceLanguage.value()));
    }else
        this->parseRichIXML(_inputIXML);
}


/**
 * @brief clsInput::parseRichIXML parses iXML input string and adds detected tokens and their additional informations to #Tokens list.
 * @param _inputIXML Input string.
 */

void clsInput::parseRichIXML(const QString &_inputIXML)
{
    if (_inputIXML.contains('<') == false) {
      foreach(const QString& Token, _inputIXML.split(" ", QString::SkipEmptyParts))
          this->newTokenInfo(Token);
      return;
    }

    enum enuParsingState{
        Look4Open,
        XMLOpened,
        CollectAttrName,
        Looking4AttrValue,
        CollectAttrValue,
        CollectXMLText,
        Look4Closed,
        XMLClosed,
    }ParsingState = Look4Open;


    QString Token;
    QString TagStr;
    QString TempStr;
    QString AttrName;
    QString AttrValue;
    QVariantMap Attributes;
    bool NextCharEscaped = false;
    int Index = 0;

    foreach(const QChar& Ch, _inputIXML){
        Index++;
        switch(ParsingState){
        case Look4Open:
            if (Ch == '<'){
                if (NextCharEscaped)
                    Token.append(Ch);
                else
                    ParsingState = XMLOpened;
                NextCharEscaped = false;
                continue;
            }
            NextCharEscaped = false;
            if (this->isSpace(Ch)){
                this->newTokenInfo(Token);
                Token.clear();
            }else if (Ch == '\\'){
                NextCharEscaped = true;
                Token.append(Ch);
            }else
                Token.append(Ch);
            break;

        case XMLOpened:
            if (this->isSpace(Ch)){
                if (this->SpecialTags.contains(TagStr) == false)
                    throw exInput("Unrecognized Tag Name: <" + TagStr+">");
                ParsingState = CollectAttrName;
            }else if (Ch == '>'){
                if (this->SpecialTags.contains(TagStr) == false)
                    throw exInput("Unrecognized Tag Name: <" + TagStr+">");
                ParsingState = CollectXMLText;
            }else if(Ch.isLetter())
                TagStr.append(Ch);
            else
                throw exInput("Inavlid character '"+QString(Ch)+"' at index: "+ QString::number(Index));
            break;
        case CollectAttrName:
             if (this->isSpace(Ch))
                 continue; //skip spaces untill attrname
             else if (Ch == '=')
                 ParsingState = Looking4AttrValue;
             else if (Ch == '>')
                 ParsingState = CollectXMLText; //No new attribute so collext XML text
             else if (Ch.isLetter())
                 AttrName.append(Ch);
             else
                 throw exInput("Inavlid character '"+QString(Ch)+"' at index: "+ QString::number(Index));
             break;
        case Looking4AttrValue:
            if (this->isSpace(Ch))
                continue; //skip spaces unitl attr value started
            else if (Ch == '"')
                ParsingState = CollectAttrValue;
            else //Short XML tags <b/> are invalid as XML text is obligatory
                throw exInput("Inavlid character '"+QString(Ch)+"' at index: "+ QString::number(Index));
            break;
        case CollectAttrValue:
            if (Ch == '"'){
                if (NextCharEscaped)
                    AttrValue.append(Ch);
                else{
                    if (Attributes.contains(AttrName))
                        throw exInput("Attribute: <"+AttrName+"> Was defined later.");
                    Attributes.insert(AttrName, AttrValue);
                    AttrName.clear();
                    AttrValue.clear();
                    ParsingState = CollectAttrName;
                }
                NextCharEscaped = false;
                continue;
            }
            NextCharEscaped = false;
            if (Ch == '\\')
                NextCharEscaped = true;
            AttrValue.append(Ch);
            break;
        case CollectXMLText:
            if (Ch == '<'){
                if (NextCharEscaped)
                    Token.append(Ch);
                else
                    ParsingState = Look4Closed;
                NextCharEscaped = false;
                continue;
            }
            NextCharEscaped = false;
            if (Ch == '\\')
                NextCharEscaped = true;
            Token.append(Ch);
            break;

        case Look4Closed:
            if (Ch == '/')
                ParsingState = XMLClosed;
            else
                throw exInput("Inavlid character '"+QString(Ch)+"' at index: "+ QString::number(Index)+" it must be '/'");
            break;
        case XMLClosed:
            if (this->isSpace(Ch))
                continue; //skip Until end of tag
            else if (Ch == '>'){
                if (TempStr != TagStr)
                    throw exInput("Invalid closing tag: <"+TempStr+"> while looking for <"+TagStr+">");
                this->newTokenInfo(Token, TagStr, Attributes);

                Token.clear();
                TempStr.clear();
                TagStr.clear();
                Attributes.clear();
                AttrName.clear();
                AttrValue.clear();
                ParsingState = Look4Open;
            }else if (Ch.isLetter())
                TempStr.append(Ch);
            else
                throw exInput("Inavlid character '"+QString(Ch)+"' at index: "+ QString::number(Index));
        }
    }

    if(Token.size()){
        this->newTokenInfo(Token);
        Token.clear();
    }

    switch(ParsingState){
    case Look4Open:
        return;
    case XMLOpened:
    case CollectAttrName:
    case CollectXMLText:
    case Look4Closed:
    case XMLClosed:
        throw exInput("XML Tag: <"+TagStr+"> has not been closed");
    case Looking4AttrValue:
        throw exInput("XML Tag: <"+TagStr+"> Attribute: <"+AttrName+"> has no value");
    case CollectAttrValue:
        throw exInput("XML Tag: <"+TagStr+"> Attribute: <"+AttrName+"> value not closed");
    }
}

/**
 * @brief This function finds wordIndex of input (token or tag string) then inserts token, tag string, attributes and word index to #Tokens.
 *
 * If token is wrapped with a tag, word index of that tag will be find and the token string is not important in finding the word index.
 * If word index of token can not be found using SourceVocab, OOVHandler helps us to allocate a word index for that OOV word.
 * If OOVHandler inform us that this unknown token should be ignored in decoding process we don't add #Tokens.
 *
 * @note: SourceVocab has already been filled in loading phrase table phase.
 * @param _token Input token
 * @param _tagStr If token is wrapped with a tag, this argument inserts string of tag to the function.
 * @param _attrs If token is wrapped with a tag and tag has some attributes, this argument inserts keys and value of those attributes.
 */
void clsInput::newTokenInfo(const QString &_token,
                            const QString &_tagStr,
                            const QVariantMap &_attrs){
    if (_token.isEmpty())
        return;
    this->TokenInfoList.append(clsToken::stuInfo(_token, _tagStr, _attrs));
}


void clsInput::makeSentence()
{
    this->Tokens.clear();
    for(clsToken::stuInfo& TokenInfo : this->TokenInfoList) {
        QList<WordIndex_t> WordIndexes;

        if (TokenInfo.TagStr.size() )
            WordIndexes = IXMLTagHandler::instance().getWordIndexOptions(
                        TokenInfo.TagStr, TokenInfo.Str, TokenInfo.Attrs
                        );
        auto qVariantMapToString = [] (const QVariantMap& _map) {
                QStringList Result;
                for(auto Iter = _map.begin(); Iter != _map.end(); ++Iter)
                    Result.append( QString("[%1]=%2").arg(Iter.key()).arg(Iter.value().toString()) );
                return "(" + Result.join(",") + ")";
        };

        if (TokenInfo.Attrs.value(enuDefaultAttrs::toStr(enuDefaultAttrs::NoDecode)).isValid())
            return; // User Or IXMLTagHandler says that I must ignore this word when decoding

        if (WordIndexes.isEmpty()){
            WordIndex_t WordIndex = gConfigs.SourceVocab.value(
                        TokenInfo.Str, Constants::SrcVocabUnkWordIndex);
            if (WordIndex == Constants::SrcVocabUnkWordIndex ||
                    gConfigs.VocabWithoutSingleWordRule.contains(TokenInfo.Str)){
                WordIndexes = OOVHandler::instance().getWordIndexOptions(TokenInfo.Str, TokenInfo.Attrs);

                bool SingleWordIdxFound = false, RepeatedWordIdx = false;
                for(WordIndex_t w : WordIndexes){
                    if(w == WordIndex)
                        RepeatedWordIdx = true;
                    if(gConfigs.VocabWithoutSingleWordRule.contains(
                                gConfigs.SourceVocab.key(w)))
                        SingleWordIdxFound = true;
                }

                if (WordIndexes.isEmpty() || SingleWordIdxFound){
                    RuleTable::TargetRulesContainer_t TargetRules = OOVHandler::instance().gatherTemporaryTargetRules(TokenInfo.Str, TokenInfo.Attrs);
                    if (TargetRules.size()){
                        TokenInfo.TemporaryRuleNode.detachInvalidData();
                        TokenInfo.TemporaryRuleNode.targetRules().append(TargetRules);
                    }
                    if (TokenInfo.Attrs.value(enuDefaultAttrs::toStr(enuDefaultAttrs::NoDecode)).isValid())
                        return; // OOVHandler says that I must ignore this word when decoding
                }
                if(WordIndex != Constants::SrcVocabUnkWordIndex && !RepeatedWordIdx){
                    WordIndexes.append(WordIndex);
                }
            }else
                WordIndexes.append(WordIndex);
        }
        this->Tokens.append(clsToken(TokenInfo, WordIndexes));
        TargomanLogDebug(9,TokenInfo.Str<<","<<
                         TokenInfo.TagStr<<","<<
                         qVariantMapToString(TokenInfo.Attrs)<<","<<
                         WordIndexes<<","<<
                         gConfigs.SourceVocab.value(TokenInfo.Str, Constants::SrcVocabUnkWordIndex));

    }
}

/**
  TAGGING PROBLEM:
  @tag --> @tag --> Ok

  @tag word --> @tag word | word @tag --> Ok

  @tag --> word --> Ok

  word --> @tag --> ??

  @tag1 --> @tag2  --> ??

  @tag @tag --> @tag [@tag]+ --> ??

  @tag word --> @tag @tag  --> ??

  @tag

  **/
}
}
}
}
