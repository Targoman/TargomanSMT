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

#include "Normalizer.h"

#include <QFile>
#include <QTextStream>
#include <iostream>

#include "Unicode.hpp"

namespace Targoman {
namespace NLPLibs {
namespace Private {


Normalizer* Normalizer::Instance = NULL;


const QString DIACRITIC_CHARS =
        QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ");

Normalizer::Normalizer()
{
    this->NormalizedDiacritics<<"S"<<"OE"<<"Z"<<"s"<<"oe"<<"z"<<"Y"<<"Y"<<"u"<<"A"<<"A"<<"A"<<"A"<<"A"<<"A"<<"AE"<<
                                "C"<<"E"<<"E"<<"E"<<"E"<<"I"<<"I"<<"I"<<"I"<<"D"<<"N"<<"O"<<"O"<<"O"<<"O"<<"O"<<"O"<<
                                "U"<<"U"<<"U"<<"U"<<"Y"<<"s"<<"a"<<"a"<<"a"<<"a"<<"a"<<"a"<<"ae"<<"c"<<"e"<<"e"<<"e"<<
                                "e"<<"i"<<"i"<<"i"<<"i"<<"o"<<"n"<<"o"<<"o"<<"o"<<"o"<<"o"<<"o"<<"u"<<"u"<<"u"<<"u"<<
                                "y"<<"y";

}

QString Normalizer::normalize(const QChar &_char,
                              const QChar &_nextChar,
                              bool _interactive,
                              quint32 _line,
                              const QString &_phrase,
                              size_t _charPos)
{
    QChar Char = _char;
    //Special characters
    if (Char == '\t' ||
        Char == QChar(0xFFFF) ||
        Char == QChar(0x7F))
        return " ";

    //Digits must be converted to ascii
    if (Char.isDigit())
            return this->LastChar = QChar(Char.digitValue() + '0');

    //Remove extra non-breaking space after non-joinable characters and before space
    if (Char == ARABIC_ZWNJ && (
                this->LastChar.joining() == QChar::Right ||
                this->LastChar.joining() == QChar::OtherJoining ||
                this->LastChar.isSpace() ||
                this->LastChar.isSymbol() ||
                this->LastChar.isDigit()||
                this->LastChar.isPunct() ||
                this->LastChar.isNull() ||
                _nextChar.isSpace() ||
                _nextChar.isSymbol() ||
                _nextChar.isDigit() ||
                _nextChar.isPunct() ||
                _nextChar.isNull())){
        this->LastChar = QChar();
        return "";
    }

    //Temporarily accept [POP DIRECTIONAL FORMATTING] character as it maybe used for ZWNJ
    if (Char == QChar(0x202c)){
        this->LastChar = Char;
        return "";
    }

    //Convert special ZWNJ to ZWNJ
    if (Char == QChar(0x202b) && LastChar == QChar(0x202c))
        return this->normalize(this->LastChar = ARABIC_ZWNJ, _nextChar, false, _line, _phrase, _charPos);

    //Convert thousand separators to comma
    if (LastChar.isDigit() && _nextChar.isDigit() && (
                Char == QChar(0x066C) ||
                Char == QChar(0xCB99)))
        return this->LastChar = ',';

    //Convert special decimal point
    if (LastChar.isDigit() && _nextChar.isDigit() && (
                Char == QChar(0xC2B7)))
        return this->LastChar = '.';

    //Convert TitleCase to UpperCase
    if (Char.isTitleCase())
        Char = Char.toUpper();

    //Convert marked diacritic characters to their correspondent ASCII strings
    int DiacriticIndex = DIACRITIC_CHARS.indexOf(Char);
    if (DiacriticIndex >= 0)
        return this->NormalizedDiacritics.at(DiacriticIndex);

    //Accept Currency, Math and special symbol characters
    if (Char.category() == QChar::Symbol_Currency ||
            Char.category() == QChar::Symbol_Math ||
            Char.category() == QChar::Symbol_Other)
        return this->LastChar = Char;

    //Convert all special forms of quote and dquote to ASCII
    if (Char.category() == QChar::Punctuation_InitialQuote ||
        Char.category() == QChar::Punctuation_FinalQuote)
        return this->LastChar = '"';

    //Accept characters defined as white
    if (this->WhiteList.contains (Char))
        return this->LastChar = Char;

    //Remove characters defined in config file
    if (this->RemovingList.contains(Char)){
        this->LastChar = QChar();
        return "";
    }

    //Convert to normal Space characters marked as space
    if (this->SpaceCharList.contains(Char))
        return this->LastChar = ' ';

    //Convert to ZWNJ characters marked as ZWNJ
    if (this->ZeroWidthSpaceCharList.contains(Char))
        return this->normalize(this->LastChar = ARABIC_ZWNJ, _nextChar, false, _line, _phrase, _charPos);

    //Convert characcters based on Normalization table
    if (this->ReplacingTable.contains(Char)){
        QString Buff = this->ReplacingTable.value(Char);
        if (Buff.size() > 1){
            this->LastChar = *(Buff.end() - 1);
            return Buff;
        }else
            return this->LastChar = Buff.at(0);
    }

    //Remove all special control characters and character modifiers
    if (Char.category() == QChar::Letter_Modifier ||
        Char.category() == QChar::Symbol_Modifier)
        return this->LastChar = QChar();

    if (Char.category() == QChar::Other_NotAssigned ||
        Char.category() == QChar::Other_PrivateUse ||
        Char.category() == QChar::Mark_NonSpacing ||
        Char.category() == QChar::Other_Surrogate)
        return this->LastChar = SYMBOL_REMOVED;

    enuUnicodeCharScripts::Type CharacterScript = (enuUnicodeCharScripts::Type)QUnicodeTables::script(Char.unicode());

    //Check if there are sepcial normalizers
    if (ScriptBasedNormalizers[CharacterScript]){
        QString Normalized = ScriptBasedNormalizers[CharacterScript](Char.unicode());
        if (Normalized.size()){
            this->LastChar = Normalized.at(Normalized.size() - 1);
            return Normalized;
        }
    }

    if(_interactive){
        std::cout<<"Character <"<<QString(Char).toUtf8().constData()<<">(0x";
        std::cout<<QString::number(Char.unicode(),16).toUpper().toAscii().constData();
        std::cout<<")["<<QCHAR_UNICOE_CATEGORIES[Char.category()]<<"] [";
        std::cout<<enuUnicodeCharScripts::toStr(CharacterScript);
        std::cout<<"]could not be found in any list. What to do?"<<std::endl;
        std::cout<<"Line: "<<_line<<": "<<std::endl;
        std::cout<<_phrase.toUtf8().constData()<<std::endl;
        std::cout<<QString(_charPos-1, '-').toAscii().constData()<<"^"<<std::endl;

        bool ValidSelection=false;
        while (!ValidSelection)
        {
            std::cout<<"Press: (1: delete, 2: Accept, 3: Normalize, 4: Add as Space, 5: Add as ZWNJ, 6: Not Sure...)"<<std::endl;
            QString Result = QChar((char)std::cin.get());
            switch(Result.toInt ())
            {
            case 1:
                this->RemovingList.insert(Char);
                this->add2Configs (enuDicType::RemovingCharcters, Char);
                ValidSelection = true;
                break;
            case 2:
                this->WhiteList.insert(Char);
                this->add2Configs (enuDicType::WhiteList, Char);
                ValidSelection = true;
                break;
            case 4:
                this->SpaceCharList.insert(Char);
                this->add2Configs (enuDicType::SpaceCharacters, Char);
                ValidSelection = true;
                break;
            case 5:
                this->ZeroWidthSpaceCharList.insert(Char);
                this->add2Configs (enuDicType::ZeroWidthSpaceCharacters, Char);
                ValidSelection = true;
                break;
            case 6:
                this->add2Configs (enuDicType::NotSure, Char);
                ValidSelection = true;
                break;
            case 3:
            {
                std::string Buffer;

                std::cout<<"Normalize <"<<QString(Char).toUtf8().constData()<<">(0x";
                std::cout<<QString::number(Char.unicode(),16).toUpper().toAscii().constData();
                std::cout<<")["<<QCHAR_UNICOE_CATEGORIES[Char.category()]<<"] [";
                std::cout<<enuUnicodeCharScripts::toStr(CharacterScript);
                std::cout<<"] to: "<<std::endl;
                std::cin >>Buffer;
                QString TempBuffer = QString::fromUtf8(Buffer.c_str());

                if (TempBuffer.isEmpty())
                    continue;

                if (TempBuffer.size() > 1){
                    qCritical("Invalid normalization character. Modify config file manually to insert multi-char modifiers");
                    continue;
                }
                this->ReplacingTable.insert(Char,TempBuffer.at(0));
                this->add2Configs(enuDicType::ReplacingCharacter, Char, TempBuffer.at(0));
                ValidSelection = true;
                break;
            }
            default:
                break;
            }
        }
        return normalize(Char, _nextChar, false, _line, _phrase, _charPos);
    }
    else
        return Char;
}

QString Normalizer::normalize(const QString &_string, bool _interactive)
{
    QString Normalized;
    for (int i=0; i<_string.size(); i++){
        Normalized.append(
                    this->normalize(_string.at(i),
                                    ((i + 1) < _string.size() ? _string.at(i+1) : QChar('\n')),
                                    _interactive,
                                    0,
                                    _string,
                                    i));
    }
    return Normalized;
}

void Normalizer::add2Configs(enuDicType::Type _type, QChar _originalChar, QChar _replacement)
{
    QFile ConfigFileOut(this->ConfigFile);
    if(QFile::exists (this->ConfigFile + ".back"))
        QFile::remove(this->ConfigFile + ".back");
    if (!ConfigFileOut.copy(this->ConfigFile + ".back"))
        throw exNormalizer("Unable to backup normalization file");

    QFile ConfigFileIn(this->ConfigFile + ".back");
    ConfigFileIn.open(QIODevice::ReadOnly);
    ConfigFileOut.open(QIODevice::WriteOnly);

    if(!ConfigFileIn.isReadable ())
        throw exNormalizer("Unable to open backup normalization file.");
    if(!ConfigFileOut.isWritable ())
        throw exNormalizer("Unable to open output normalization file.");

    QByteArray DataLine;
    enuDicType::Type DicStep;
    bool SectionFound= false;

    while (!ConfigFileIn.atEnd())
    {
        DataLine = ConfigFileIn.readLine().trimmed();

        if (DataLine.startsWith("[") && DataLine.endsWith("]")){
            DicStep = enuDicType::toEnum(DataLine.mid(1,DataLine.size() - 2).constData());
            if (DicStep == enuDicType::Unknown)
                throw exNormalizer(QString("Invalid Dic Type: %1").arg(DataLine.constData()));
            else if (DicStep == _type)
                SectionFound=true;
            else if(SectionFound && DicStep != _type){
                switch(_type){
                case enuDicType::NotSure:
                    ConfigFileOut.write(("NEW " + this->char2Str(_originalChar) + "\n").toUtf8());
                case enuDicType::WhiteList:
                case enuDicType::RemovingCharcters:
                case enuDicType::SpaceCharacters:
                case enuDicType::ZeroWidthSpaceCharacters:
                {
                    QString ToBeWritten = this->char2Str(_originalChar);
                    if (ToBeWritten.startsWith("<0x"))
                        ConfigFileOut.write(QString("NEW %1 ## {%2} [%3][%4]\n").arg(
                                         ToBeWritten).arg(
                                         _originalChar).arg(
                                         QCHAR_UNICOE_CATEGORIES[_originalChar.category()]).arg(
                                         enuUnicodeCharScripts::toStr(
                                            (enuUnicodeCharScripts::Type)
                                        QUnicodeTables::script(_originalChar.unicode()))).toUtf8());
                    else
                        ConfigFileOut.write(("NEW " + ToBeWritten + "\n").toUtf8());
                }
                    break;
                case enuDicType::ReplacingCharacter:
                {
                    QString ToBeWrittenKey = this->char2Str(_originalChar);
                    QString ToBeWrittenVal = this->char2Str(_replacement);
                    if (ToBeWrittenKey.startsWith("<0x") || ToBeWrittenVal.startsWith("<0x"))
                        ConfigFileOut.write(QString("NEW %1 = %2 ## {%3} ==> {%4}\n").arg(
                                         ToBeWrittenKey, ToBeWrittenVal).arg(
                                         _originalChar).arg(
                                         _replacement).toUtf8());
                    else
                        ConfigFileOut.write(QString("NEW %1 = %2\n").arg(ToBeWrittenKey, ToBeWrittenVal).toUtf8());
                }
                    break;
                default:
                    throw exNormalizer("Invalid insertion Type");
                }
                SectionFound= false;
            }
        }
        ConfigFileOut.write (DataLine + "\n");
    }


    ConfigFileIn.close ();
    ConfigFileOut.close ();
}

QString Normalizer::char2Str(const QChar &_char)
{
    if ((_char.isLetterOrNumber() || _char.toAscii() == _char) && _char != '=')
        return _char + QString(" ##<0x%1>").arg(QString::number(_char.unicode(),16).toAscii().toUpper().constData());
    else
        return QString("<0x%1>").arg(QString::number(_char.unicode(),16).toAscii().toUpper().constData());
}

QChar Normalizer::str2QChar(QString _str, quint16 _line)
{
    if (_str.size() == 1)
        return _str.at(0);
    else if (_str.contains(QRegExp("<0[xX][0-9a-fA-F]+>")))
        return QChar(_str.replace("<0x","").replace(">","").toUInt(NULL, 16));
    else
        throw exNormalizer(("Invalid normalization character at line "+ QString::number(_line)+": <" + _str + ">"));
}

void Normalizer::init(const QString &_configFile)
{
    this->ConfigFile = _configFile;

    QFile ConfigFile(this->ConfigFile);
    ConfigFile.open(QIODevice::ReadOnly);
    if(!ConfigFile.isReadable ())
        throw exNormalizer("Unable to open normalization file.");

    QTextStream ConfigStream(&ConfigFile);
    ConfigStream.setCodec("UTF-8");

    QString ConfigLine;
    int CommentIndex = -1;
    int LineNumber = 0;
    enuDicType::Type DicStep = enuDicType::Unknown;
    bool IsEOF = false;

    while (!ConfigStream.atEnd() && !IsEOF)
    {
        ConfigLine = ConfigStream.readLine().trimmed();
        LineNumber++;
        if (ConfigLine.startsWith("##") || ConfigLine.trimmed().isEmpty())
            continue;

        if ((CommentIndex = ConfigLine.indexOf("##")) >= 0)
            ConfigLine.truncate(CommentIndex);

        ConfigLine = ConfigLine.trimmed();

        if (ConfigLine.startsWith("[") && ConfigLine.endsWith("]")){
            DicStep = enuDicType::toEnum(ConfigLine.mid(1,ConfigLine.size() - 2).toAscii().constData());
            if (DicStep == enuDicType::Unknown)
                throw exNormalizer(QString("Invalid Dic Type at line: %1").arg(LineNumber));
            else if (DicStep == enuDicType::EndOfFile)
                IsEOF=true;
            continue;
        }

        if (ConfigLine.startsWith("NEW "))
            ConfigLine.remove(0, 4);

        QStringList StrList;
        switch(DicStep)
        {
        case enuDicType::WhiteList:
            StrList = ConfigLine.split(" ", QString::SkipEmptyParts);
            foreach (const QString& CharStr, StrList)
                this->WhiteList.insert(this->str2QChar(CharStr, LineNumber));
            break;
        case enuDicType::ReplacingCharacter:
            {
                QStringList Pair = ConfigLine.split('=');
                if (Pair.size() == 2)
                    this->ReplacingTable.insert(
                            this->str2QChar(Pair[0].trimmed(), LineNumber),
                        Pair[1].trimmed().size() > 1 && Pair[1].trimmed().startsWith("<") ==false ?
                            Pair[1].trimmed() : str2QChar(Pair[1].trimmed(), LineNumber));
                else {
                    throw exNormalizer(QString("Invalid Word Pair at line: %1 ==> %2").arg(LineNumber).arg(ConfigLine));
                }
            }
            break;
        case enuDicType::RemovingCharcters:
            StrList = ConfigLine.split(" ", QString::SkipEmptyParts);
            foreach (const QString& CharStr, StrList)
                this->RemovingList.insert (this->str2QChar(CharStr,LineNumber));
            break;
        case enuDicType::SpaceCharacters:
            StrList = ConfigLine.split(" ", QString::SkipEmptyParts);
            foreach (const QString& CharStr, StrList)
                this->SpaceCharList.insert (this->str2QChar(CharStr,LineNumber));
            break;
        case enuDicType::ZeroWidthSpaceCharacters:
            StrList = ConfigLine.split(" ", QString::SkipEmptyParts);
            foreach (const QString& CharStr, StrList)
                this->ZeroWidthSpaceCharList.insert (this->str2QChar(CharStr,LineNumber));
            break;
        default:
            continue;
        }
    }

    if (!IsEOF)
        throw exNormalizer("Invalid Normalization file as EOF section not found");

    TargomanLogInfo(5,QString("Normalization Table has (%1 WHT/ %2 BLK/ %3 SPC/ %4 NBS/ %5 RPL)").arg(
                            this->WhiteList.size()).arg(
                            this->RemovingList.size ()).arg(
                            this->SpaceCharList.size ()).arg(
                            this->ZeroWidthSpaceCharList.size ()).arg(
                            this->ReplacingTable.size()));
}

}
}
}
