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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#include <QStringList>
#include "libTargomanTextProcessor/TextProcessor.h"
#include "clsInput.h"
#include "Private/OOVHandler/OOVHandler.h"

using Targoman::NLPLibs::TextProcessor;

namespace Targoman {
namespace Core {
namespace Private {
namespace InputDecomposer {

using namespace Common;
using namespace OOV;

QSet<QString>    clsInput::SpecialTags;

Configuration::tmplConfigurable<QString>  clsInput::UserDefinedTags(
        clsInput::moduleName() + "/UserDefinedTags",
        "User Defined valid XML tags. ",
        "Valid user defined XML tags that must be stored with their attributes."
        "These must not overlap with predefined XML Tags"
        /*TODO add lambda to check overlap*/);
Configuration::tmplConfigurable<bool>    clsInput::IsIXML(
        clsInput::moduleName() + "/IsIXML",
        "Input is in Plain text(default) or IXML format",
        false);
Configuration::tmplConfigurable<bool>    clsInput::DoNormalize(
        clsInput::moduleName() + "/DoNormalize",
        "Normalize Input(default) or let it unchanged",
        true);

Configuration::tmplConfigurable<bool>    clsInput::TagNameEntities(
        clsInput::moduleName() + "/TagNameEntities",
        "Use NER to tag name entities",
        false);

/**
 * @brief clsInput::clsInput Instructor of this class gets input string and based on input arguments parses that.
 * @param _inputStr input string.
 */
clsInput::clsInput(const QString &_inputStr)
{
    if (this->IsIXML.value()) {
        if (this->DoNormalize.value())
            this->parseRichIXML(_inputStr,true, gConfigs.SourceLanguage.value());
        else
            this->parseRichIXML(_inputStr);
    }else
        this->parsePlain(_inputStr, gConfigs.SourceLanguage.value());
}
/**
 * @brief clsInput::init This function inserts userdefined and default tags to #SpecialTags.
 */
void clsInput::init()
{
    if (UserDefinedTags.value().size())
        foreach(const QString& Tag, UserDefinedTags.value().split(gConfigs.Separator.value()))
            SpecialTags.insert(Tag);
    for (int i=0; i<Targoman::NLPLibs::enuTextTags::getCount(); i++)
        SpecialTags.insert(Targoman::NLPLibs::enuTextTags::toStr((Targoman::NLPLibs::enuTextTags::Type)i));
}

/**
 * @brief clsInput::parsePlain This function fisrt converts plain text to iXML format and then parses that.
 * @param _inputStr Input string
 * @param _lang Language.
 */

void clsInput::parsePlain(const QString &_inputStr, const QString& _lang)
{
    this->parseRichIXML(TextProcessor::instance().text2RichIXML(_inputStr, _lang), false);
}


void clsInput::parseRichIXML(const QString &_inputIXML, bool _normalize, const QString &_lang)
{
    if (_normalize)
        this->parseRichIXML(TextProcessor::instance().normalizeText(_inputIXML,false, _lang));
    else
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
          this->newToken(Token);
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
                this->newToken(Token);
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
                        throw exInput("Attribute: <"+AttrName+" Was defined later.");
                    Attributes.insert(AttrName, AttrValue);
                    AttrName.clear();
                    AttrValue.clear();
                    ParsingState = CollectAttrName;
                }
                NextCharEscaped = false;
                continue;
            }
            NextCharEscaped = false;
            if (Ch == '\\')             //zhnDebug: why do we append '\'
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
                this->newToken(Token, TagStr, Attributes);

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

void clsInput::newToken(const QString &_token, const QString &_tagStr, const QVariantMap &_attrs)
{
    WordIndex_t WordIndex;
    QVariantMap Attributes = _attrs;

    if (_tagStr.size())
        WordIndex =  gConfigs.SourceVocab.value(_tagStr);
    else{
        WordIndex = gConfigs.SourceVocab.value(_token,0);
        if (WordIndex == 0){
            WordIndex = OOVHandler::instance().getWordIndex(_token, Attributes);
            if (_attrs.value(enuDefaultAttrs::toStr(enuDefaultAttrs::NoDecode)).isValid())
                return; // OOVHandler says that I must ignore this word when decoding
        }
    }

    this->Tokens.append(clsToken(_token, WordIndex, _tagStr, Attributes));
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
