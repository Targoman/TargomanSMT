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
#include <QSettings>
#include <iostream>
#include <QCryptographicHash>

#include "Unicode.hpp"

namespace Targoman {
namespace NLPLibs {
namespace Private {


Normalizer* Normalizer::Instance = NULL;

Normalizer::Normalizer()
{
    initUnicodeNormalizers();
}

QString Normalizer::normalize(const QChar &_char,
                              const QChar &_nextChar,
                              bool _interactive,
                              quint32 _line,
                              const QString &_phrase,
                              size_t _charPos,
                              bool _skipRecheck)
{
    QChar Char = _char;

    ////////////////////////////////////////////////////////////////////////////
    ////                    Multi Char Normalizers                           ///
    ////////////////////////////////////////////////////////////////////////////
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

    if (Char == ARABIC_YE_HAMZA && (
                _nextChar.isSpace() ||
                _nextChar.isSymbol() ||
                _nextChar.isDigit() ||
                _nextChar.isPunct() ||
                _nextChar.isNull()))
        return this->LastChar = ARABIC_YE;

    if ((Char == ARABIC_ALEF_HAMZA_DOWN || Char == ARABIC_ALEF_HAMZA_UP) && (
                _nextChar.isSpace() ||
                _nextChar.isSymbol() ||
                _nextChar.isDigit() ||
                _nextChar.isPunct() ||
                _nextChar.isNull()))
        return this->LastChar = ARABIC_ALEF;

    ////////////////////////////////////////////////////////////////////////////
    ////                        Using Binary Table                           ///
    ////////////////////////////////////////////////////////////////////////////
    if (this->BinaryMode){
        QString Normalized;
        if (Char == ARABIC_ZWNJ ||
            Char == QChar(0x202c) ||
            Char == QChar(0x202b) ||
            Char == QChar(0x066C) ||
            Char == QChar(0xCB99) ||
            Char == QChar(0xC2B7))
            Normalized = Char;
        else
            Normalized = this->BinTable.at(Char.unicode()).toString();

        if (Normalized.size()){
            if (Normalized.size() > 1){
                return Normalized;
                this->LastChar = Normalized.at(Normalized.size() - 1);
            }
            else if (_skipRecheck){
                this->LastChar = Normalized.at(Normalized.size() - 1);
                return Normalized;
            }else{
                Normalized  = this->normalize(Normalized.at(0),
                                       _nextChar, true, _line, _phrase, _charPos, true);
                uint unicode = Normalized.at(0).unicode();

                return Normalized;
            }
        }else{
            this->LastChar = QChar();
            return "";
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    ////                       Single Char Normalizers                       ///
    ////////////////////////////////////////////////////////////////////////////

    //Special characters
    if (Char == '\t' ||
            Char == QChar(0xFFFF) ||
            Char == QChar(0x7F))
        return " ";

    //Digits must be converted to ascii
    if (Char.isDigit())
        return this->LastChar = QChar(Char.digitValue() + '0');

    //Convert TitleCase to UpperCase
    if (Char.isTitleCase())
        Char = Char.toUpper();

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
            Char.category() == QChar::Mark_NonSpacing ||
            Char.category() == QChar::Symbol_Modifier){
        this->LastChar = QChar();
        return "";
    }

    if (Char.category() == QChar::Other_NotAssigned ||
            Char.category() == QChar::Other_PrivateUse ||
            Char.category() == QChar::Other_Surrogate)
        return this->LastChar = SYMBOL_REMOVED;

    if (_skipRecheck)
        return Char;

    enuUnicodeCharScripts::Type CharacterScript = (enuUnicodeCharScripts::Type)Char.script();

    //Check if there are sepcial normalizers
    if (ScriptBasedNormalizers[CharacterScript]){
        QString Normalized = ScriptBasedNormalizers[CharacterScript](Char.unicode());
        if (Normalized.size()){
            this->LastChar = Normalized.at(Normalized.size() - 1);
            if (Normalized.size() > 1)
                return Normalized;
            else
                return this->normalize(this->LastChar = Normalized.at(0),
                                       _nextChar, false, _line, _phrase, _charPos, true);
        }
    }

    //Accept Currency, Math and special symbol characters
    if (Char.category() == QChar::Symbol_Currency ||
            Char.category() == QChar::Symbol_Math ||
            Char.category() == QChar::Symbol_Other)
        return this->LastChar = Char;

    if(_interactive){
        std::cout<<"Character <"<<QString(Char).toUtf8().constData()<<">(0x";
        std::cout<<QString::number(Char.unicode(),16).toUpper().toLatin1().constData();
        std::cout<<")["<<QCHAR_UNICOE_CATEGORIES[Char.category()]<<"] [";
        std::cout<<enuUnicodeCharScripts::toStr(CharacterScript);
        std::cout<<"]could not be found in any list. What to do?"<<std::endl;
        std::cout<<"Line: "<<_line<<": "<<std::endl;
        std::cout<<_phrase.toUtf8().constData()<<std::endl;
        std::cout<<QString(_charPos-1, '-').toLatin1().constData()<<"^"<<std::endl;

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
                std::cout<<QString::number(Char.unicode(),16).toUpper().toLatin1().constData();
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

QString Normalizer::normalize(const QString &_string, qint32 _line, bool _interactive)
{
    QString Normalized;
    this->LastChar = QChar();
    for (int i=0; i<_string.size(); i++){
        Normalized.append(
                    this->normalize(_string.at(i),
                                    ((i + 1) < _string.size() ? _string.at(i+1) : QChar('\n')),
                                    _interactive,
                                    _line,
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
                    ConfigFileOut.write(QString("NEW %1 ## %2 [%3][%4]\n").arg(
                                            ToBeWritten).arg(
                                            ToBeWritten.startsWith("<0x") ?
                                                QString("{%1}").arg(_originalChar) :
                                                this->char2Str(_originalChar, true)).arg(
                                            QCHAR_UNICOE_CATEGORIES[_originalChar.category()]).arg(
                                enuUnicodeCharScripts::toStr(
                                    (enuUnicodeCharScripts::Type)
                                    _originalChar.script())).toUtf8());
                }
                    break;
                case enuDicType::ReplacingCharacter:
                {
                    QString ToBeWrittenKey = this->char2Str(_originalChar);
                    QString ToBeWrittenVal = this->char2Str(_replacement);
                    ConfigFileOut.write(QString("NEW %1 = %2 ## %3 ==> %4\n").arg(
                                            ToBeWrittenKey, ToBeWrittenVal).arg(
                                            ToBeWrittenKey.startsWith("<0x") ?
                                                QString("{%1}").arg(_originalChar) :
                                                this->char2Str(_originalChar, true)).arg(
                                            ToBeWrittenVal.startsWith("<0x") ?
                                                QString("{%1}").arg(_replacement) :
                                                this->char2Str(_replacement, true)).toUtf8());
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

QString Normalizer::char2Str(const QChar &_char, bool _hexForced)
{
    if (!_hexForced && (_char.isLetterOrNumber() || _char.toLatin1() == _char) && _char != '=')
        return _char;
    else
        return QString("<0x%1>").arg(QString::number(_char.unicode(),16).toLatin1().toUpper().constData());
}

QList<QChar> Normalizer::str2QChar(QString _str, quint16 _line, bool _allowRange)
{
    QList<QChar> Chars;
    if (_str.size() == 1)
        Chars.append(_str.at(0));
    else if (_str.contains(QRegExp("<0[x][0-9a-fA-F]+>-<0[x][0-9a-fA-F]+>"))){
        if(_allowRange){
            uint RangeStart = _str.split("-").first().replace("<0x","").replace(">","").toUInt(NULL,16);
            uint RangeEnd = _str.split("-").last().replace("<0x","").replace(">","").toUInt(NULL,16);
            for (uint i=RangeStart; i<=RangeEnd;i++)
                Chars.append(QChar(i));
        }else
            throw exNormalizer(("Invalid normalization character at line "+ QString::number(_line)+": <" + _str + ">"));
    }else if (_str.contains(QRegExp("<0[x][0-9a-fA-F]+>")))
        Chars.append(QChar(_str.replace("<0x","").replace(">","").toUInt(NULL, 16)));
    else
        throw exNormalizer(("Invalid normalization character at line "+ QString::number(_line)+": <" + _str + ">"));

    return Chars;
}

void Normalizer::init(const QString &_configFile, bool _binaryMode)
{
    this->ConfigFile = _configFile;
    this->BinaryMode = _binaryMode;

    if (_binaryMode){
        QFile BinFile(_configFile);
        BinFile.open(QFile::ReadOnly);
        if (!BinFile.isReadable())
            throw exNormalizer("Unable to open " + _configFile + " for reading");

        TargomanInlineInfo(5, "Reading Normalization Bin File...");
        QByteArray MD5 = BinFile.read(32);

        QByteArray Buffer = BinFile.readAll();
        if (QCryptographicHash::hash(Buffer, QCryptographicHash::Md5).toHex() != MD5){
            TargomanFinishInlineInfo(TARGOMAN_COLOR_ERROR, "Corrupted");
            throw exNormalizer("Seems taht binary table is corrupted");
        }

        QDataStream Stream(&Buffer, QIODevice::ReadOnly);
        Stream>>this->BinTable;
        TargomanFinishInlineInfo(TARGOMAN_COLOR_HAPPY, "Loaded");
        return;
    }

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
            DicStep = enuDicType::toEnum(ConfigLine.mid(1,ConfigLine.size() - 2).toLatin1().constData());
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
            foreach (const QString& CharStr, StrList){
                QList<QChar> Chars = this->str2QChar(CharStr, LineNumber);
                foreach (const QChar& Ch, Chars)
                    this->WhiteList.insert(Ch);
            }
            break;
        case enuDicType::ReplacingCharacter:
        {
            QStringList Pair = ConfigLine.split('=');
            if (Pair.size() == 2)
                this->ReplacingTable.insert(
                        this->str2QChar(Pair[0].trimmed(), LineNumber, false).first(),
                    Pair[1].trimmed().size() > 1 && Pair[1].trimmed().startsWith("<") ==false ?
                        Pair[1].trimmed() : str2QChar(Pair[1].trimmed(), LineNumber, false).first());
            else {
                throw exNormalizer(QString("Invalid Word Pair at line: %1 ==> %2").arg(LineNumber).arg(ConfigLine));
            }
        }
            break;
        case enuDicType::RemovingCharcters:
            StrList = ConfigLine.split(" ", QString::SkipEmptyParts);
            foreach (const QString& CharStr, StrList){
                QList<QChar> Chars = this->str2QChar(CharStr, LineNumber);
                foreach (const QChar& Ch, Chars)
                    this->RemovingList.insert(Ch);
            }
            break;
        case enuDicType::SpaceCharacters:
            StrList = ConfigLine.split(" ", QString::SkipEmptyParts);
            foreach (const QString& CharStr, StrList){
                QList<QChar> Chars = this->str2QChar(CharStr, LineNumber);
                foreach (const QChar& Ch, Chars)
                    this->SpaceCharList.insert(Ch);
            }
            break;
        case enuDicType::ZeroWidthSpaceCharacters:
            StrList = ConfigLine.split(" ", QString::SkipEmptyParts);
            foreach (const QString& CharStr, StrList){
                QList<QChar> Chars = this->str2QChar(CharStr, LineNumber);
                foreach (const QChar& Ch, Chars)
                    this->ZeroWidthSpaceCharList.insert(Ch);
            }
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

void Normalizer::updateBinTable(const QString &_binFilePath, bool _interactive)
{
    if (this->BinaryMode)
        throw exNormalizer("Unable to update binary file whn working in binary mode.");

    QFile BinFile(_binFilePath);
    BinFile.open(QFile::WriteOnly);
    if (!BinFile.isWritable())
        throw exNormalizer("Unable to open " + _binFilePath + " for writing");

    TargomanInlineInfo(5, "Creating Map...");

    QList<QVariant> Map;
    Map.append(SYMBOL_REMOVED);
    QChar Char;
    for (int i=1; i<=0xFFFF; i++){
        Char = QChar(i);
        Map.append(this->normalize(Char, QChar(),_interactive,-1,"NO_PHRASE",0));
    }
    TargomanFinishInlineInfo(TARGOMAN_COLOR_HAPPY, "Done");
    TargomanInlineInfo(5, "Writing to disk...");
    QByteArray Buffer;
    QDataStream OutStream(&Buffer, QIODevice::WriteOnly);
    OutStream<<Map;


    BinFile.write(QCryptographicHash::hash(Buffer, QCryptographicHash::Md5).toHex());
    BinFile.write(Buffer);

    TargomanFinishInlineInfo(TARGOMAN_COLOR_HAPPY, "Done");

    TargomanLogHappy(5, "Normalization binTable written to" + _binFilePath);
}

QString Normalizer::fullTrim(const QString &_str)
{
    QString Normalized;

    for (int i=0; i<_str.trimmed().size(); i++){
        //Remove extra non-breaking space after non-joinable characters and before space
        if (_str.at(i) == ARABIC_ZWNJ && ((i>0 &&(
                                              _str.at(i-1).joining() == QChar::Right ||
                                              _str.at(i-1).joining() == QChar::OtherJoining ||
                                              _str.at(i-1).isSpace() ||
                                              _str.at(i-1).isSymbol() ||
                                              _str.at(i-1).isDigit()||
                                              _str.at(i-1).isPunct()))
                                          ||  i == 0
                                          ||  i == _str.size() - 1
                                          || (i+1 < _str.size() &&(
                                                  _str.at(i+1).isSpace() ||
                                                  _str.at(i+1).isSymbol() ||
                                                  _str.at(i+1).isDigit() ||
                                                  _str.at(i+1).isPunct() ||
                                                  _str.at(i+1).isNull()))))
            continue;
        else
            Normalized+=_str.at(i);
    }
    return Normalized;
}
}
}
}

