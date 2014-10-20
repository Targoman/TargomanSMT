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

namespace Targoman {
namespace NLPLibs {
namespace Private {

//More info on http://www.fileformat.info/info/unicode/category

Normalizer* Normalizer::Instance = NULL;

const char* QCHAR_UNICOE_CATEGORIES[]={
    "QChar::NoCategory",                ///0
    "QChar::Mark_NonSpacing",           ///1
    "QChar::Mark_SpacingCombining",     ///2
    "QChar::Mark_Enclosing",            ///3
    "QChar::Number_DecimalDigit",       ///4
    "QChar::Number_Letter",             ///5
    "QChar::Number_Other",              ///6
    "QChar::Separator_Space",           ///7
    "QChar::Separator_Line",            ///8
    "QChar::Separator_Paragraph",       ///9
    "QChar::Other_Control",             ///10
    "QChar::Other_Format",              ///11
    "QChar::Other_Surrogate",           ///12
    "QChar::Other_PrivateUse",          ///13
    "QChar::Other_NotAssigned",         ///14
    "QChar::Letter_Uppercase",          ///15
    "QChar::Letter_Lowercase",          ///16
    "QChar::Letter_Titlecase",          ///17
    "QChar::Letter_Modifier",           ///18
    "QChar::Letter_Other",              ///19
    "QChar::Punctuation_Connector",     ///20
    "QChar::Punctuation_Dash",          ///21
    "QChar::Punctuation_Open",          ///22
    "QChar::Punctuation_Close",         ///23
    "QChar::Punctuation_InitialQuote",  ///24
    "QChar::Punctuation_FinalQuote",    ///25
    "QChar::Punctuation_Other",         ///26
    "QChar::Symbol_Math",               ///27
    "QChar::Symbol_Currency",           ///28
    "QChar::Symbol_Modifier",           ///29
    "QChar::Symbol_Other"               ///30
};

// See http://www.unicode.org/reports/tr24/tr24-5.html
//Following strings are compatible with Qt implementation of unicode Tables
//@see $QTSOURCE/src/corelib/tools/qunicodetables_p.h

TARGOMAN_DEFINE_ENHANCED_ENUM_BEGIN(enuUnicodeCharScripts)
    Common,       //0
    Greek,        //1
    Cyrillic,     //3
    Armenian,     //4
    Hebrew,       //5
    Arabic,       //6
    Syriac,       //7
    Thaana,       //8
    Devanagari,   //9
    Bengali,      //10
    Gurmukhi,     //11
    Gujarati,     //12
    Oriya,        //13
    Tamil,        //14
    Telugu,       //15
    Kannada,      //16
    Malayalam,    //17
    Sinhala,      //18
    Thai,         //19
    Lao,          //20
    Tibetan,      //21
    Myanmar,      //22
    Georgian,     //23
    Hangul,       //24
    Ogham,        //25
    Runic,        //26
    Khmer,        //27
    Nko,          //28
    Inherited,    //29
    ScriptCount,  //30
    Latin,        //31
    Ethiopic,     //32
    Cherokee,     //33
    CanadianAboriginal,   //34
    Mongolian,    //35
    Hiragana,     //36
    Katakana,     //37
    Bopomofo,     //38
    Han,          //39
    Yi,           //40
    OldItalic,    //41
    Gothic,       //42
    Deseret,      //43
    Tagalog,      //44
    Hanunoo,      //45
    Buhid,        //46
    Tagbanwa,     //47
    Limbu,        //48
    TaiLe,        //49
    LinearB,      //50
    Ugaritic,     //51
    Shavian,      //52
    Osmanya,      //53
    Cypriot,      //54
    Braille,      //55
    Buginese,     //56
    Coptic,       //57
    NewTaiLue,    //58
    Glagolitic,   //59
    Tifinagh,     //60
    SylotiNagri,  //61
    OldPersian,   //62
    Kharoshthi,   //63
    Balinese,     //64
    Cuneiform,    //65
    Phoenician,   //66
    PhagsPa       //67
TARGOMAN_DEFINE_ENHANCED_ENUM_STRINGS
    "Common",       //0
    "Greek",        //1
    "Cyrillic",     //3
    "Armenian",     //4
    "Hebrew",       //5
    "Arabic",       //6
    "Syriac",       //7
    "Thaana",       //8
    "Devanagari",   //9
    "Bengali",      //10
    "Gurmukhi",     //11
    "Gujarati",     //12
    "Oriya",        //13
    "Tamil",        //14
    "Telugu",       //15
    "Kannada",      //16
    "Malayalam",    //17
    "Sinhala",      //18
    "Thai",         //19
    "Lao",          //20
    "Tibetan",      //21
    "Myanmar",      //22
    "Georgian",     //23
    "Hangul",       //24
    "Ogham",        //25
    "Runic",        //26
    "Khmer",        //27
    "Nko",          //28
    "Inherited",    //29
    "ScriptCount",  //30
    "Latin",        //31
    "Ethiopic",     //32
    "Cherokee",     //33
    "CanadianAboriginal",   //34
    "Mongolian",    //35
    "Hiragana",     //36
    "Katakana",     //37
    "Bopomofo",     //38
    "Han",          //39
    "Yi",           //40
    "OldItalic",    //41
    "Gothic",       //42
    "Deseret",      //43
    "Tagalog",      //44
    "Hanunoo",      //45
    "Buhid",        //46
    "Tagbanwa",     //47
    "Limbu",        //48
    "TaiLe",        //49
    "LinearB",      //50
    "Ugaritic",     //51
    "Shavian",      //52
    "Osmanya",      //53
    "Cypriot",      //54
    "Braille",      //55
    "Buginese",     //56
    "Coptic",       //57
    "NewTaiLue",    //58
    "Glagolitic",   //59
    "Tifinagh",     //60
    "SylotiNagri",  //61
    "OldPersian",   //62
    "Kharoshthi",   //63
    "Balinese",     //64
    "Cuneiform",    //65
    "Phoenician",   //66
    "PhagsPa"       //67
TARGOMAN_DEFINE_ENHANCED_ENUM_END


//A hack to use script method from unicodeTales in Qt core
QT_BEGIN_NAMESPACE
namespace QUnicodeTables {
    extern int script(uint _ucs4);
}
QT_END_NAMESPACE

typedef QString (*ScriptBasedNormalizer) (uint _ucs4);

QString acceptAll(uint _ucs4){
    return QChar(_ucs4);
}

QString greek2Ascii(uint _ucs4){
//@see http://www.unicode.org/charts/PDF/U0370.pdf
//@see http://www.unicode.org/charts/PDF/U1F00.pdf
    switch(_ucs4){
    case 0x386: return "A";
    case 0x388: return "E";
    case 0x389: return "H";
    case 0x38A: return "I";
    case 0x38C: return "O";
    case 0x38E: return "Y";

    case 0x390: return "i";
    case 0x391: return "A";
    case 0x392: return "B";
    case 0x395: return "E";
    case 0x396: return "Z";
    case 0x397: return "H";
    case 0x399: return "I";
    case 0x39A: return "K";
    case 0x39C: return "M";
    case 0x39D: return "N";

    case 0x3A1: return "P";
    case 0x3A4: return "T";
    case 0x3A5: return "Y";
    case 0x3A7: return "X";
    case 0x3AA: return "I";
    case 0x3AB: return "Y";
    case 0x3AF: return "i";

    case 0x3B9: return "i";
    case 0x3BA: return "k";
    case 0x3BD: return "v";
    case 0x3BF: return "o";

    case 0x3CA: return "i";
    case 0x3CB: return "v";
    case 0x3CC: return "o";
    case 0x3CD: return "v";

    case 0x3DC: return "F";

    case 0x3F3: return "c";
    //case 0x3F4: return "j";
    case 0x3FA: return "M";
    case 0x3F9: return "C";
    }

    if ((_ucs4 >= 0x1F08 && _ucs4 <= 0x1F0F) ||
            (_ucs4 >= 0x1F88 && _ucs4 <= 0x1F8F) ||
            (_ucs4 >= 0x1FB8 && _ucs4 <= 0x1FBC))
        return "A";
    if ((_ucs4 >= 0x1F18 && _ucs4 <= 0x1F1D) ||
            (_ucs4 >= 0x1FC8 && _ucs4 <= 0x1FC9))
        return "E";
    if ((_ucs4 >= 0x1F28 && _ucs4 <= 0x1F2F) ||
            (_ucs4 >= 0x1F98 && _ucs4 <= 0x1F9F)||
            (_ucs4 >= 0x1FCA && _ucs4 <= 0x1FCC))
        return "H";
    if ((_ucs4 >= 0x1F30 && _ucs4 <= 0x1F37) ||
            (_ucs4 >= 0x1F76 && _ucs4 <= 0x1F77)||
            (_ucs4 >= 0x1FD0 && _ucs4 <= 0x1FD3) ||
            (_ucs4 >= 0x1FD6 && _ucs4 <= 0x1FD7))
        return "i";
    if ((_ucs4 >= 0x1F38 && _ucs4 <= 0x1F3F)||
            (_ucs4 >= 0x1FD8 && _ucs4 <= 0x1FDB))
        return "I";
    if ((_ucs4 >= 0x1F40 && _ucs4 <= 0x1F45) ||
            (_ucs4 >= 0x1F78 && _ucs4 <= 0x1F79))
        return "o";
    if ((_ucs4 >= 0x1F48 && _ucs4 <= 0x1F4D)||
            (_ucs4 >= 0x1FF8 && _ucs4 <= 0x1FF9))
        return "O";
    if (_ucs4 >= 0x1F50 && _ucs4 <= 0x1F57)
        return "v";
    if (_ucs4 == 0x1F59 || _ucs4 == 0x1F5B || _ucs4 == 0x1F5D || _ucs4 == 0x1F5F ||
            (_ucs4 >= 0x1FE8 && _ucs4 <= 0x1FEB))
        return "Y";
    return "";
}

QString cyrillic2Ascii(uint _ucs4){
//@see http://www.unicode.org/charts/PDF/U0400.pdf
//@see http://www.unicode.org/charts/PDF/U0500.pdf
//@see http://www.unicode.org/charts/PDF/U2DE0.pdf
//@see http://www.unicode.org/charts/PDF/UA640.pdf
    if (_ucs4 == 0x400 || _ucs4 == 0x401 || _ucs4 == 0x4D6 || _ucs4 == 0x415)
            return "E";
    if ((_ucs4 == 0X410 || _ucs4 == 0x4D0 || _ucs4 == 0x4D2 ))
        return "A";
    if (_ucs4 == 0x412 || _ucs4 == 0x432)
        return "B";
    if (_ucs4 == 0x41A || _ucs4 == 0x43A || _ucs4 == 0x45C || _ucs4 == 0x51E || _ucs4 == 0x51F ||
            (_ucs4 >= 0x49A && _ucs4 <= 0x49E) ||
            _ucs4 == 0x4A0 || _ucs4 == 0x4A1 )
        return "K";
    if (_ucs4 == 0x41C || _ucs4 == 0x43C)
        return "M";
    if (_ucs4 == 0x41D || _ucs4 == 0x43D)
        return "H";
    if (_ucs4 == 0x41E)
        return "O";
    if (_ucs4 == 0x420 || _ucs4 == 0x48E || _ucs4 == 0x48F)
        return "P";
    if (_ucs4 == 0x421)
        return "C";
    if (_ucs4 == 0x422 || _ucs4 == 0x442 || _ucs4 == 0xA690 || _ucs4 == 0xA691)
        return "T";
    if (_ucs4 == 0x423 || _ucs4 == 0x443 || _ucs4 == 0x45E || _ucs4 == 0x4EE || _ucs4 == 0x4EF ||
            (_ucs4 >= 0x4F0 && _ucs4 <= 0x4F3))
        return "y";
    if (_ucs4 == 0x425 || _ucs4 == 0x445)
        return "X";
    if (_ucs4 == 0x42C || _ucs4 == 0x44C)
        return "b";
    if (_ucs4 == 0x456 || _ucs4 == 0x457)
        return "i";
    if (_ucs4 == 0x430)
        return "a";
    if (_ucs4 == 0x0501)
        return "d";
    if (_ucs4 == 0x050C)
        return "G";
    if (_ucs4 == 0x051A)
        return "Q";
    if (_ucs4 == 0x051B)
        return "q";
    if (_ucs4 == 0x458)
        return "j";
    if (_ucs4 == 0x435 || _ucs4 == 0x450 || _ucs4 == 0x451)
        return "e";
    if (_ucs4 == 0x43E)
        return "o";
    if (_ucs4 == 0x440)
        return "p";
    if (_ucs4 == 0x441)
        return "c";
    if (_ucs4 == 0x405)
        return "S";
    if (_ucs4 == 0x455)
        return "s";
    if (_ucs4 == 0x4D4)
        return "AE";
    if (_ucs4 == 0x4D5)
        return "ae";
    if (_ucs4 == 0x460 || _ucs4 == 0x51C)
        return "W";
    if (_ucs4 == 0x461 || _ucs4 == 0x51D)
        return "w";
    if (_ucs4 == 0x49F)
        return "w";
    if (_ucs4 == 0x526 || _ucs4 == 0x527)
        return "h";
    if (_ucs4 >= 0xA640 && _ucs4 <= 0xA643)
        return "Z";
}

QString armenina2Ascii(uint _ucs4){
//@see http://www.unicode.org/charts/PDF/U0530.pdf
//@see http://www.unicode.org/charts/PDF/UFB00.pdf

    if (_ucs4 == 0xFB00)
        return "ff";
    if (_ucs4 == 0xFB01)
        return "fi";
    if (_ucs4 == 0xFB02)
        return "fl";
    if (_ucs4 == 0xFB03)
        return "ffi";
    if (_ucs4 == 0xFB04)
        return "ffl";
    if (_ucs4 == 0xFB05)
        return "st";
    if (_ucs4 == 0xFB06)
        return "st";
}

const ScriptBasedNormalizer ScriptBasedNormalizers[]={
    NULL,//    Common,       //0
    greek2Ascii,//    Greek,        //1
    cyrillic2Ascii,//    Cyrillic,     //3
    armenina2Ascii,//    Armenian,     //4
    acceptAll,//    Hebrew,       //5
    NULL,//    Arabic,       //6
    NULL,//    Syriac,       //7
    acceptAll,//    Thaana,       //8
    acceptAll,//    Devanagari,   //9
    acceptAll,//    Bengali,      //10
    acceptAll,//    Gurmukhi,     //11
    acceptAll,//    Gujarati,     //12
    acceptAll,//    Oriya,        //13
    acceptAll,//    Tamil,        //14
    acceptAll,//    Telugu,       //15
    acceptAll,//    Kannada,      //16
    acceptAll,//    Malayalam,    //17
    acceptAll,//    Sinhala,      //18
    acceptAll,//    Thai,         //19
    acceptAll,//    Lao,          //20
    acceptAll,//    Tibetan,      //21
    acceptAll,//    Myanmar,      //22
    NULL,//    Georgian,     //23
    acceptAll,//    Hangul,       //24
    acceptAll,//    Ogham,        //25
    acceptAll,//    Runic,        //26
    acceptAll,//    Khmer,        //27
    acceptAll,//    Nko,          //28
    NULL,//    Inherited,    //29
    NULL,//    ScriptCount,  //30
    NULL,//    Latin,        //31
    NULL,//    Ethiopic,     //32
    NULL,//    Cherokee,     //33
    NULL,//    CanadianAboriginal,   //34
    NULL,//    Mongolian,    //35
    NULL,//    Hiragana,     //36
    NULL,//    Katakana,     //37
    NULL,//    Bopomofo,     //38
    NULL,//    Han,          //39
    NULL,//    Yi,           //40
    NULL,//    OldItalic,    //41
    NULL,//    Gothic,       //42
    NULL,//    Deseret,      //43
    NULL,//    Tagalog,      //44
    NULL,//    Hanunoo,      //45
    NULL,//    Buhid,        //46
    NULL,//    Tagbanwa,     //47
    NULL,//    Limbu,        //48
    NULL,//    TaiLe,        //49
    NULL,//    LinearB,      //50
    NULL,//    Ugaritic,     //51
    NULL,//    Shavian,      //52
    NULL,//    Osmanya,      //53
    NULL,//    Cypriot,      //54
    NULL,//    Braille,      //55
    NULL,//    Buginese,     //56
    NULL,//    Coptic,       //57
    NULL,//    NewTaiLue,    //58
    NULL,//    Glagolitic,   //59
    NULL,//    Tifinagh,     //60
    NULL,//    SylotiNagri,  //61
    NULL,//    OldPersian,   //62
    NULL,//    Kharoshthi,   //63
    NULL,//    Balinese,     //64
    NULL,//    Cuneiform,    //65
    NULL,//    Phoenician,   //66
    NULL,//    PhagsPa       //67
};

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

    //Accept Currency characters
    if (Char.category() == QChar::Symbol_Currency)
        return this->LastChar = Char;

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

    if (Char.category() == QChar::Letter_Modifier ||
        Char.category() == QChar::Symbol_Modifier ||
        Char.category() == QChar::Other_NotAssigned ||
        Char.category() == QChar::Other_PrivateUse ||
        Char.category() == QChar::Mark_NonSpacing ||
        Char.category() == QChar::Other_Surrogate)
        return "";

    enuUnicodeCharScripts::Type CharacterScript = (enuUnicodeCharScripts::Type)QUnicodeTables::script(Char.unicode());

    //Check if there are sepcial normalizers
    if (ScriptBasedNormalizers[CharacterScript]){
        QString Normalized = ScriptBasedNormalizers[CharacterScript](Char.unicode());
        if (Normalized.size())
            return Normalized;
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
            std::cout<<"Press: (1: delete, 2: Accept, 3: Normalize, 4: Add as Space, 5: Add as ZWNJ...)"<<std::endl;
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
                    qCritical("Invalid normalization character. Modify config file manually");
                    continue;
                }
                this->ReplacingTable.insert(Char,TempBuffer.at(0));
                this->add2Configs(enuDicType::ReplacingCharacter, Char, TempBuffer.at(0));
                ValidSelection = true;
                break;
            }
            default:
            //    qCritical("Invalid Selection.");
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
    QFile WTFOut(this->ConfigFile);
    if(QFile::exists (this->ConfigFile + ".back"))
        QFile::remove(this->ConfigFile + ".back");
    if (!WTFOut.copy(this->ConfigFile + ".back"))
        throw exNormalizer("Unable to backup normalization file");

    QFile WTFIn(this->ConfigFile + ".back");
    WTFIn.open(QIODevice::ReadOnly);
    WTFOut.open(QIODevice::WriteOnly);

    if(!WTFIn.isReadable ())
        throw exNormalizer("Unable to open backup normalization file.");
    if(!WTFOut.isWritable ())
        throw exNormalizer("Unable to open output normalization file.");

    QByteArray DataLine;
    enuDicType::Type DicStep;
    bool SectionFound= false;

    while (!WTFIn.atEnd())
    {
        DataLine = WTFIn.readLine().trimmed();

        if (DataLine.startsWith("[") && DataLine.endsWith("]")){
            DicStep = enuDicType::toEnum(DataLine.mid(1,DataLine.size() - 2).constData());
            if (DicStep == enuDicType::Unknown)
                throw exNormalizer(QString("Invalid Dic Type: %1").arg(DataLine.constData()));
            else if (DicStep == _type)
                SectionFound=true;
            else if(SectionFound && DicStep != _type){
                switch(_type){
                case enuDicType::WhiteList:
                case enuDicType::RemovingCharcters:
                case enuDicType::SpaceCharacters:
                case enuDicType::ZeroWidthSpaceCharacters:
                {
                    QString ToBeWritten = this->char2Str(_originalChar);
                    if (ToBeWritten.startsWith("<0x"))
                        WTFOut.write(QString("NEW %1 ## {%2} [%3][%4]\n").arg(
                                         ToBeWritten).arg(
                                         _originalChar).arg(
                                         QCHAR_UNICOE_CATEGORIES[_originalChar.category()]).arg(
                                         enuUnicodeCharScripts::toStr(
                                            (enuUnicodeCharScripts::Type)
                                        QUnicodeTables::script(_originalChar.unicode()))).toUtf8());
                    else
                        WTFOut.write(("NEW " + ToBeWritten + "\n").toUtf8());
                }
                    break;
                case enuDicType::ReplacingCharacter:
                {
                    QString ToBeWrittenKey = this->char2Str(_originalChar);
                    QString ToBeWrittenVal = this->char2Str(_replacement);
                    if (ToBeWrittenKey.startsWith("<0x") || ToBeWrittenVal.startsWith("<0x"))
                        WTFOut.write(QString("NEW %1 = %2 ## {%3} ==> {%4}\n").arg(
                                         ToBeWrittenKey, ToBeWrittenVal).arg(
                                         _originalChar).arg(
                                         _replacement).toUtf8());
                    else
                        WTFOut.write(QString("NEW %1 = %2\n").arg(ToBeWrittenKey, ToBeWrittenVal).toUtf8());
                }
                    break;
                default:
                    throw exNormalizer("Invalid insertion Type");
                }
                SectionFound= false;
            }
        }
        WTFOut.write (DataLine + "\n");
    }


    WTFIn.close ();
    WTFOut.close ();
}

QString Normalizer::char2Str(const QChar &_char)
{
    if ((_char.isLetterOrNumber() || _char.toAscii() == _char) && _char != '=')
        return _char;
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
    if (_configFile.isEmpty()){
        this->ReplacingTable.clear();
        this->WhiteList.clear();
        this->RemovingList.clear();
        this->SpaceCharList.clear();
        this->ZeroWidthSpaceCharList.clear();
        return;
    }

    QFile ConfigFile(_configFile);
    ConfigFile.open(QIODevice::ReadOnly);
    if(!ConfigFile.isReadable ())
        throw exNormalizer("Unable to open normalization file.");

    QTextStream ConfigStream(&ConfigFile);
    ConfigStream.setCodec("UTF-8");
    this->ConfigFile = _configFile;

    QString ConfigLine;
    int CommentIndex = -1;
    int LineNumber = 0;
    enuDicType::Type DicStep = enuDicType::Unknown;
    bool IsEOF = false;

    QStringList ShorteningQuotes;

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
        case enuDicType::ApostrofAttached:
            StrList = ConfigLine.split(" ", QString::SkipEmptyParts);
            ShorteningQuotes.append(StrList);
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
    this->RxShorteningQuote = QRegExp(QString("(?: )('[%1])(?: )").arg(ShorteningQuotes.join("|")));
}

}
}
}
