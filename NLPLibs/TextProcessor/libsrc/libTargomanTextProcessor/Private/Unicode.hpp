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

#include <QHash>
#include <QChar>
#include "libTargomanCommon/Macros.h"
#include "Normalizer.h"

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

typedef QHash<uint, QString> UnicodeNormalizationMap_t;
typedef QString (*ScriptBasedNormalizer)(uint);

UnicodeNormalizationMap_t UN_Greek;
UnicodeNormalizationMap_t UN_Cyrillic;
UnicodeNormalizationMap_t UN_Armeninan;
UnicodeNormalizationMap_t UN_Arabic;
UnicodeNormalizationMap_t UN_Latin;

inline QString acceptAll(uint _ucs4){
    return QChar(_ucs4);
}

inline QString unicodeNormalize(uint _ucs4, const UnicodeNormalizationMap_t& _map, bool _acceptRest = false){
    if(_map.contains(_ucs4))
        return _map.value(_ucs4);
    else if (_acceptRest)
        return QChar(_ucs4);
    else
        return "";
}

inline void unicodeAdd2Map(UnicodeNormalizationMap_t& _map, const QString _char, const QString& _norm){
    _map.insert(_char.at(0).unicode(), _norm);
}

QString DiacriticChars;
QStringList NormalizedDiacritics;

void initUnicodeNormalizers(){
    /////////////////////////////////////////////////////////////////
    ///                        Greek                              ///
    ///        http://www.unicode.org/charts/PDF/U0370.pdf        ///
    ///        http://www.unicode.org/charts/PDF/U1F00.pdf        ///
    /////////////////////////////////////////////////////////////////
    UN_Greek.clear();
    UN_Greek.insert(0x386,"A"); // Ά
    UN_Greek.insert(0x388,"E"); // Έ
    UN_Greek.insert(0x389,"H"); // Ή
    UN_Greek.insert(0x38A,"I"); // Ί
    UN_Greek.insert(0x38C,"O"); // Ό
    UN_Greek.insert(0x38E,"Y");

    UN_Greek.insert(0x390,"i");
    UN_Greek.insert(0x391,"A");
    UN_Greek.insert(0x392,"B");
    UN_Greek.insert(0x395,"E");
    UN_Greek.insert(0x396,"Z");
    UN_Greek.insert(0x397,"H");
    UN_Greek.insert(0x399,"I");
    UN_Greek.insert(0x39A,"K");
    UN_Greek.insert(0x39C,"M");
    UN_Greek.insert(0x39D,"N");

    UN_Greek.insert(0x3A1,"P");
    UN_Greek.insert(0x3A4,"T");
    UN_Greek.insert(0x3A5,"Y");
    UN_Greek.insert(0x3A7,"X");
    UN_Greek.insert(0x3AA,"I");
    UN_Greek.insert(0x3AB,"Y");
    UN_Greek.insert(0x3AF,"i");

    UN_Greek.insert(0x3B9,"i");
    UN_Greek.insert(0x3BA,"k");
    UN_Greek.insert(0x3BD,"v");
    UN_Greek.insert(0x3BF,"o");

    UN_Greek.insert(0x3CA,"i");
    UN_Greek.insert(0x3CB,"v");
    UN_Greek.insert(0x3CC,"o");
    UN_Greek.insert(0x3CD,"v");

    UN_Greek.insert(0x3DC,"F");

    UN_Greek.insert(0x3F3,"c");
    UN_Greek.insert(0x3FA,"M");
    UN_Greek.insert(0x3F9,"C");

    UN_Greek.insert(0x1F08,"A");
    UN_Greek.insert(0x1F09,"A");
    UN_Greek.insert(0x1F0A,"A");
    UN_Greek.insert(0x1F0B,"A");
    UN_Greek.insert(0x1F0C,"A");
    UN_Greek.insert(0x1F0D,"A");
    UN_Greek.insert(0x1F0E,"A");
    UN_Greek.insert(0x1F0F,"A");

    UN_Greek.insert(0x1F88,"A");
    UN_Greek.insert(0x1F89,"A");
    UN_Greek.insert(0x1F8A,"A");
    UN_Greek.insert(0x1F8B,"A");
    UN_Greek.insert(0x1F8C,"A");
    UN_Greek.insert(0x1F8D,"A");
    UN_Greek.insert(0x1F8E,"A");
    UN_Greek.insert(0x1F8F,"A");

    UN_Greek.insert(0x1FB8,"A");
    UN_Greek.insert(0x1FB9,"A");
    UN_Greek.insert(0x1FBA,"A");
    UN_Greek.insert(0x1FBB,"A");
    UN_Greek.insert(0x1FBC,"A");

    UN_Greek.insert(0x1F18,"E");
    UN_Greek.insert(0x1F19,"E");
    UN_Greek.insert(0x1F1A,"E");
    UN_Greek.insert(0x1F1B,"E");
    UN_Greek.insert(0x1F1C,"E");
    UN_Greek.insert(0x1F1D,"E");

    UN_Greek.insert(0x1FC8,"E");
    UN_Greek.insert(0x1FC9,"E");

    UN_Greek.insert(0x1F28,"H");
    UN_Greek.insert(0x1F29,"H");
    UN_Greek.insert(0x1F2A,"H");
    UN_Greek.insert(0x1F2B,"H");
    UN_Greek.insert(0x1F2C,"H");
    UN_Greek.insert(0x1F2D,"H");
    UN_Greek.insert(0x1F2E,"H");
    UN_Greek.insert(0x1F2F,"H");

    UN_Greek.insert(0x1F98,"H");
    UN_Greek.insert(0x1F99,"H");
    UN_Greek.insert(0x1F9A,"H");
    UN_Greek.insert(0x1F9B,"H");
    UN_Greek.insert(0x1F9C,"H");
    UN_Greek.insert(0x1F9D,"H");
    UN_Greek.insert(0x1F9E,"H");
    UN_Greek.insert(0x1F9F,"H");

    UN_Greek.insert(0x1FCA,"H");
    UN_Greek.insert(0x1FCB,"H");
    UN_Greek.insert(0x1FCC,"H");

    UN_Greek.insert(0x1F30,"i");
    UN_Greek.insert(0x1F31,"i");
    UN_Greek.insert(0x1F32,"i");
    UN_Greek.insert(0x1F33,"i");
    UN_Greek.insert(0x1F34,"i");
    UN_Greek.insert(0x1F35,"i");
    UN_Greek.insert(0x1F36,"i");
    UN_Greek.insert(0x1F37,"i");

    UN_Greek.insert(0x1F76,"i");
    UN_Greek.insert(0x1F77,"i");

    UN_Greek.insert(0x1FD0,"i");
    UN_Greek.insert(0x1FD1,"i");
    UN_Greek.insert(0x1FD2,"i");
    UN_Greek.insert(0x1FD3,"i");

    UN_Greek.insert(0x1FD6,"i");
    UN_Greek.insert(0x1FD7,"i");

    UN_Greek.insert(0x1F38,"I");
    UN_Greek.insert(0x1F39,"I");
    UN_Greek.insert(0x1F3A,"I");
    UN_Greek.insert(0x1F3B,"I");
    UN_Greek.insert(0x1F3C,"I");
    UN_Greek.insert(0x1F3D,"I");
    UN_Greek.insert(0x1F3E,"I");
    UN_Greek.insert(0x1F3F,"I");

    UN_Greek.insert(0x1FD8,"I");
    UN_Greek.insert(0x1FD9,"I");
    UN_Greek.insert(0x1FDA,"I");
    UN_Greek.insert(0x1FDB,"I");

    UN_Greek.insert(0x1F40,"o");
    UN_Greek.insert(0x1F41,"o");
    UN_Greek.insert(0x1F42,"o");
    UN_Greek.insert(0x1F43,"o");
    UN_Greek.insert(0x1F44,"o");
    UN_Greek.insert(0x1F45,"o");
    UN_Greek.insert(0x1F78,"o");
    UN_Greek.insert(0x1F79,"o");

    UN_Greek.insert(0x1F48,"O");
    UN_Greek.insert(0x1F49,"O");
    UN_Greek.insert(0x1F4A,"O");
    UN_Greek.insert(0x1F4B,"O");
    UN_Greek.insert(0x1F4C,"O");
    UN_Greek.insert(0x1F4D,"O");
    UN_Greek.insert(0x1FF8,"O");
    UN_Greek.insert(0x1FF9,"O");

    UN_Greek.insert(0x1F50,"v");
    UN_Greek.insert(0x1F51,"v");
    UN_Greek.insert(0x1F52,"v");
    UN_Greek.insert(0x1F53,"v");
    UN_Greek.insert(0x1F54,"v");
    UN_Greek.insert(0x1F55,"v");
    UN_Greek.insert(0x1F56,"v");
    UN_Greek.insert(0x1F57,"v");

    UN_Greek.insert(0x1F59,"Y");
    UN_Greek.insert(0x1F5B,"Y");
    UN_Greek.insert(0x1F5D,"Y");
    UN_Greek.insert(0x1F5F,"Y");
    UN_Greek.insert(0x1FE8,"Y");
    UN_Greek.insert(0x1FEB,"Y");

    /////////////////////////////////////////////////////////////////
    ///                       Cyrilic                             ///
    ///        http://www.unicode.org/charts/PDF/U0400.pdf        ///
    ///        http://www.unicode.org/charts/PDF/U0500.pdf        ///
    ///        http://www.unicode.org/charts/PDF/U2DE0.pdf        ///
    ///        http://www.unicode.org/charts/PDF/UA640.pdf        ///
    /////////////////////////////////////////////////////////////////
    UN_Cyrillic.insert(0x400,"E");
    UN_Cyrillic.insert(0x401,"E");
    UN_Cyrillic.insert(0x4D6,"E");
    UN_Cyrillic.insert(0x415,"E");

    UN_Cyrillic.insert(0x410,"A");
    UN_Cyrillic.insert(0x4D0,"A");
    UN_Cyrillic.insert(0x4D2,"A");

    UN_Cyrillic.insert(0x412,"B");
    UN_Cyrillic.insert(0x432,"B");

    UN_Cyrillic.insert(0x41A,"K");
    UN_Cyrillic.insert(0x43A,"K");
    UN_Cyrillic.insert(0x45C,"K");
    UN_Cyrillic.insert(0x51E,"K");
    UN_Cyrillic.insert(0x51F,"K");
    UN_Cyrillic.insert(0x49A,"K");
    UN_Cyrillic.insert(0x49B,"K");
    UN_Cyrillic.insert(0x49C,"K");
    UN_Cyrillic.insert(0x49D,"K");
    UN_Cyrillic.insert(0x49E,"K");
    UN_Cyrillic.insert(0x4A0,"K");
    UN_Cyrillic.insert(0x4A1,"K");

    UN_Cyrillic.insert(0x41C,"M");
    UN_Cyrillic.insert(0x43C,"M");

    UN_Cyrillic.insert(0x41D,"H");
    UN_Cyrillic.insert(0x43D,"H");

    UN_Cyrillic.insert(0x41E,"O");

    UN_Cyrillic.insert(0x420,"P");
    UN_Cyrillic.insert(0x48E,"P");
    UN_Cyrillic.insert(0x48F,"P");

    UN_Cyrillic.insert(0x421,"C");

    UN_Cyrillic.insert(0x422,"T");
    UN_Cyrillic.insert(0x442,"T");
    UN_Cyrillic.insert(0xA690,"T");
    UN_Cyrillic.insert(0xA691,"T");

    UN_Cyrillic.insert(0x423,"y");
    UN_Cyrillic.insert(0x443,"y");
    UN_Cyrillic.insert(0x45E,"y");
    UN_Cyrillic.insert(0x4EE,"y");
    UN_Cyrillic.insert(0x4EF,"y");
    UN_Cyrillic.insert(0x4F0,"y");
    UN_Cyrillic.insert(0x4F1,"y");
    UN_Cyrillic.insert(0x4F2,"y");
    UN_Cyrillic.insert(0x4F3,"y");

    UN_Cyrillic.insert(0x425,"X");
    UN_Cyrillic.insert(0x445,"X");

    UN_Cyrillic.insert(0x42C,"b");
    UN_Cyrillic.insert(0x44C,"b");

    UN_Cyrillic.insert(0x456,"i");
    UN_Cyrillic.insert(0x457,"i");

    UN_Cyrillic.insert(0x430,"a");

    UN_Cyrillic.insert(0x501,"d");

    UN_Cyrillic.insert(0x50C,"G");

    UN_Cyrillic.insert(0x51A,"Q");

    UN_Cyrillic.insert(0x51B,"q");

    UN_Cyrillic.insert(0x458,"j");

    UN_Cyrillic.insert(0x435,"e");
    UN_Cyrillic.insert(0x450,"e");
    UN_Cyrillic.insert(0x451,"e");

    UN_Cyrillic.insert(0x43E,"o");

    UN_Cyrillic.insert(0x440,"p");

    UN_Cyrillic.insert(0x441,"c");

    UN_Cyrillic.insert(0x405,"S");

    UN_Cyrillic.insert(0x445,"s");

    UN_Cyrillic.insert(0x4D4,"AE");

    UN_Cyrillic.insert(0x4D5,"ae");

    UN_Cyrillic.insert(0x460,"W");
    UN_Cyrillic.insert(0x51C,"W");

    UN_Cyrillic.insert(0x461,"w");
    UN_Cyrillic.insert(0x51D,"w");
    UN_Cyrillic.insert(0x49F,"w");

    UN_Cyrillic.insert(0x526,"h");
    UN_Cyrillic.insert(0x527,"h");

    UN_Cyrillic.insert(0xA640,"Z");
    UN_Cyrillic.insert(0xA641,"Z");
    UN_Cyrillic.insert(0xA642,"Z");
    UN_Cyrillic.insert(0xA643,"Z");

    /////////////////////////////////////////////////////////////////
    ///                       Armenian                            ///
    ///        http://www.unicode.org/charts/PDF/U0530.pdf        ///
    ///        http://www.unicode.org/charts/PDF/UFB00.pdf        ///
    /////////////////////////////////////////////////////////////////
    UN_Cyrillic.insert(0xFB00,"ff");

    UN_Cyrillic.insert(0xFB01,"fi");

    UN_Cyrillic.insert(0xFB02,"fl");

    UN_Cyrillic.insert(0xFB03,"ffi");

    UN_Cyrillic.insert(0xFB04,"ffl");

    UN_Cyrillic.insert(0xFB05,"st");
    UN_Cyrillic.insert(0xFB06,"st");

    /////////////////////////////////////////////////////////////////
    ///                        Latin                              ///
    /////////////////////////////////////////////////////////////////
    UN_Latin.insert(0xC0, "A");   // "À"
    UN_Latin.insert(0xC1, "A");   // "Á"
    UN_Latin.insert(0xC2, "A");   // "Â"
    UN_Latin.insert(0xC3, "A");   // "Ã"
    UN_Latin.insert(0xC4, "A");   // "Ä"
    UN_Latin.insert(0xC5, "A");   // "Å"
    UN_Latin.insert(0xC6, "AE");  // "Æ"
    UN_Latin.insert(0xC7, "C");   // "Ç"
    UN_Latin.insert(0xC8, "E");   // "È"
    UN_Latin.insert(0xC9, "E");   // "É"
    UN_Latin.insert(0xCA, "E");   // "Ê"
    UN_Latin.insert(0xCB, "E");   // "Ë"
    UN_Latin.insert(0xCC, "I");   // "Ì"
    UN_Latin.insert(0xCD, "I");   // "Í"
    UN_Latin.insert(0xCE, "I");   // "Î"
    UN_Latin.insert(0xCF, "I");   // "Ï"
    UN_Latin.insert(0xD0, "D");   // "Ð"
    UN_Latin.insert(0xD1, "N");   // "Ñ"
    UN_Latin.insert(0xD2, "O");   // "Ò"
    UN_Latin.insert(0xD3, "O");   // "Ó"
    UN_Latin.insert(0xD4, "O");   // "Ô"
    UN_Latin.insert(0xD5, "O");   // "Õ"
    UN_Latin.insert(0xD6, "O");   // "Ö"
    UN_Latin.insert(0xD9, "U");   // "Ù"
    UN_Latin.insert(0xDA, "U");   // "Ú"
    UN_Latin.insert(0xDB, "U");   // "Û"
    UN_Latin.insert(0xDC, "U");   // "Ü"
    UN_Latin.insert(0xDD, "Y");   // "Ý"
    UN_Latin.insert(0xE0, "a");   // "à"
    UN_Latin.insert(0xE1, "a");   // "á"
    UN_Latin.insert(0xE2, "a");   // "â"
    UN_Latin.insert(0xE3, "a");   // "ã"
    UN_Latin.insert(0xE4, "a");   // "ä"
    UN_Latin.insert(0xE5, "a");   // "å"
    UN_Latin.insert(0xE6, "ae");  // "æ"
    UN_Latin.insert(0xE7, "c");   // "ç"
    UN_Latin.insert(0xE8, "e");   // "è"
    UN_Latin.insert(0xE9, "e");   // "é"
    UN_Latin.insert(0xEA, "e");   // "ê"
    UN_Latin.insert(0xEB, "e");   // "ë"
    UN_Latin.insert(0xEC, "i");   // "ì"
    UN_Latin.insert(0xED, "i");   // "í"
    UN_Latin.insert(0xEE, "i");   // "î"
    UN_Latin.insert(0xEF, "i");   // "ï"
    UN_Latin.insert(0xF1, "n");   // "ñ"
    UN_Latin.insert(0xF2, "o");   // "ò"
    UN_Latin.insert(0xF3, "o");   // "ó"
    UN_Latin.insert(0xF4, "o");   // "ô"
    UN_Latin.insert(0xF5, "o");   // "õ"
    UN_Latin.insert(0xF6, "o");   // "ö"
    UN_Latin.insert(0xF9, "u");   // "ù"
    UN_Latin.insert(0xFA, "u");   // "ú"
    UN_Latin.insert(0xFB, "u");   // "û"
    UN_Latin.insert(0xFC, "u");   // "ü"
    UN_Latin.insert(0xFD, "y");   // "ý"
    UN_Latin.insert(0xFF, "y");   // "ÿ"
    UN_Latin.insert(0x100, "A");  // "Ā"
    UN_Latin.insert(0x101, "a");  // "ā"
    UN_Latin.insert(0x102, "A");  // "Ă"
    UN_Latin.insert(0x103, "a");  // "ă"
    UN_Latin.insert(0x104, "A");  // "Ą"
    UN_Latin.insert(0x105, "a");  // "ą"
    UN_Latin.insert(0x106, "C");  // "Ć"
    UN_Latin.insert(0x107, "c");  // "ć"
    UN_Latin.insert(0x108, "C");  // "Ĉ"
    UN_Latin.insert(0x109, "c");  // "ĉ"
    UN_Latin.insert(0x10A, "C");  // "Ċ"
    UN_Latin.insert(0x10B, "c");  // "ċ"
    UN_Latin.insert(0x10C, "C");  // "Č"
    UN_Latin.insert(0x10D, "c");  // "č"
    UN_Latin.insert(0x10E, "D");  // "Ď"
    UN_Latin.insert(0x10F, "d");  // "ď"
    UN_Latin.insert(0x110, "D");  // "Đ"
    UN_Latin.insert(0x111, "d");  // "đ"
    UN_Latin.insert(0x112, "E");  // "Ē"
    UN_Latin.insert(0x113, "e");  // "ē"
    UN_Latin.insert(0x114, "E");  // "Ĕ"
    UN_Latin.insert(0x115, "e");  // "ĕ"
    UN_Latin.insert(0x116, "E");  // "Ė"
    UN_Latin.insert(0x117, "e");  // "ė"
    UN_Latin.insert(0x118, "E");  // "Ę"
    UN_Latin.insert(0x119, "e");  // "ę"
    UN_Latin.insert(0x11A, "E");  // "Ě"
    UN_Latin.insert(0x11B, "e");  // "ě"
    UN_Latin.insert(0x11C, "G");  // "Ĝ"
    UN_Latin.insert(0x11D, "g");  // "ĝ"
    UN_Latin.insert(0x11E, "G");  // "Ğ"
    UN_Latin.insert(0x11F, "g");  // "ğ"
    UN_Latin.insert(0x120, "G");  // "Ġ"
    UN_Latin.insert(0x121, "g");  // "ġ"
    UN_Latin.insert(0x122, "G");  // "Ģ"
    UN_Latin.insert(0x123, "g");  // "ģ"
    UN_Latin.insert(0x124, "H");  // "Ĥ"
    UN_Latin.insert(0x125, "h");  // "ĥ"
    UN_Latin.insert(0x126, "H");  // "Ħ"
    UN_Latin.insert(0x127, "h");  // "ħ"
    UN_Latin.insert(0x128, "I");  // "Ĩ"
    UN_Latin.insert(0x129, "i");  // "ĩ"
    UN_Latin.insert(0x12A, "I");  // "Ī"
    UN_Latin.insert(0x12B, "i");  // "ī"
    UN_Latin.insert(0x12C, "I");  // "Ĭ"
    UN_Latin.insert(0x12D, "i");  // "ĭ"
    UN_Latin.insert(0x12E, "I");  // "Į"
    UN_Latin.insert(0x12F, "i");  // "į"
    UN_Latin.insert(0x130, "I");  // "İ"
    UN_Latin.insert(0x131, "i");  // "ı"
    UN_Latin.insert(0x132, "I");  // "Ĳ"
    UN_Latin.insert(0x133, "i");  // "ĳ"
    UN_Latin.insert(0x134, "J");  // "Ĵ"
    UN_Latin.insert(0x135, "j");  // "ĵ"
    UN_Latin.insert(0x136, "K");  // "Ķ"
    UN_Latin.insert(0x137, "k");  // "ķ"
    UN_Latin.insert(0x138, "k");  // "ĸ"
    UN_Latin.insert(0x139, "L");  // "Ĺ"
    UN_Latin.insert(0x13A, "l");  // "ĺ"
    UN_Latin.insert(0x13B, "L");  // "Ļ"
    UN_Latin.insert(0x13C, "l");  // "ļ"
    UN_Latin.insert(0x13D, "L");  // "Ľ"
    UN_Latin.insert(0x13E, "l");  // "ľ"
    UN_Latin.insert(0x13F, "L");  // "Ŀ"
    UN_Latin.insert(0x140, "l");  // "ŀ"
    UN_Latin.insert(0x141, "L");  // "Ł"
    UN_Latin.insert(0x142, "l");  // "ł"
    UN_Latin.insert(0x143, "N");  // "Ń"
    UN_Latin.insert(0x144, "n");  // "ń"
    UN_Latin.insert(0x145, "N");  // "Ņ"
    UN_Latin.insert(0x146, "n");  // "ņ"
    UN_Latin.insert(0x147, "N");  // "Ň"
    UN_Latin.insert(0x148, "n");  // "ň"
    UN_Latin.insert(0x149, "n");  // "ŉ"
    UN_Latin.insert(0x14C, "O");  // "Ō"
    UN_Latin.insert(0x14D, "o");  // "ō"
    UN_Latin.insert(0x14E, "O");  // "Ŏ"
    UN_Latin.insert(0x14F, "o");  // "ŏ"
    UN_Latin.insert(0x150, "O");  // "Ő"
    UN_Latin.insert(0x151, "o");  // "ő"
    UN_Latin.insert(0x152, "CE"); // "Œ"
    UN_Latin.insert(0x153, "ce"); // "œ"
    UN_Latin.insert(0x154, "R");  // "Ŕ"
    UN_Latin.insert(0x155, "r");  // "ŕ"
    UN_Latin.insert(0x156, "R");  // "Ŗ"
    UN_Latin.insert(0x157, "r");  // "ŗ"
    UN_Latin.insert(0x158, "R");  // "Ř"
    UN_Latin.insert(0x159, "r");  // "ř"
    UN_Latin.insert(0x15A, "S");  // "Ś"
    UN_Latin.insert(0x15B, "s");  // "ś"
    UN_Latin.insert(0x15C, "S");  // "Ŝ"
    UN_Latin.insert(0x15D, "s");  // "ŝ"
    UN_Latin.insert(0x15E, "S");  // "Ş"
    UN_Latin.insert(0x15F, "s");  // "ş"
    UN_Latin.insert(0x160, "S");  // "Š"
    UN_Latin.insert(0x161, "s");  // "š"
    UN_Latin.insert(0x162, "T");  // "Ţ"
    UN_Latin.insert(0x163, "t");  // "ţ"
    UN_Latin.insert(0x164, "T");  // "Ť"
    UN_Latin.insert(0x165, "t");  // "ť"
    UN_Latin.insert(0x166, "T");  // "Ŧ"
    UN_Latin.insert(0x167, "t");  // "ŧ"
    UN_Latin.insert(0x168, "U");  // "Ũ"
    UN_Latin.insert(0x169, "u");  // "ũ"
    UN_Latin.insert(0x16A, "U");  // "Ū"
    UN_Latin.insert(0x16B, "u");  // "ū"
    UN_Latin.insert(0x16C, "U");  // "Ŭ"
    UN_Latin.insert(0x16D, "u");  // "ŭ"
    UN_Latin.insert(0x16E, "U");  // "Ů"
    UN_Latin.insert(0x16F, "u");  // "ů"
    UN_Latin.insert(0x170, "U");  // "Ű"
    UN_Latin.insert(0x171, "u");  // "ű"
    UN_Latin.insert(0x172, "U");  // "Ų"
    UN_Latin.insert(0x173, "u");  // "ų"
    UN_Latin.insert(0x174, "W");  // "Ŵ"
    UN_Latin.insert(0x175, "w");  // "ŵ"
    UN_Latin.insert(0x176, "Y");  // "Ŷ"
    UN_Latin.insert(0x177, "y");  // "ŷ"
    UN_Latin.insert(0x178, "Y");  // "Ÿ"
    UN_Latin.insert(0x179, "Z");  // "Ź"
    UN_Latin.insert(0x17A, "z");  // "ź"
    UN_Latin.insert(0x17B, "Z");  // "Ż"
    UN_Latin.insert(0x17C, "z");  // "ż"
    UN_Latin.insert(0x17D, "Z");  // "Ž"
    UN_Latin.insert(0x17E, "z");  // "ž"
    UN_Latin.insert(0x180, "b");  // "ƀ"
    UN_Latin.insert(0x181, "B");  // "Ɓ"
    UN_Latin.insert(0x184, "b");  // "Ƅ"
    UN_Latin.insert(0x185, "b");  // "ƅ"
    UN_Latin.insert(0x187, "C");  // "Ƈ"
    UN_Latin.insert(0x188, "c");  // "ƈ"
    UN_Latin.insert(0x189, "D");  // "Ɖ"
    UN_Latin.insert(0x18A, "D");  // "Ɗ"
    UN_Latin.insert(0x18D, "o");  // "ƍ"
    UN_Latin.insert(0x192, "f");  // "ƒ"
    UN_Latin.insert(0x193, "G");  // "Ɠ"
    UN_Latin.insert(0x195, "hu"); // "ƕ"
    UN_Latin.insert(0x197, "I");  // "Ɨ"
    UN_Latin.insert(0x198, "K");  // "Ƙ"
    UN_Latin.insert(0x199, "k");  // "ƙ"
    UN_Latin.insert(0x19A, "l");  // "ƚ"
    UN_Latin.insert(0x1A0, "O");  // "Ơ"
    UN_Latin.insert(0x1A1, "o");  // "ơ"
    UN_Latin.insert(0x1A3, "oi"); // "ƣ"
    UN_Latin.insert(0x1A4, "P");  // "Ƥ"
    UN_Latin.insert(0x1A5, "p");  // "ƥ"
    UN_Latin.insert(0x1A6, "R");  // "Ʀ"
    UN_Latin.insert(0x1AB, "t");  // "ƫ"
    UN_Latin.insert(0x1AC, "T");  // "Ƭ"
    UN_Latin.insert(0x1AD, "t");  // "ƭ"
    UN_Latin.insert(0x1AE, "T");  // "Ʈ"
    UN_Latin.insert(0x1AF, "U");  // "Ư"
    UN_Latin.insert(0x1B0, "u");  // "ư"
    UN_Latin.insert(0x1B3, "Y");  // "Ƴ"
    UN_Latin.insert(0x1B4, "y");  // "ƴ"
    UN_Latin.insert(0x1B5, "Z");  // "Ƶ"
    UN_Latin.insert(0x1B6, "z");  // "ƶ"
    UN_Latin.insert(0x1C4, "DZ"); // "Ǆ"
    UN_Latin.insert(0x1C6, "dz"); // "ǆ"
    UN_Latin.insert(0x1C7, "LJ"); // "Ǉ"
    UN_Latin.insert(0x1C9, "lj"); // "ǉ"
    UN_Latin.insert(0x1CA, "NJ"); // "Ǌ"
    UN_Latin.insert(0x1CC, "nj"); // "ǌ"
    UN_Latin.insert(0x1CD, "A");  // "Ǎ"
    UN_Latin.insert(0x1CE, "a");  // "ǎ"
    UN_Latin.insert(0x1CF, "I");  // "Ǐ"
    UN_Latin.insert(0x1D0, "i");  // "ǐ"
    UN_Latin.insert(0x1D1, "O");  // "Ǒ"
    UN_Latin.insert(0x1D2, "o");  // "ǒ"
    UN_Latin.insert(0x1D3, "U");  // "Ǔ"
    UN_Latin.insert(0x1D4, "u");  // "ǔ"
    UN_Latin.insert(0x1D5, "U");  // "Ǖ"
    UN_Latin.insert(0x1D6, "u");  // "ǖ"
    UN_Latin.insert(0x1D7, "U");  // "Ǘ"
    UN_Latin.insert(0x1D8, "u");  // "ǘ"
    UN_Latin.insert(0x1D9, "U");  // "Ǚ"
    UN_Latin.insert(0x1DA, "u");  // "ǚ"
    UN_Latin.insert(0x1DB, "U");  // "Ǜ"
    UN_Latin.insert(0x1DC, "u");  // "ǜ"
    UN_Latin.insert(0x1DE, "A");  // "Ǟ"
    UN_Latin.insert(0x1DF, "a");  // "ǟ"
    UN_Latin.insert(0x1E0, "A");  // "Ǡ"
    UN_Latin.insert(0x1E1, "a");  // "ǡ"
    UN_Latin.insert(0x1E2, "AE"); // "Ǣ"
    UN_Latin.insert(0x1E3, "ae"); // "ǣ"
    UN_Latin.insert(0x1E6, "G");  // "Ǧ"
    UN_Latin.insert(0x1E7, "g");  // "ǧ"
    UN_Latin.insert(0x1E8, "K");  // "Ǩ"
    UN_Latin.insert(0x1E9, "k");  // "ǩ"
    UN_Latin.insert(0x1EA, "O");  // "Ǫ"
    UN_Latin.insert(0x1EB, "o");  // "ǫ"
    UN_Latin.insert(0x1EC, "O");  // "Ǭ"
    UN_Latin.insert(0x1ED, "o");  // "ǭ"
    UN_Latin.insert(0x1F0, "j");  // "ǰ"
    UN_Latin.insert(0x1F1, "DZ"); // "Ǳ"
    UN_Latin.insert(0x1F3, "dz"); // "ǳ"
    UN_Latin.insert(0x1F4, "G");  // "Ǵ"
    UN_Latin.insert(0x1F5, "g");  // "ǵ"
    UN_Latin.insert(0x1F6, "Hu"); // "Ƕ"
    UN_Latin.insert(0x1F8, "N");  // "Ǹ"
    UN_Latin.insert(0x1F9, "n");  // "ǹ"
    UN_Latin.insert(0x1FA, "A");  // "Ǻ"
    UN_Latin.insert(0x1FB, "a");  // "ǻ"
    UN_Latin.insert(0x1FC, "AE"); // "Ǽ"
    UN_Latin.insert(0x1FD, "ae"); // "ǽ"
    UN_Latin.insert(0x200, "A");  // "Ȁ"
    UN_Latin.insert(0x201, "a");  // "ȁ"
    UN_Latin.insert(0x202, "A");  // "Ȃ"
    UN_Latin.insert(0x203, "a");  // "ȃ"
    UN_Latin.insert(0x204, "E");  // "Ȅ"
    UN_Latin.insert(0x205, "e");  // "ȅ"
    UN_Latin.insert(0x206, "E");  // "Ȇ"
    UN_Latin.insert(0x207, "e");  // "ȇ"
    UN_Latin.insert(0x208, "I");  // "Ȉ"
    UN_Latin.insert(0x209, "i");  // "ȉ"
    UN_Latin.insert(0x20A, "I");  // "Ȋ"
    UN_Latin.insert(0x20B, "i");  // "ȋ"
    UN_Latin.insert(0x20C, "O");  // "Ȍ"
    UN_Latin.insert(0x20D, "o");  // "ȍ"
    UN_Latin.insert(0x20E, "O");  // "Ȏ"
    UN_Latin.insert(0x20F, "o");  // "ȏ"
    UN_Latin.insert(0x210, "R");  // "Ȑ"
    UN_Latin.insert(0x211, "r");  // "ȑ"
    UN_Latin.insert(0x212, "R");  // "Ȓ"
    UN_Latin.insert(0x213, "r");  // "ȓ"
    UN_Latin.insert(0x214, "U");  // "Ȕ"
    UN_Latin.insert(0x215, "u");  // "ȕ"
    UN_Latin.insert(0x216, "U");  // "Ȗ"
    UN_Latin.insert(0x217, "u");  // "ȗ"
    UN_Latin.insert(0x218, "S");  // "Ș"
    UN_Latin.insert(0x219, "s");  // "ș"
    UN_Latin.insert(0x21A, "T");  // "Ț"
    UN_Latin.insert(0x21B, "t");  // "ț"
    UN_Latin.insert(0x21E, "H");  // "Ȟ"
    UN_Latin.insert(0x21F, "h");  // "ȟ"
    UN_Latin.insert(0x221, "d");  // "ȡ"
    UN_Latin.insert(0x224, "Z");  // "Ȥ"
    UN_Latin.insert(0x225, "z");  // "ȥ"
    UN_Latin.insert(0x226, "A");  // "Ȧ"
    UN_Latin.insert(0x227, "a");  // "ȧ"
    UN_Latin.insert(0x228, "E");  // "Ȩ"
    UN_Latin.insert(0x229, "e");  // "ȩ"
    UN_Latin.insert(0x22A, "O");  // "Ȫ"
    UN_Latin.insert(0x22B, "o");  // "ȫ"
    UN_Latin.insert(0x22C, "O");  // "Ȭ"
    UN_Latin.insert(0x22D, "o");  // "ȭ"
    UN_Latin.insert(0x22E, "O");  // "Ȯ"
    UN_Latin.insert(0x22F, "o");  // "ȯ"
    UN_Latin.insert(0x230, "O");  // "Ȱ"
    UN_Latin.insert(0x231, "o");  // "ȱ"
    UN_Latin.insert(0x232, "Y");  // "Ȳ"
    UN_Latin.insert(0x233, "y");  // "ȳ"
    UN_Latin.insert(0x234, "l");  // "ȴ"
    UN_Latin.insert(0x235, "n");  // "ȵ"
    UN_Latin.insert(0x236, "t");  // "ȶ"
    UN_Latin.insert(0x237, "j");  // "ȷ"
    UN_Latin.insert(0x23A, "A");  // "Ⱥ"
    UN_Latin.insert(0x23B, "C");  // "Ȼ"
    UN_Latin.insert(0x23C, "c");  // "ȼ"
    UN_Latin.insert(0x23D, "L");  // "Ƚ"
    UN_Latin.insert(0x23E, "T");  // "Ⱦ"
    UN_Latin.insert(0x23F, "s");  // "ȿ"
    UN_Latin.insert(0x243, "B");  // "Ƀ"
    UN_Latin.insert(0x244, "U");  // "Ʉ"
    UN_Latin.insert(0x246, "E");  // "Ɇ"
    UN_Latin.insert(0x247, "e");  // "ɇ"
    UN_Latin.insert(0x248, "J");  // "Ɉ"
    UN_Latin.insert(0x249, "i");  // "ɉ"
    UN_Latin.insert(0x24A, "Q");  // "Ɋ"
    UN_Latin.insert(0x24B, "q");  // "ɋ"
    UN_Latin.insert(0x24C, "R");  // "Ɍ"
    UN_Latin.insert(0x24E, "Y");  // "Ɏ"
    UN_Latin.insert(0x24F, "y");  // "ɏ"
    UN_Latin.insert(0x253, "b");  // "ɓ"
    UN_Latin.insert(0x255, "c");  // "ɕ"
    UN_Latin.insert(0x256, "d");  // "ɖ"
    UN_Latin.insert(0x257, "d");  // "ɗ"
    UN_Latin.insert(0x260, "g");  // "ɠ"
    UN_Latin.insert(0x261, "g");  // "ɡ"
    UN_Latin.insert(0x262, "G");  // "ɢ"
    UN_Latin.insert(0x266, "h");  // "ɦ"
    UN_Latin.insert(0x268, "i");  // "ɨ"
    UN_Latin.insert(0x26A, "i");  // "ɪ"
    UN_Latin.insert(0x26B, "l");  // "ɫ"
    UN_Latin.insert(0x26C, "l");  // "ɬ"
    UN_Latin.insert(0x26D, "l");  // "ɭ"
    UN_Latin.insert(0x276, "oe"); // "ɶ"
    UN_Latin.insert(0x280, "r");  // "ʀ"
    UN_Latin.insert(0x28F, "y");  // "ʏ"
    UN_Latin.insert(0x290, "z");  // "ʐ"
    UN_Latin.insert(0x291, "z");  // "ʑ"
    UN_Latin.insert(0x29B, "g");  // "ʛ"
    UN_Latin.insert(0x29C, "h");  // "ʜ"
    UN_Latin.insert(0x29D, "j");  // "ʝ"
    UN_Latin.insert(0x29F, "l");  // "ʟ"
    UN_Latin.insert(0x2A3, "dz"); // "ʣ"
    UN_Latin.insert(0x2A5, "dz"); // "ʥ"
    UN_Latin.insert(0x2A6, "ts"); // "ʦ"
    UN_Latin.insert(0x2A8, "tc"); // "ʨ"
    UN_Latin.insert(0x2AB, "tz"); // "ʫ"

    UN_Latin.insert(0x2103,QString::fromUtf8("°C"));
    UN_Latin.insert(0x2109,QString::fromUtf8("°F"));
    UN_Latin.insert(0x210E, "h"); // "h"
    UN_Latin.insert(0x210F, "h"); // "h"
    UN_Latin.insert(0x2120, "SM"); // "SM"
    UN_Latin.insert(0x211E, "R"); // "SM"
    UN_Latin.insert(0x2121, "Tel"); // "SM"
    UN_Latin.insert(0x2122, "TM"); // "SM"
    UN_Latin.insert(0x212B, "A"); // "SM"

    UN_Latin.insert(0x2460, "(1)"); // ①
    UN_Latin.insert(0x2461, "(2)"); // ①
    UN_Latin.insert(0x2462, "(3)"); // ①
    UN_Latin.insert(0x2463, "(4)"); // ①
    UN_Latin.insert(0x2464, "(5)"); // ①
    UN_Latin.insert(0x2465, "(6)"); // ①
    UN_Latin.insert(0x2466, "(7)"); // ①
    UN_Latin.insert(0x2467, "(8)"); // ①
    UN_Latin.insert(0x2468, "(9)"); // ①
    UN_Latin.insert(0x2469, "(10)"); // ①
    UN_Latin.insert(0x246A, "(11)"); // ①
    UN_Latin.insert(0x246B, "(12)"); // ①
    UN_Latin.insert(0x246C, "(13)"); // ①
    UN_Latin.insert(0x246D, "(14)"); // ①
    UN_Latin.insert(0x246E, "(15)"); // ①
    UN_Latin.insert(0x246F, "(16)"); // ①
    UN_Latin.insert(0x2470, "(17)"); // ①
    UN_Latin.insert(0x2471, "(18)"); // ①
    UN_Latin.insert(0x2472, "(19)"); // ①
    UN_Latin.insert(0x2473, "(20)"); // ①
    UN_Latin.insert(0x2474, "(1)"); // ①
    UN_Latin.insert(0x2475, "(2)"); // ①
    UN_Latin.insert(0x2476, "(3)"); // ①
    UN_Latin.insert(0x2477, "(4)"); // ①
    UN_Latin.insert(0x2478, "(5)"); // ①
    UN_Latin.insert(0x2479, "(6)"); // ①
    UN_Latin.insert(0x247A, "(7)"); // ①
    UN_Latin.insert(0x247B, "(8)"); // ①
    UN_Latin.insert(0x247C, "(9)"); // ①
    UN_Latin.insert(0x247D, "(10)"); // ①
    UN_Latin.insert(0x247E, "(11)"); // ①
    UN_Latin.insert(0x247F, "(12)"); // ①
    UN_Latin.insert(0x2480, "(13)"); // ①
    UN_Latin.insert(0x2481, "(14)"); // ①
    UN_Latin.insert(0x2482, "(15)"); // ①
    UN_Latin.insert(0x2483, "(16)"); // ①
    UN_Latin.insert(0x2484, "(17)"); // ①
    UN_Latin.insert(0x2485, "(18)"); // ①
    UN_Latin.insert(0x2486, "(19)"); // ①
    UN_Latin.insert(0x2487, "(20)"); // ①

    UN_Latin.insert(0x2488, "1"); // ①
    UN_Latin.insert(0x2489, "2"); // ①
    UN_Latin.insert(0x248A, "3"); // ①
    UN_Latin.insert(0x248B, "4"); // ①
    UN_Latin.insert(0x248C, "5"); // ①
    UN_Latin.insert(0x248D, "6"); // ①
    UN_Latin.insert(0x248E, "7"); // ①
    UN_Latin.insert(0x248F, "8"); // ①
    UN_Latin.insert(0x2490, "9"); // ①
    UN_Latin.insert(0x2491, "(10)"); // ①
    UN_Latin.insert(0x2492, "(11)"); // ①
    UN_Latin.insert(0x2493, "(12)"); // ①
    UN_Latin.insert(0x2494, "(13)"); // ①
    UN_Latin.insert(0x2495, "(14)"); // ①
    UN_Latin.insert(0x2496, "(15)"); // ①
    UN_Latin.insert(0x2497, "(16)"); // ①
    UN_Latin.insert(0x2498, "(17)"); // ①
    UN_Latin.insert(0x2499, "(18)"); // ①
    UN_Latin.insert(0x249A, "(19)"); // ①
    UN_Latin.insert(0x249B, "(20)"); // ①

    UN_Latin.insert(0x249C, "(a)"); // ①
    UN_Latin.insert(0x249D, "(b)"); // ①
    UN_Latin.insert(0x249E, "(c)"); // ①
    UN_Latin.insert(0x249F, "(d)"); // ①
    UN_Latin.insert(0x24A0, "(e)"); // ①
    UN_Latin.insert(0x24A1, "(f)"); // ①
    UN_Latin.insert(0x24A2, "(g)"); // ①
    UN_Latin.insert(0x24A3, "(h)"); // ①
    UN_Latin.insert(0x24A4, "(i)"); // ①
    UN_Latin.insert(0x24A5, "(j)"); // ①
    UN_Latin.insert(0x24A6, "(k)"); // ①
    UN_Latin.insert(0x24A7, "(l)"); // ①
    UN_Latin.insert(0x24A8, "(m)"); // ①
    UN_Latin.insert(0x24A9, "(n)"); // ①
    UN_Latin.insert(0x24AA, "(o)"); // ①
    UN_Latin.insert(0x24AB, "(p)"); // ①
    UN_Latin.insert(0x24AC, "(q)"); // ①
    UN_Latin.insert(0x24AD, "(r)"); // ①
    UN_Latin.insert(0x24AE, "(s)"); // ①
    UN_Latin.insert(0x24AF, "(t)"); // ①
    UN_Latin.insert(0x24B0, "(u)"); // ①
    UN_Latin.insert(0x24B1, "(v)"); // ①
    UN_Latin.insert(0x24B2, "(w)"); // ①
    UN_Latin.insert(0x24B3, "(x)"); // ①
    UN_Latin.insert(0x24B4, "(y)"); // ①
    UN_Latin.insert(0x24B5, "(z)"); // ①

    UN_Latin.insert(0x24B6, "(A)"); // ①
    UN_Latin.insert(0x24B7, "(B)"); // ①
    UN_Latin.insert(0x24B8, "(C)"); // ①
    UN_Latin.insert(0x24B9, "(D)"); // ①
    UN_Latin.insert(0x24BA, "(E)"); // ①
    UN_Latin.insert(0x24BB, "(F)"); // ①
    UN_Latin.insert(0x24BC, "(G)"); // ①
    UN_Latin.insert(0x24BD, "(H)"); // ①
    UN_Latin.insert(0x24BE, "(I)"); // ①
    UN_Latin.insert(0x24BF, "(J)"); // ①
    UN_Latin.insert(0x24C0, "(K)"); // ①
    UN_Latin.insert(0x24C1, "(L)"); // ①
    UN_Latin.insert(0x24C2, "(M)"); // ①
    UN_Latin.insert(0x24C3, "(N)"); // ①
    UN_Latin.insert(0x24C4, "(O)"); // ①
    UN_Latin.insert(0x24C5, "(P)"); // ①
    UN_Latin.insert(0x24C6, "(Q)"); // ①
    UN_Latin.insert(0x24C7, "(R)"); // ①
    UN_Latin.insert(0x24C8, "(S)"); // ①
    UN_Latin.insert(0x24C9, "(T)"); // ①
    UN_Latin.insert(0x24CA, "(U)"); // ①
    UN_Latin.insert(0x24CB, "(V)"); // ①
    UN_Latin.insert(0x24CC, "(W)"); // ①
    UN_Latin.insert(0x24CD, "(X)"); // ①
    UN_Latin.insert(0x24CE, "(Y)"); // ①
    UN_Latin.insert(0x24CF, "(Z)"); // ①

    UN_Latin.insert(0x24D0, "(a)"); // ①
    UN_Latin.insert(0x24D1, "(b)"); // ①
    UN_Latin.insert(0x24D2, "(c)"); // ①
    UN_Latin.insert(0x24D3, "(d)"); // ①
    UN_Latin.insert(0x24D4, "(e)"); // ①
    UN_Latin.insert(0x24D5, "(f)"); // ①
    UN_Latin.insert(0x24D6, "(g)"); // ①
    UN_Latin.insert(0x24D7, "(h)"); // ①
    UN_Latin.insert(0x24D8, "(i)"); // ①
    UN_Latin.insert(0x24D9, "(j)"); // ①
    UN_Latin.insert(0x24DA, "(k)"); // ①
    UN_Latin.insert(0x24DB, "(l)"); // ①
    UN_Latin.insert(0x24DC, "(m)"); // ①
    UN_Latin.insert(0x24DD, "(n)"); // ①
    UN_Latin.insert(0x24DE, "(o)"); // ①
    UN_Latin.insert(0x24DF, "(p)"); // ①
    UN_Latin.insert(0x24E0, "(q)"); // ①
    UN_Latin.insert(0x24E1, "(r)"); // ①
    UN_Latin.insert(0x24E2, "(s)"); // ①
    UN_Latin.insert(0x24E3, "(t)"); // ①
    UN_Latin.insert(0x24E4, "(u)"); // ①
    UN_Latin.insert(0x24E5, "(v)"); // ①
    UN_Latin.insert(0x24E6, "(w)"); // ①
    UN_Latin.insert(0x24E7, "(x)"); // ①
    UN_Latin.insert(0x24E8, "(y)"); // ①
    UN_Latin.insert(0x24E9, "(z)"); // ①

    UN_Latin.insert(0x24EA, "(0)"); // ①

    UN_Latin.insert(0x24EB, "(11)"); // ①
    UN_Latin.insert(0x24EC, "(12)"); // ①
    UN_Latin.insert(0x24ED, "(13)"); // ①
    UN_Latin.insert(0x24EE, "(14)"); // ①
    UN_Latin.insert(0x24EF, "(15)"); // ①
    UN_Latin.insert(0x24F0, "(16)"); // ①
    UN_Latin.insert(0x24F1, "(17)"); // ①
    UN_Latin.insert(0x24F2, "(18)"); // ①
    UN_Latin.insert(0x24F3, "(19)"); // ①
    UN_Latin.insert(0x24F4, "(20)"); // ①

    UN_Latin.insert(0x24F5, "1"); // ①
    UN_Latin.insert(0x24F6, "2"); // ①
    UN_Latin.insert(0x24F7, "3"); // ①
    UN_Latin.insert(0x24F8, "4"); // ①
    UN_Latin.insert(0x24F9, "5"); // ①
    UN_Latin.insert(0x24FA, "6"); // ①
    UN_Latin.insert(0x24FB, "7"); // ①
    UN_Latin.insert(0x24FC, "8"); // ①
    UN_Latin.insert(0x24FD, "9"); // ①

    UN_Latin.insert(0x24FE, "(10)"); // ①
    UN_Latin.insert(0x24FF, "(0)"); // ①

    UN_Latin.insert(0x2721, SYMBOL_REMOVED);

    UN_Latin.insert(0x2753, "?"); // ①
    UN_Latin.insert(0x2754, "?"); // ①

    UN_Latin.insert(0x2755, "!");
    UN_Latin.insert(0x2756, "*");
    UN_Latin.insert(0x2757, "!");
    UN_Latin.insert(0x2758, "|");
    UN_Latin.insert(0x2759, "|");
    UN_Latin.insert(0x275A, "|");

    UN_Latin.insert(0x275B, "'");
    UN_Latin.insert(0x275C, "'");

    UN_Latin.insert(0x275D, "\"");
    UN_Latin.insert(0x275E, "\"");

    UN_Latin.insert(0x275F, "'");
    UN_Latin.insert(0x2760, "\"");

    UN_Latin.insert(0x2762, "!");
    UN_Latin.insert(0x2763, "!");

    UN_Latin.insert(0x2776, "(1)"); // ①
    UN_Latin.insert(0x2777, "(2)"); // ①
    UN_Latin.insert(0x2778, "(3)"); // ①
    UN_Latin.insert(0x2779, "(4)"); // ①
    UN_Latin.insert(0x277A, "(5)"); // ①
    UN_Latin.insert(0x277B, "(6)"); // ①
    UN_Latin.insert(0x277C, "(7)"); // ①
    UN_Latin.insert(0x277D, "(8)"); // ①
    UN_Latin.insert(0x277E, "(9)"); // ①
    UN_Latin.insert(0x277F, "(10)"); // ①

    UN_Latin.insert(0x2780, "(1)"); // ①
    UN_Latin.insert(0x2781, "(2)"); // ①
    UN_Latin.insert(0x2782, "(3)"); // ①
    UN_Latin.insert(0x2783, "(4)"); // ①
    UN_Latin.insert(0x2784, "(5)"); // ①
    UN_Latin.insert(0x2785, "(6)"); // ①
    UN_Latin.insert(0x2786, "(7)"); // ①
    UN_Latin.insert(0x2787, "(8)"); // ①
    UN_Latin.insert(0x2788, "(9)"); // ①
    UN_Latin.insert(0x2789, "(10)"); // ①

    UN_Latin.insert(0x278A, "(1)"); // ①
    UN_Latin.insert(0x278B, "(2)"); // ①
    UN_Latin.insert(0x278C, "(3)"); // ①
    UN_Latin.insert(0x278D, "(4)"); // ①
    UN_Latin.insert(0x278E, "(5)"); // ①
    UN_Latin.insert(0x278F, "(6)"); // ①
    UN_Latin.insert(0x2790, "(7)"); // ①
    UN_Latin.insert(0x2791, "(8)"); // ①
    UN_Latin.insert(0x2792, "(9)"); // ①
    UN_Latin.insert(0x2793, "(10)"); // ①

    UN_Latin.insert(0x2795, "+"); // ①
    UN_Latin.insert(0x2796, "-"); // ①

    UN_Latin.insert(0x2C60, "L");
    UN_Latin.insert(0x2C61, "l");
    UN_Latin.insert(0x2C62, "L");
    UN_Latin.insert(0x2C63, "P");
    UN_Latin.insert(0x2C64, "R");
    UN_Latin.insert(0x2C65, "a");
    UN_Latin.insert(0x2C66, "t");
    UN_Latin.insert(0x2C67, "H");
    UN_Latin.insert(0x2C68, "h");
    UN_Latin.insert(0x2C69, "K");
    UN_Latin.insert(0x2C6A, "k");
    UN_Latin.insert(0x2C6B, "Z");
    UN_Latin.insert(0x2C6C, "z");

    UN_Latin.insert(0x2C6E, "M");

    UN_Latin.insert(0x2C71, "v");
    UN_Latin.insert(0x2C72, "W");
    UN_Latin.insert(0x2C73, "w");
    UN_Latin.insert(0x2C74, "v");
    UN_Latin.insert(0x2C75, SYMBOL_REMOVED);
    UN_Latin.insert(0x2C76, SYMBOL_REMOVED);
    UN_Latin.insert(0x2C77, "ⱷ");
    UN_Latin.insert(0x2C78, "e");
    UN_Latin.insert(0x2C7C, QString::fromUtf8("ⱼ"));
    UN_Latin.insert(0x2C7D, QString::fromUtf8("ⱽ"));
    UN_Latin.insert(0x2C7E, "S");
    UN_Latin.insert(0x2C7F, "Z");


    /////////////////////////////////////////////////////////////////
    ///                          Arabic                           ///
    ///        http://www.unicode.org/charts/PDF/U0600.pdf        ///
    ///        http://www.unicode.org/charts/PDF/U0750.pdf        ///
    ///        http://www.unicode.org/charts/PDF/U08A0.pdf        ///
    ///        http://www.unicode.org/charts/PDF/UFB50.pdf        ///
    ///        http://www.unicode.org/charts/PDF/UFE70.pdf        ///
    /////////////////////////////////////////////////////////////////
    UN_Arabic.insert(0x672,QString::fromUtf8("أ"));
    UN_Arabic.insert(0x675,QString::fromUtf8("أ"));

    UN_Arabic.insert(0x673,QString::fromUtf8("إ"));

    UN_Arabic.insert(0x66E,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x67B,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x680,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x750,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x752,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x754,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x755,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x756,QString::fromUtf8("ب"));

    UN_Arabic.insert(0x679,QString::fromUtf8("ت"));
    UN_Arabic.insert(0x67A,QString::fromUtf8("ت"));
    UN_Arabic.insert(0x67C,QString::fromUtf8("ت"));
    UN_Arabic.insert(0x67F,QString::fromUtf8("ت"));
    UN_Arabic.insert(0x753,QString::fromUtf8("ت"));

    UN_Arabic.insert(0x67D,QString::fromUtf8("ث"));
    UN_Arabic.insert(0x751,QString::fromUtf8("ث"));

    UN_Arabic.insert(0x683,QString::fromUtf8("ج"));
    UN_Arabic.insert(0x684,QString::fromUtf8("ج"));

    UN_Arabic.insert(0x687,QString::fromUtf8("چ"));
    UN_Arabic.insert(0x6BF,QString::fromUtf8("چ"));

    UN_Arabic.insert(0x681,QString::fromUtf8("ح"));
    UN_Arabic.insert(0x682,QString::fromUtf8("ح"));
    UN_Arabic.insert(0x685,QString::fromUtf8("ح"));
    UN_Arabic.insert(0x757,QString::fromUtf8("ح"));
    UN_Arabic.insert(0x758,QString::fromUtf8("ح"));
    UN_Arabic.insert(0x76D,QString::fromUtf8("ح"));
    UN_Arabic.insert(0x76E,QString::fromUtf8("ح"));

    UN_Arabic.insert(0x688,QString::fromUtf8("د"));
    UN_Arabic.insert(0x689,QString::fromUtf8("د"));
    UN_Arabic.insert(0x68A,QString::fromUtf8("د"));
    UN_Arabic.insert(0x68B,QString::fromUtf8("د"));
    UN_Arabic.insert(0x68D,QString::fromUtf8("د"));
    UN_Arabic.insert(0x68E,QString::fromUtf8("د"));
    UN_Arabic.insert(0x68F,QString::fromUtf8("د"));
    UN_Arabic.insert(0x690,QString::fromUtf8("د"));
    UN_Arabic.insert(0x6EE,QString::fromUtf8("د"));
    UN_Arabic.insert(0x759,QString::fromUtf8("د"));
    UN_Arabic.insert(0x75A,QString::fromUtf8("د"));

    UN_Arabic.insert(0x68C,QString::fromUtf8("ذ"));

    UN_Arabic.insert(0x691,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x692,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x693,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x694,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x695,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x696,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x6EF,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x75B,QString::fromUtf8("ر"));

    UN_Arabic.insert(0x697,QString::fromUtf8("ز"));
    UN_Arabic.insert(0x76B,QString::fromUtf8("ز"));
    UN_Arabic.insert(0x76C,QString::fromUtf8("ز"));

    UN_Arabic.insert(0x699,QString::fromUtf8("ژ"));

    UN_Arabic.insert(0x69A,QString::fromUtf8("س"));
    UN_Arabic.insert(0x69B,QString::fromUtf8("س"));
    UN_Arabic.insert(0x75C,QString::fromUtf8("س"));
    UN_Arabic.insert(0x76C,QString::fromUtf8("س"));

    UN_Arabic.insert(0x69C,QString::fromUtf8("ش"));
    UN_Arabic.insert(0x6FA,QString::fromUtf8("ش"));

    UN_Arabic.insert(0x69D,QString::fromUtf8("ص"));
    UN_Arabic.insert(0x69E,QString::fromUtf8("ص"));

    UN_Arabic.insert(0x6FB,QString::fromUtf8("ض"));

    UN_Arabic.insert(0x69F,QString::fromUtf8("ط"));

    UN_Arabic.insert(0x6A0,QString::fromUtf8("ع"));
    UN_Arabic.insert(0x75D,QString::fromUtf8("ع"));
    UN_Arabic.insert(0x75E,QString::fromUtf8("ع"));
    UN_Arabic.insert(0x75F,QString::fromUtf8("ع"));

    UN_Arabic.insert(0x6FC,QString::fromUtf8("غ"));

    UN_Arabic.insert(0x6A1,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x6A2,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x6A3,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x6A4,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x6A5,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x6A6,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x760,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x761,QString::fromUtf8("ف"));

    UN_Arabic.insert(0x66F,QString::fromUtf8("ق"));
    UN_Arabic.insert(0x6A7,QString::fromUtf8("ق"));
    UN_Arabic.insert(0x6A8,QString::fromUtf8("ق"));

    UN_Arabic.insert(0x643,QString::fromUtf8("ک"));
    UN_Arabic.insert(0x63B,QString::fromUtf8("ک"));
    UN_Arabic.insert(0x63C,QString::fromUtf8("ک"));
    UN_Arabic.insert(0x6A9,QString::fromUtf8("ک"));
    UN_Arabic.insert(0x6AA,QString::fromUtf8("ک"));
    UN_Arabic.insert(0x6AB,QString::fromUtf8("ک"));
    UN_Arabic.insert(0x6AC,QString::fromUtf8("ک"));
    UN_Arabic.insert(0x6AD,QString::fromUtf8("ک"));
    UN_Arabic.insert(0x6AE,QString::fromUtf8("ک"));
    UN_Arabic.insert(0x762,QString::fromUtf8("ک"));
    UN_Arabic.insert(0x763,QString::fromUtf8("ک"));
    UN_Arabic.insert(0x764,QString::fromUtf8("ک"));
    UN_Arabic.insert(0x76F,QString::fromUtf8("ک"));

    UN_Arabic.insert(0x6B0,QString::fromUtf8("گ"));
    UN_Arabic.insert(0x6B1,QString::fromUtf8("گ"));
    UN_Arabic.insert(0x6B2,QString::fromUtf8("گ"));
    UN_Arabic.insert(0x6B3,QString::fromUtf8("گ"));
    UN_Arabic.insert(0x6B4,QString::fromUtf8("گ"));

    UN_Arabic.insert(0x6B5,QString::fromUtf8("ل"));
    UN_Arabic.insert(0x6B6,QString::fromUtf8("ل"));
    UN_Arabic.insert(0x6B7,QString::fromUtf8("ل"));
    UN_Arabic.insert(0x6B8,QString::fromUtf8("ل"));
    UN_Arabic.insert(0x76A,QString::fromUtf8("ل"));

    UN_Arabic.insert(0x6FE,QString::fromUtf8("م"));
    UN_Arabic.insert(0x765,QString::fromUtf8("م"));
    UN_Arabic.insert(0x766,QString::fromUtf8("م"));

    UN_Arabic.insert(0x6B9,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x6BA,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x6BB,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x6BC,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x6BD,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x767,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x768,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x769,QString::fromUtf8("ن"));

    UN_Arabic.insert(0x676,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0x677,QString::fromUtf8("ؤ"));

    UN_Arabic.insert(0x6C4,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6C5,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6C6,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6C7,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6C8,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6C9,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6CA,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6CB,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6CF,QString::fromUtf8("و"));

    UN_Arabic.insert(0x6BE,QString::fromUtf8("ه"));
    UN_Arabic.insert(0x6FF,QString::fromUtf8("ه"));
    UN_Arabic.insert(0x6C0,QString::fromUtf8("ه"));
    UN_Arabic.insert(0x6C1,QString::fromUtf8("ه"));
    UN_Arabic.insert(0x6C2,QString::fromUtf8("ه"));
    UN_Arabic.insert(0x6C3,QString::fromUtf8("ه"));
    UN_Arabic.insert(0x6D5,QString::fromUtf8("ه"));

    UN_Arabic.insert(0x620,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x63D,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x63E,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x63F,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x649,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x64A,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x678,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x6CC,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x6CD,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x6CE,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x6D0,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x6D1,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x6D2,QString::fromUtf8("ی"));
    UN_Arabic.insert(0x6D3,QString::fromUtf8("ی"));


    UN_Arabic.insert(0x6DD,QString(QChar(0x2205))); // SYMBOL_REMOVED
    UN_Arabic.insert(0x6DE,QString(QChar(0x2205))); // SYMBOL_REMOVED
    UN_Arabic.insert(0x6E9,QString(QChar(0x2205))); // SYMBOL_REMOVED

}

inline QString greek2Ascii(uint _ucs4){
    return unicodeNormalize(_ucs4, UN_Greek, true);
}


QString cyrillic2Ascii(uint _ucs4){
    return unicodeNormalize(_ucs4, UN_Cyrillic, true);
}

QString armenina2Ascii(uint _ucs4){
    return unicodeNormalize(_ucs4, UN_Armeninan, true);
}

QString latin2Ascii(uint _ucs4){
    ///@ref http://www.unicode.org/charts/PDF/U2070.pdf
    if (_ucs4>=0x2070 && _ucs4 <=0x209F)
        return QString(QChar(_ucs4));

    ///@ref Coptic
    if (_ucs4>=0x2C80 && _ucs4 <=0x2CFF)
        return QString(QChar(_ucs4));

    ///@ref TIFINAGH
    if (_ucs4>=0x2D30 && _ucs4 <=0x2DFF)
        return QString(QChar(_ucs4));

    if (_ucs4>=0x2C00 && _ucs4 <=0x2C5F)
        return QString(QChar(_ucs4));

    ///@ref HIRGANA & KATANKA & Bopomofo
    if ((_ucs4>=0x3041 && _ucs4 <=0x312F) ||
        (_ucs4>=0x31A0 && _ucs4<=0x31BF) ||
        (_ucs4>=0x31F0 && _ucs4<=0x31FF))
        return QString(QChar(_ucs4));

    ///@ref CJK
    if (_ucs4>=0x3400 && _ucs4 <=0x9FFF)
        return QString(QChar(_ucs4));

    if(UN_Latin.contains(_ucs4))
        return UN_Latin.value(_ucs4);

    if((_ucs4>=0x2719 && _ucs4<=0x2725))
        return "+";

    if((_ucs4>=0x2719 && _ucs4<=0x2725))
        return "+";

    if((_ucs4>=0x2715 && _ucs4<=0x2718) ||
       (_ucs4>=0x2726 && _ucs4<=0x274C))
        return "*";


    if((_ucs4>=0x274D && _ucs4<=0x2752) ||
       (_ucs4>=0x2794 && _ucs4<=0x27BE) ||
       (_ucs4>=0x3190 && _ucs4<=0x319F))
        return SYMBOL_REMOVED;


    if ((_ucs4>=0x80 && _ucs4<=0xFF) ||
        (_ucs4>=0x100 && _ucs4<=0x17F) ||
        (_ucs4>=0x14A && _ucs4<=0x14B) ||
        (_ucs4>=0x180 && _ucs4<=0x24F) ||
        (_ucs4>=0x250 && _ucs4<=0x2AF) ||
        (_ucs4>=0x370 && _ucs4<=0x3FF) ||
        (_ucs4>=0x3200 && _ucs4<=0x32FF) ||
        (_ucs4>=0x2100 && _ucs4<=0x214F))
        return QString(QChar(_ucs4));

    return "";
}

QString arabicNormalizer(uint _ucs4){
    return unicodeNormalize(_ucs4, UN_Arabic, false);
}

const ScriptBasedNormalizer ScriptBasedNormalizers[]={
    latin2Ascii,//    Common,       //0
    greek2Ascii,//    Greek,        //1
    cyrillic2Ascii,//    Cyrillic,     //3
    armenina2Ascii,//    Armenian,     //4
    acceptAll,//    Hebrew,       //5
    arabicNormalizer,//    Arabic,       //6
    acceptAll,//    Syriac,       //7
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
    acceptAll,//    Georgian,     //23
    acceptAll,//    Hangul,       //24
    acceptAll,//    Ogham,        //25
    acceptAll,//    Runic,        //26
    acceptAll,//    Khmer,        //27
    acceptAll,//    Nko,          //28
    NULL,//    Inherited,    //29
    NULL,//    ScriptCount,  //30
    latin2Ascii,//    Latin,        //31
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
