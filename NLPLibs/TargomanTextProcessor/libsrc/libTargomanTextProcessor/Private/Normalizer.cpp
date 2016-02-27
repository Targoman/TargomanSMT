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
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include "Normalizer.h"

#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <iostream>
#include <QCryptographicHash>

#include "Unicode.hpp"

//zhnDebug:
#include <QtDebug>

namespace Targoman {
namespace NLPLibs {
namespace TargomanTP{
namespace Private {

Normalizer::Normalizer()
{
    initUnicodeNormalizers();
}

#define POP_DIRECTIONAL_FORMATTING      QChar(0x202c)
#define RIGHT_TO_LEFT_EMBEDDING         QChar(0x202b)
#define LEFT_TO_RIGHT_EMBEDDING         QChar(0x202a)
#define ARABIC_THOUSAND_SEPERATOR       QChar(0x066C)
#define WEIRD_THOUSAND_SEPERATOR        QChar(0xCB99)
#define WEIRD_DECIMAL_POINT             QChar(0xC2B7)
#define ARABIC_DECIMAL_POINT            QChar(0x066B)
#define ARABIC_TATWEEL                  QChar(0x0640)


/**
 * @brief Normalizes a character based on next char, last char, normalization config file and binary table.
 * @param _char character which should be normalized.
 * @param _nextChar next character
 * @param _interactive whether ask from user to define normal form of a unknown character or not?
 * @param _line line number
 * @param _phrase whole word.
 * @param _charPos character position.
 * @param _skipRecheck whether normalize again after normalization or not.
 * @return normalized form of character.
 */
QString Normalizer::normalize(const QChar &_char,
                              const QChar &_nextChar,
                              bool _interactive,
                              quint32 _line,
                              const QString &_phrase,
                              size_t _charPos,
                              bool _skipRecheck)
{
    QChar Char = _char;
    bool NextCharIsNotLeftJoinable = (_nextChar.isSpace() ||
                                      _nextChar.isSymbol() ||
                                      _nextChar.isDigit() ||
                                      _nextChar.isPunct() ||
                                      _nextChar.isNull());
    // //////////////////////////////////////////////////////////////////////////
    // //                    Multi Char Normalizers                           ///
    // //////////////////////////////////////////////////////////////////////////
    // Remove extra non-breaking space after non-joinable characters and before space
    if (Char == ARABIC_ZWNJ && (
                this->LastChar.joining() == QChar::Right ||         //character that join just from their right side. like: د,ر,ا
                this->LastChar.joining() == QChar::OtherJoining ||
                this->LastChar.isSpace() ||
                this->LastChar.isSymbol() ||
                this->LastChar.isDigit()||
                this->LastChar.isPunct() ||
                this->LastChar.isNull() ||
                NextCharIsNotLeftJoinable)){
        return "";
    }

    //Temporarily accept [POP DIRECTIONAL FORMATTING] character as it maybe used for ZWNJ
    if (Char == POP_DIRECTIONAL_FORMATTING){
        this->LastChar = Char;
        return "";
    }

    //Convert wrong tatweels to dash.
    if(Char == ARABIC_TATWEEL && !(_nextChar.script() == QChar::Script_Arabic && this->LastChar.script() == QChar::Script_Arabic))
            return this->LastChar = '-';

    //Convert special ZWNJ to ZWNJ
    if (Char == RIGHT_TO_LEFT_EMBEDDING && LastChar == POP_DIRECTIONAL_FORMATTING)
        return this->normalize(this->LastChar = ARABIC_ZWNJ, _nextChar, false, _line, _phrase, _charPos);

    //Convert thousand separators to comma //zhnDebug: Arabic Thousand Seperator is same glyph as comma in some fonts like Tahoma. we can handle it.
    if (LastChar.isDigit() && _nextChar.isDigit() && (
                Char == ARABIC_THOUSAND_SEPERATOR ||
                Char == WEIRD_THOUSAND_SEPERATOR))
        return this->LastChar = ',';

    //Convert special decimal point
    if (LastChar.isDigit() && _nextChar.isDigit() && (
                Char == WEIRD_DECIMAL_POINT ||
                Char == ARABIC_DECIMAL_POINT
                ))
        return this->LastChar = '.';

    //convert ye hamze, if it is in its isolated or last form, to ye hamze.
    if (Char == ARABIC_YE_HAMZA && (
                NextCharIsNotLeftJoinable))
        return this->LastChar = ARABIC_YE;

    //convert alef hamza down or alef hamza up, if it is in its isolated or last form, to alef.
    if ((Char == ARABIC_ALEF_HAMZA_DOWN || Char == ARABIC_ALEF_HAMZA_UP) && (
                NextCharIsNotLeftJoinable))
        return this->LastChar = ARABIC_ALEF;

    // //////////////////////////////////////////////////////////////////////////
    // //                        Using Binary Table                           ///
    // //////////////////////////////////////////////////////////////////////////
    if (this->BinaryMode){
        Q_ASSERT_X(this->BinTable.size(), "Initialized", "Seems that normalizer is not initialized");
        QString Normalized;
        if (Char == ARABIC_ZWNJ ||
            Char == POP_DIRECTIONAL_FORMATTING ||
            Char == RIGHT_TO_LEFT_EMBEDDING ||
            Char == ARABIC_THOUSAND_SEPERATOR ||
            Char == WEIRD_THOUSAND_SEPERATOR ||
            Char == WEIRD_DECIMAL_POINT)
            Normalized = Char;
        else
            Normalized = this->BinTable.at(Char.unicode()).toString();

        if (Normalized.size()){

            if (_skipRecheck) {
                this->LastChar = Normalized.at(Normalized.size() - 1);
                return Normalized;
            }
            else {
                QString TempNormalized = Normalized;
                Normalized.clear();
                for (int i = 0; i < TempNormalized.size(); i++) {
                    QString NormalizedChar = this->normalize(TempNormalized.at(i),
                                                             ((i + 1) < TempNormalized.size() ? TempNormalized.at(i+1) : _nextChar),
                                                             _interactive,
                                                             _line,
                                                             _phrase,
                                                             _charPos,
                                                             true
                                                             );
                    if(NormalizedChar.size())
                        Normalized.append(this->LastChar =  NormalizedChar[0]);
                }
                return Normalized;
            }
        }else{
            return "";
        }
    }

    // //////////////////////////////////////////////////////////////////////////
    // //                       Single Char Normalizers                       ///
    // //////////////////////////////////////////////////////////////////////////

    //Special characters
    if (Char == '\t' ||
            Char == QChar(0xFFFF) || // Noncharacter
            Char == QChar(0x7F))     // delete character
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
        return "";
    }

    //Convert to normal Space characters marked as space
    if (this->SpaceCharList.contains(Char))
        return this->LastChar = ' ';

    //Convert to ZWNJ characters marked as ZWNJ
    if (this->ZeroWidthSpaceCharList.contains(Char))
        return this->normalize(this->LastChar = ARABIC_ZWNJ, _nextChar, false, _line, _phrase, _charPos);

    //Convert characters based on Normalization table
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
        return "";
    }

    //convert any non assigned character to empty set symbol.
    if (Char.category() == QChar::Other_NotAssigned ||
            Char.category() == QChar::Other_PrivateUse ||
            Char.category() == QChar::Other_Surrogate)
        return this->LastChar = SYMBOL_REMOVED;

    if (_skipRecheck)
        return this->LastChar = Char;


    //Check if there are sepcial normalizers

    if (QCharScriptToNormalizerMap[Char.script()]){
        QString TempNormalized = QCharScriptToNormalizerMap[Char.script()](Char.unicode());
        QString Normalized;
        for (int i=0; i < TempNormalized.size(); i++) {
            QString NormalizedChar = this->normalize(TempNormalized.at(i),
                                                     ((i + 1) < TempNormalized.size() ? TempNormalized.at(i+1) : _nextChar),
                                                     _interactive,
                                                     _line,
                                                     _phrase,
                                                     _charPos,
                                                     true
                                                     );
            if(NormalizedChar.size())
                Normalized.append(this->LastChar =  NormalizedChar[0]);
        }
        return Normalized;
    }

    //Accept Currency, Math and special symbol characters
    if (Char.category() == QChar::Symbol_Currency ||
            Char.category() == QChar::Symbol_Math ||
            Char.category() == QChar::Symbol_Other)
        return this->LastChar = Char;

    //Change not resolved characrters interactively by user input.
    if(_interactive){
        std::cout<<"Character <"<<QString(Char).toUtf8().constData()<<">(0x";
        std::cout<<QString::number(Char.unicode(),16).toUpper().toLatin1().constData();
        std::cout<<")["<<QCharCategorytToStringMap[Char.category()]<<"] [";
        std::cout<<QCharScriptToStringMap[Char.script()];
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
                this->add2Configs (enuDicType::RemovingCharacters, Char);
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
                std::cout<<")["<<QCharCategorytToStringMap[Char.category()]<<"] [";
                std::cout<<QCharScriptToStringMap[Char.script()] ;
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
                this->add2Configs(enuDicType::ReplacingCharacters, Char, TempBuffer.at(0));
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
/**
 * @brief calls main normalizer function character by character.
 * @param _string input string
 * @param _line line number
 * @param _interactive can user interactively decide not resolve characters or not.
 * @return Returns normalized string
 */
QString Normalizer::normalize(const QString &_string, qint32 _line, bool _interactive)
{
    QString Normalized;
    this->LastChar = QChar();
    for (int i=0; i<_string.size(); i++){
        QString normalizedCharString = this->normalize(_string.at(i),
                                                       ((i + 1) < _string.size() ? _string.at(i+1) : QChar('\n')),
                                                       _interactive,
                                                       _line,
                                                       _string,
                                                       i);
        Normalized.append(normalizedCharString);
    }
    return fullTrim(Normalized);
}

/**
 * @brief adds interactively user inputs to normalization configuration file.
 * @param _type type of normalization (i.e. whitelist, removingCharacters, ...)
 * @param _originalChar original character
 * @param _replacement replacement character which is needed for ReplacingCharacters type.
 * @exception throws exception if backup of config file can not be created.
 * @exception throws exception if config file can not be opened.
 * @exception throws exception if config file is not writable.
 * @exception throws exception if type of type of normalization is invalid or "Unknown".
 */
void Normalizer::add2Configs(enuDicType::Type _type, QChar _originalChar, QChar _replacement)
{
    QFile ConfigFileOut(this->ConfigFileName);
    if(QFile::exists (this->ConfigFileName + ".back"))
        QFile::remove(this->ConfigFileName + ".back");
    if (!ConfigFileOut.copy(this->ConfigFileName + ".back"))
        throw exNormalizer("Unable to backup normalization file");

    QFile ConfigFileIn(this->ConfigFileName + ".back");
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
                // this happens when we have found correct sectoin, passed all lines of that and found the first line next section.
                // So we are in the correct place to add config.
                switch(_type){
                case enuDicType::NotSure:
                    ConfigFileOut.write(("NEW " + this->char2Str(_originalChar) + "\n").toUtf8());
                case enuDicType::WhiteList:
                case enuDicType::RemovingCharacters:
                case enuDicType::SpaceCharacters:
                case enuDicType::ZeroWidthSpaceCharacters:
                {
                    QString ToBeWritten = this->char2Str(_originalChar);
                    ConfigFileOut.write(QString("NEW %1 ## %2 [%3][%4]\n").arg(
                                            ToBeWritten).arg(
                                            ToBeWritten.startsWith("<0x") ?
                                                QString("{%1}").arg(_originalChar) :
                                                this->char2Str(_originalChar, true)).arg(
                                            QCharCategorytToStringMap[_originalChar.category()].c_str()).arg(
                                            QCharScriptToStringMap[_originalChar.script()].c_str()).toUtf8());
                }
                    break;
                case enuDicType::ReplacingCharacters:
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

/**
 * @brief Converts a character to a string (some times hex string) based on input character unicode.
 * @param _char Input character
 * @param _hexForced Sould output string necessarily be in hex format or not.
 * @return Returns output string
 */
QString Normalizer::char2Str(const QChar &_char, bool _hexForced)
{
    // it doesn't make hex string if _hexForced is not true and input character is of type latin 1 or number. 'equal (=)' character should always be in hex format.
    if (!_hexForced && (_char.isLetterOrNumber() || _char.toLatin1() == _char) && _char != '=')
        return _char;
    else
        return QString("<0x%1>").arg(QString::number(_char.unicode(),16).toLatin1().toUpper().constData());
}
/**
 * @brief converts a single or a range of character string to a list of QChars. characters can be in hex format.
 * @param _str input string
 * @param _line line number
 * @param _allowRange does it accept a range of character or not.
 * @exception throws exception if range conversion is nor allowed, but input string contains a range of characters.
 * @exception throws exception if input string does not contains a valid input pattern.
 * @return Returns a list QChars, extracted from input string.
 */
QList<QChar> Normalizer::str2QChar(QString _str, quint16 _line, bool _allowRange)
{
    thread_local static QRegExp RxRange = QRegExp("<0[x][0-9a-fA-F]+>-<0[x][0-9a-fA-F]+>");
    thread_local static QRegExp RxQChar = QRegExp("<0[x][0-9a-fA-F]+>");

    QList<QChar> Chars;
    if (_str.size() == 1)
        Chars.append(_str.at(0));
    else if (_str.contains(RxRange)){
        if(_allowRange){
            uint RangeStart = _str.split("-").first().replace("<0x","").replace(">","").toUInt(NULL,16);
            uint RangeEnd = _str.split("-").last().replace("<0x","").replace(">","").toUInt(NULL,16);
            for (uint i=RangeStart; i<=RangeEnd;i++)
                Chars.append(QChar(i));
        }else
            throw exNormalizer(("Invalid normalization character at line "+ QString::number(_line)+": <" + _str + ">"));
    }else if (_str.contains(RxQChar))
        Chars.append(QChar(_str.replace("<0x","").replace(">","").toUInt(NULL, 16)));
    else
        throw exNormalizer(("Invalid normalization character at line "+ QString::number(_line)+": <" + _str + ">"));

    return Chars;
}

/**
 * @brief Initializes and loads normalization rules using configuration file.
 * @param _configFile address of conguration file.
 * @param _binaryMode does "_cofigFile" is the address of binary table or not.
 * @exception throws exception if it is unable to open Binary table.
 * @exception throws exception if binary table is corrupted.
 * @exception throws exception if it is unable to open configuration file.
 * @exception throws exception if type of normalization in configuration file is "Unknown".
 * @exception throws exception if replacemnt type of normalization is not valid.
 * @exception throws exception we reach end of file before reading eof section in configuration file.
 */
void Normalizer::init(const QString &_configFile, bool _binaryMode)
{
    this->ConfigFileName = _configFile;
    this->BinaryMode = _binaryMode;

    if (_binaryMode){
        if (QFile::exists(_configFile) == false)
            throw exNormalizer("File <" + _configFile + " not found.");
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
    if (QFile::exists(this->ConfigFileName) == false)
        throw exNormalizer("File: <" + this->ConfigFileName + "> not found");
    QFile NormalizationFile(this->ConfigFileName);
    NormalizationFile.open(QIODevice::ReadOnly);
    if(!NormalizationFile.isReadable ())
        throw exNormalizer("Unable to open normalization file: <" + this->ConfigFileName + ">");

    QTextStream NormalizationConfigStream(&NormalizationFile);
    NormalizationConfigStream.setCodec("UTF-8");

    QString ConfigLine;
    int CommentIndex = -1;
    int LineNumber = 0;
    enuDicType::Type DicStep = enuDicType::Unknown;
    bool IsEOF = false;

    while (!NormalizationConfigStream.atEnd() && !IsEOF)
    {
        ConfigLine = NormalizationConfigStream.readLine().trimmed();
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
        case enuDicType::ReplacingCharacters:
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
        case enuDicType::RemovingCharacters:
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

    TargomanLogInfo(5,QString("Normalization Table has (%1 WHT/ %2 BLK/ %3 SPC/ %4 ZWNJ/ %5 RPL)").arg(
                        this->WhiteList.size()).arg(
                        this->RemovingList.size ()).arg(
                        this->SpaceCharList.size ()).arg(
                        this->ZeroWidthSpaceCharList.size ()).arg(
                        this->ReplacingTable.size()));
}

/**
 * @brief Creates binary table. For each character in unicode range, this function finds a normalized form for that and saves normalized form of that in a binary file.
 * @param _binFilePath Path to save binary table.
 * @param _interactive Can normalization process be done intractively or not.
 * @exception throws exception if this function is called when working in binary mode.
 * @exception throws exception if it could not open file to write binary table.
 */
void Normalizer::updateBinTable(const QString &_binFilePath, bool _interactive)
{
    if (this->BinaryMode)
        throw exNormalizer("Unable to update binary file when working in binary mode.");

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
/**
 * @brief Remove extra non-breaking space after non-joinable characters and before space and trims out side spaces and ZWNJs.
 * @param _str input string.
 * @return returns full trimmed string.
 */
QString Normalizer::fullTrim(const QString &_str)
{
    QString Normalized;
    QString trimmedString = _str.trimmed();
    for (int i=0; i<trimmedString.size(); i++){
        //
        if (trimmedString.at(i) == ARABIC_ZWNJ && ((i>0 &&(
                                              trimmedString.at(i-1).joining() == QChar::Right ||
                                              trimmedString.at(i-1).joining() == QChar::OtherJoining ||
                                              trimmedString.at(i-1).isSpace() ||
                                              trimmedString.at(i-1).isSymbol() ||
                                              trimmedString.at(i-1).isDigit()||
                                              trimmedString.at(i-1).isPunct()))
                                          ||  i == 0
                                          ||  i == trimmedString.size() - 1
                                          || (i+1 < trimmedString.size() &&(
                                                  trimmedString.at(i+1).isSpace() ||
                                                  trimmedString.at(i+1).isSymbol() ||
                                                  trimmedString.at(i+1).isDigit() ||
                                                  trimmedString.at(i+1).isPunct() ||
                                                  trimmedString.at(i+1).isNull()))))
            continue;
        else
            Normalized+=trimmedString.at(i);
    }
    return Normalized;
}

}
}
}
}

