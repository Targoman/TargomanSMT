#include "clsInput.h"
#include <QVariantMap>
#include <iostream>

namespace MosesTraining{


clsInput::clsInput()
{

    QString sp[] = {"Number", "SpecialNumber", "Email", "URL", "Abbreviation",
                    "OrderedListItem", "Time", "Date", "Ordinals", "Symbol"};
    for(int i = 0; i < 10; i++)
        SpecialTags.insert(sp[i]);
}

std::vector<TokenInfo> clsInput::parseRichIXML(const QString &_inputIXML)
{
    std::vector<TokenInfo> res;
    if (_inputIXML.contains('<') == false) {
      foreach(const QString& token, _inputIXML.split(" ", QString::SkipEmptyParts))
          res.push_back(TokenInfo(token));
      return res;
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
                if(Token.size())
                    res.push_back(Token);
                Token.clear();
            }else if (Ch == '\\'){
                NextCharEscaped = true;
                Token.append(Ch);
            }else
                Token.append(Ch);
            break;

        case XMLOpened:
            if (this->isSpace(Ch)){
                if (this->SpecialTags.contains(TagStr) == false){
                    std::cerr << "Unrecognized Tag Name: <" << TagStr.toStdString() << ">" << std::endl;
                    exit(2);
                }
                ParsingState = CollectAttrName;
            }else if (Ch == '>'){
                if (this->SpecialTags.contains(TagStr) == false){
                    std::cerr << "Unrecognized Tag Name: <" << TagStr.toStdString() << ">" << std::endl;
                    exit(2);
                }
                ParsingState = CollectXMLText;
            }else if(Ch.isLetter())
                TagStr.append(Ch);
            else{
                std::cerr << "Inavlid character '" << QString(Ch).toStdString() <<"' at index: " << Index << std::endl;
                exit(2);
            }
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
             else{
                 std::cerr << "Inavlid character '" << QString(Ch).toStdString() << "' at index: " << Index << std::endl;
                 exit(2);
             }
             break;
        case Looking4AttrValue:
            if (this->isSpace(Ch))
                continue; //skip spaces unitl attr value started
            else if (Ch == '"')
                ParsingState = CollectAttrValue;
            else{ //Short XML tags <b/> are invalid as XML text is obligatory
                std::cerr << "Inavlid character '" << QString(Ch).toStdString() << "' at index: " << Index << std::endl;
                exit(2);
            }
            break;
        case CollectAttrValue:
            if (Ch == '"'){
                if (NextCharEscaped)
                    AttrValue.append(Ch);
                else{
                    if (Attributes.contains(AttrName)){

                        std::cerr << "Attribute: <" << AttrName.toStdString() << "> Was defined later." << std::endl;
                        exit(2);
                    }
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
            else{
                std::cerr << "Inavlid character '" << QString(Ch).toStdString()<<"' at index: " << Index << " it must be '/'" << std::endl;
                exit(2);
            }
            break;
        case XMLClosed:
            if (this->isSpace(Ch))
                continue; //skip Until end of tag
            else if (Ch == '>'){
                if (TempStr != TagStr){
                    std::cerr << "Invalid closing tag: <" << TempStr.toStdString() << "> while looking for <" <<
                                 TagStr.toStdString() << ">" << std::endl;
                    exit(2);
                }
                res.push_back(TokenInfo(Token, TagStr));

                Token.clear();
                TempStr.clear();
                TagStr.clear();
                Attributes.clear();
                AttrName.clear();
                AttrValue.clear();
                ParsingState = Look4Open;
            }else if (Ch.isLetter())
                TempStr.append(Ch);
            else{
                std::cerr << "Inavlid character '" << QString(Ch).toStdString() << "' at index: " << Index << std::endl;
                exit(2);
            }
        }
    }

//    std::cerr << "Token INFO ***** " << Token.toStdString() << " ** " << ParsingState << std::endl;
    if(Token.size())
        res.push_back(TokenInfo(Token, TagStr));
    switch(ParsingState){
    case Look4Open:
        return res;
    case XMLOpened:
    case CollectAttrName:
    case CollectXMLText:
    case Look4Closed:
    case XMLClosed:
        std::cerr << "XML Tag: <" << TagStr.toStdString() << "> has not been closed" << std::endl;
    case Looking4AttrValue:
        std::cerr << "XML Tag: <" << TagStr.toStdString() << "> Attribute: <" << AttrName.toStdString()
                  << "> has no value" << std::endl;
    case CollectAttrValue:
        std::cerr << "XML Tag: <" << TagStr.toStdString() << "> Attribute: <" << AttrName.toStdString()
                  <<"> value not closed"<< std::endl;
    std::cerr << "Error in parsing IXML" << std::endl;
    exit(2);
    }
    return res;
}

}
