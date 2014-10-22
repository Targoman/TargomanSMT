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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_UNICODE_HPP
#define TARGOMAN_NLPLIBS_PRIVATE_UNICODE_HPP

#include <QChar>
#include "libTargomanCommon/Macros.h"

//A hack to use script method from unicodeTales in Qt core
QT_BEGIN_NAMESPACE
namespace QUnicodeTables {
    extern int script(uint _ucs4);
}
QT_END_NAMESPACE

namespace Targoman {
namespace NLPLibs {
namespace Private {

//More info on http://www.fileformat.info/info/unicode/category
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
    return QChar(_ucs4);
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
    return QChar(_ucs4);
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
    return QChar(_ucs4);
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
    acceptAll,//    Braille,      //55
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

}
}
}
#endif // TARGOMAN_NLPLIBS_PRIVATE_UNICODE_HPP
