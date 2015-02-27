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
 @author S. Saeed Torabzadeh <saeed.torabzadeh@gmail.com>
 */

#ifndef TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_UNICODE_HPP
#define TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_UNICODE_HPP

#include <QHash>
#include <QChar>
#include "libTargomanCommon/Macros.h"
#include "Normalizer.h"

//A hack to use script method from unicodeTales in Qt core
/*QT_BEGIN_NAMESPACE
namespace QUnicodeTables {
    extern int script(uint _ucs4);
}
QT_END_NAMESPACE
*/

namespace Targoman {
namespace NLPLibs {
namespace TargomanTP{
namespace Private {

static QMap<QChar::Category, std::string> QCharCategorytToStringMap {
    {QChar::Mark_NonSpacing, "QChar::Mark_NonSpacing"},
    {QChar::Mark_SpacingCombining, "QChar::Mark_SpacingCombining"},
    {QChar::Mark_Enclosing, "QChar::Mark_Enclosing"},
    {QChar::Number_DecimalDigit, "QChar::Number_DecimalDigit"},
    {QChar::Number_Letter, "QChar::Number_Letter"},
    {QChar::Number_Other, "QChar::Number_Other"},
    {QChar::Separator_Space, "QChar::Separator_Space"},
    {QChar::Separator_Line, "QChar::Separator_Line"},
    {QChar::Separator_Paragraph, "QChar::Separator_Paragraph"},
    {QChar::Other_Control, "QChar::Other_Control"},
    {QChar::Other_Format, "QChar::Other_Format"},
    {QChar::Other_Surrogate, "QChar::Other_Surrogate"},
    {QChar::Other_PrivateUse, "QChar::Other_PrivateUse"},
    {QChar::Other_NotAssigned, "QChar::Other_NotAssigned"}
};

static QMap<QChar::Script, std::string> QCharScriptToStringMap {
    {QChar::Script_Unknown,"Unknown"},
    {QChar::Script_Inherited,"Inherited"},
    {QChar::Script_Common,"Common"},
    {QChar::Script_Latin,"Latin"},
    {QChar::Script_Greek,"Greek"},
    {QChar::Script_Cyrillic,"Cyrillic"},
    {QChar::Script_Armenian,"Armenian"},
    {QChar::Script_Hebrew,"Hebrew"},
    {QChar::Script_Arabic,"Arabic"},
    {QChar::Script_Syriac,"Syriac"},
    {QChar::Script_Thaana,"Thaana"},
    {QChar::Script_Devanagari,"Devanagari"},
    {QChar::Script_Bengali,"Bengali"},
    {QChar::Script_Gurmukhi,"Gurmukhi"},
    {QChar::Script_Gujarati,"Gujarati"},
    {QChar::Script_Oriya,"Oriya"},
    {QChar::Script_Tamil,"Tamil"},
    {QChar::Script_Telugu,"Telugu"},
    {QChar::Script_Kannada,"Kannada"},
    {QChar::Script_Malayalam,"Malayalam"},
    {QChar::Script_Sinhala,"Sinhala"},
    {QChar::Script_Thai,"Thai"},
    {QChar::Script_Lao,"Lao"},
    {QChar::Script_Tibetan,"Tibetan"},
    {QChar::Script_Myanmar,"Myanmar"},
    {QChar::Script_Georgian,"Georgian"},
    {QChar::Script_Hangul,"Hangul"},
    {QChar::Script_Ethiopic,"Ethiopic"},
    {QChar::Script_Cherokee,"Cherokee"},
    {QChar::Script_CanadianAboriginal,"CanadianAboriginal"},
    {QChar::Script_Ogham,"Ogham"},
    {QChar::Script_Runic,"Runic"},
    {QChar::Script_Khmer,"Khmer"},
    {QChar::Script_Mongolian,"Mongolian"},
    {QChar::Script_Hiragana,"Hiragana"},
    {QChar::Script_Katakana,"Katakana"},
    {QChar::Script_Bopomofo,"Bopomofo"},
    {QChar::Script_Han,"Han"},
    {QChar::Script_Yi,"Yi"},
    {QChar::Script_OldItalic,"OldItalic"},
    {QChar::Script_Gothic,"Gothic"},
    {QChar::Script_Deseret,"Deseret"},
    {QChar::Script_Tagalog,"Tagalog"},
    {QChar::Script_Hanunoo,"Hanunoo"},
    {QChar::Script_Buhid,"Buhid"},
    {QChar::Script_Tagbanwa,"Tagbanwa"},
    {QChar::Script_Coptic,"Coptic"},
    {QChar::Script_Limbu,"Limbu"},
    {QChar::Script_TaiLe,"TaiLe"},
    {QChar::Script_LinearB,"LinearB"},
    {QChar::Script_Ugaritic,"Ugaritic"},
    {QChar::Script_Shavian,"Shavian"},
    {QChar::Script_Osmanya,"Osmanya"},
    {QChar::Script_Cypriot,"Cypriot"},
    {QChar::Script_Braille,"Braille"},
    {QChar::Script_Buginese,"Buginese"},
    {QChar::Script_NewTaiLue,"NewTaiLue"},
    {QChar::Script_Glagolitic,"Glagolitic"},
    {QChar::Script_Tifinagh,"Tifinagh"},
    {QChar::Script_SylotiNagri,"SylotiNagri"},
    {QChar::Script_OldPersian,"OldPersian"},
    {QChar::Script_Kharoshthi,"Kharoshthi"},
    {QChar::Script_Balinese,"Balinese"},
    {QChar::Script_Cuneiform,"Cuneiform"},
    {QChar::Script_Phoenician,"Phoenician"},
    {QChar::Script_PhagsPa,"PhagsPa"},
    {QChar::Script_Nko,"Nko"},
    {QChar::Script_Sundanese,"Sundanese"},
    {QChar::Script_Lepcha,"Lepcha"},
    {QChar::Script_OlChiki,"OlChiki"},
    {QChar::Script_Vai,"Vai"},
    {QChar::Script_Saurashtra,"Saurashtra"},
    {QChar::Script_KayahLi,"KayahLi"},
    {QChar::Script_Rejang,"Rejang"},
    {QChar::Script_Lycian,"Lycian"},
    {QChar::Script_Carian,"Carian"},
    {QChar::Script_Lydian,"Lydian"},
    {QChar::Script_Cham,"Cham"},
    {QChar::Script_TaiTham,"TaiTham"},
    {QChar::Script_TaiViet,"TaiViet"},
    {QChar::Script_Avestan,"Avestan"},
    {QChar::Script_EgyptianHieroglyphs,"EgyptianHieroglyphs"},
    {QChar::Script_Samaritan,"Samaritan"},
    {QChar::Script_Lisu,"Lisu"},
    {QChar::Script_Bamum,"Bamum"},
    {QChar::Script_Javanese,"Javanese"},
    {QChar::Script_MeeteiMayek,"MeeteiMayek"},
    {QChar::Script_ImperialAramaic,"ImperialAramaic"},
    {QChar::Script_OldSouthArabian,"OldSouthArabian"},
    {QChar::Script_InscriptionalParthian,"InscriptionalParthian"},
    {QChar::Script_InscriptionalPahlavi,"InscriptionalPahlavi"},
    {QChar::Script_OldTurkic,"OldTurkic"},
    {QChar::Script_Kaithi,"Kaithi"},
    {QChar::Script_Batak,"Batak"},
    {QChar::Script_Brahmi,"Brahmi"},
    {QChar::Script_Mandaic,"Mandaic"},
    {QChar::Script_Chakma,"Chakma"},
    {QChar::Script_MeroiticCursive,"MeroiticCursive"},
    {QChar::Script_MeroiticHieroglyphs,"MeroiticHieroglyphs"},
    {QChar::Script_Miao,"Miao"},
    {QChar::Script_Sharada,"Sharada"},
    {QChar::Script_SoraSompeng,"SoraSompeng"},
    {QChar::Script_Takri,"Takri"}
};

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
    UN_Latin.insert(0xFB00,"ff");

    UN_Latin.insert(0xFB01,"fi");

    UN_Latin.insert(0xFB02,"fl");

    UN_Latin.insert(0xFB03,"ffi");

    UN_Latin.insert(0xFB04,"ffl");

    UN_Latin.insert(0xFB05,"st");
    UN_Latin.insert(0xFB06,"st");

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

    UN_Latin.insert(0x1D00, "A");
    UN_Latin.insert(0x1D01, "AE");
    UN_Latin.insert(0x1D03, "B");
    UN_Latin.insert(0x1D04, "C");
    UN_Latin.insert(0x1D05, "D");
    UN_Latin.insert(0x1D06, "D");
    UN_Latin.insert(0x1D07, "E");
    UN_Latin.insert(0x1D09, "!");
    UN_Latin.insert(0x1D0A, "J");
    UN_Latin.insert(0x1D0B, "K");
    UN_Latin.insert(0x1D0C, "L");
    UN_Latin.insert(0x1D0D, "M");
    UN_Latin.insert(0x1D0F, "o");
    UN_Latin.insert(0x1D18, "P");
    UN_Latin.insert(0x1D1B, "T");
    UN_Latin.insert(0x1D1C, "U");
    UN_Latin.insert(0x1D20, "V");
    UN_Latin.insert(0x1D21, "W");
    UN_Latin.insert(0x1D22, "Z");



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

    UN_Latin.insert(0xFFE0, QString::fromUtf8("¢"));
    UN_Latin.insert(0xFFE1, QString::fromUtf8("£"));

    UN_Latin.insert(0xFFE5, QString::fromUtf8("¥"));
    UN_Latin.insert(0xFFE6, QString::fromUtf8("₩"));


    /////////////////////////////////////////////////////////////////
    ///                          Arabic                           ///
    ///        http://www.unicode.org/charts/PDF/U0600.pdf        ///
    ///        http://www.unicode.org/charts/PDF/U0750.pdf        ///
    ///        http://www.unicode.org/charts/PDF/U08A0.pdf        ///
    ///        http://www.unicode.org/charts/PDF/UFB50.pdf        ///
    ///        http://www.unicode.org/charts/PDF/UFE70.pdf        ///
    /////////////////////////////////////////////////////////////////
    UN_Arabic.insert(0xFE81,QString::fromUtf8("آ"));

    UN_Arabic.insert(0xFE8D,QString::fromUtf8("ا"));
    UN_Arabic.insert(0xFE8E,QString::fromUtf8("ا"));

    UN_Arabic.insert(0x672,QString::fromUtf8("أ"));
    UN_Arabic.insert(0x675,QString::fromUtf8("أ"));
    UN_Arabic.insert(0xFB50,QString::fromUtf8("أ"));
    UN_Arabic.insert(0xFB51,QString::fromUtf8("أ"));
    UN_Arabic.insert(0xFE83,QString::fromUtf8("أ"));
    UN_Arabic.insert(0xFE84,QString::fromUtf8("أ"));

    UN_Arabic.insert(0x673,QString::fromUtf8("إ"));
    UN_Arabic.insert(0xFE87,QString::fromUtf8("إ"));
    UN_Arabic.insert(0xFE88,QString::fromUtf8("إ"));

    UN_Arabic.insert(0xFE82,QString::fromUtf8("ا"));

    UN_Arabic.insert(0x66E,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x67B,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x680,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x750,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x752,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x754,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x755,QString::fromUtf8("ب"));
    UN_Arabic.insert(0x756,QString::fromUtf8("ب"));
    UN_Arabic.insert(0xFB52,QString::fromUtf8("ب") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB53,QString::fromUtf8("ب") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB54,QString::fromUtf8("ب"));
    UN_Arabic.insert(0xFB55,QString::fromUtf8("ب"));
    UN_Arabic.insert(0xFE8F,QString::fromUtf8("ب") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFE90,QString::fromUtf8("ب") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFE91,QString::fromUtf8("ب"));
    UN_Arabic.insert(0xFE92,QString::fromUtf8("ب"));

    UN_Arabic.insert(0xFB56,QString::fromUtf8("پ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB57,QString::fromUtf8("پ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB58,QString::fromUtf8("پ"));
    UN_Arabic.insert(0xFB59,QString::fromUtf8("پ"));
    UN_Arabic.insert(0xFB5A,QString::fromUtf8("پ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB5B,QString::fromUtf8("پ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB5C,QString::fromUtf8("پ"));
    UN_Arabic.insert(0xFB5D,QString::fromUtf8("پ"));

    UN_Arabic.insert(0x679,QString::fromUtf8("ت"));
    UN_Arabic.insert(0x67A,QString::fromUtf8("ت"));
    UN_Arabic.insert(0x67C,QString::fromUtf8("ت"));
    UN_Arabic.insert(0x67F,QString::fromUtf8("ت"));
    UN_Arabic.insert(0x753,QString::fromUtf8("ت"));
    UN_Arabic.insert(0xFB5E,QString::fromUtf8("ت") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB5F,QString::fromUtf8("ت") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB60,QString::fromUtf8("ت"));
    UN_Arabic.insert(0xFB61,QString::fromUtf8("ت"));
    UN_Arabic.insert(0xFB66,QString::fromUtf8("ت") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB67,QString::fromUtf8("ت") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB68,QString::fromUtf8("ت"));
    UN_Arabic.insert(0xFB69,QString::fromUtf8("ت"));
    UN_Arabic.insert(0xFE95,QString::fromUtf8("ت") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFE96,QString::fromUtf8("ت") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFE97,QString::fromUtf8("ت"));
    UN_Arabic.insert(0xFE98,QString::fromUtf8("ت"));

    UN_Arabic.insert(0x67D,QString::fromUtf8("ث"));
    UN_Arabic.insert(0x751,QString::fromUtf8("ث"));
    UN_Arabic.insert(0xFE99,QString::fromUtf8("ث") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFE9A,QString::fromUtf8("ث") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFE9B,QString::fromUtf8("ث"));
    UN_Arabic.insert(0xFE9C,QString::fromUtf8("ث"));
    UN_Arabic.insert(0xFB62,QString::fromUtf8("ث") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB63,QString::fromUtf8("ث") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB64,QString::fromUtf8("ث"));
    UN_Arabic.insert(0xFB65,QString::fromUtf8("ث"));


    UN_Arabic.insert(0x683,QString::fromUtf8("ج"));
    UN_Arabic.insert(0x684,QString::fromUtf8("ج"));
    UN_Arabic.insert(0xFB72,QString::fromUtf8("ج") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB73,QString::fromUtf8("ج") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB74,QString::fromUtf8("ج"));
    UN_Arabic.insert(0xFB75,QString::fromUtf8("ج"));
    UN_Arabic.insert(0xFB76,QString::fromUtf8("ج") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB77,QString::fromUtf8("ج") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB78,QString::fromUtf8("ج"));
    UN_Arabic.insert(0xFB79,QString::fromUtf8("ج"));
    UN_Arabic.insert(0xFE9D,QString::fromUtf8("ج") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFE9E,QString::fromUtf8("ج") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFE9F,QString::fromUtf8("ج"));
    UN_Arabic.insert(0xFEA0,QString::fromUtf8("ج"));

    UN_Arabic.insert(0x687,QString::fromUtf8("چ"));
    UN_Arabic.insert(0x6BF,QString::fromUtf8("چ"));
    UN_Arabic.insert(0xFB7A,QString::fromUtf8("چ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB7B,QString::fromUtf8("چ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB7C,QString::fromUtf8("چ"));
    UN_Arabic.insert(0xFB7D,QString::fromUtf8("چ"));
    UN_Arabic.insert(0xFB7E,QString::fromUtf8("چ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB7F,QString::fromUtf8("چ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB80,QString::fromUtf8("چ"));
    UN_Arabic.insert(0xFB81,QString::fromUtf8("چ"));

    UN_Arabic.insert(0x681,QString::fromUtf8("ح"));
    UN_Arabic.insert(0x682,QString::fromUtf8("ح"));
    UN_Arabic.insert(0x685,QString::fromUtf8("ح"));
    UN_Arabic.insert(0x757,QString::fromUtf8("ح"));
    UN_Arabic.insert(0x758,QString::fromUtf8("ح"));
    UN_Arabic.insert(0x76D,QString::fromUtf8("ح"));
    UN_Arabic.insert(0x76E,QString::fromUtf8("ح"));
    UN_Arabic.insert(0xFEA1,QString::fromUtf8("ح") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEA2,QString::fromUtf8("ح") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEA3,QString::fromUtf8("ح"));
    UN_Arabic.insert(0xFEA4,QString::fromUtf8("ح"));

    UN_Arabic.insert(0xFEA5,QString::fromUtf8("خ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEA6,QString::fromUtf8("خ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEA7,QString::fromUtf8("خ"));
    UN_Arabic.insert(0xFEA8,QString::fromUtf8("خ"));

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
    UN_Arabic.insert(0xFB82,QString::fromUtf8("د"));
    UN_Arabic.insert(0xFB83,QString::fromUtf8("د"));
    UN_Arabic.insert(0xFB84,QString::fromUtf8("د"));
    UN_Arabic.insert(0xFB85,QString::fromUtf8("د"));
    UN_Arabic.insert(0xFB86,QString::fromUtf8("د"));
    UN_Arabic.insert(0xFB87,QString::fromUtf8("د"));
    UN_Arabic.insert(0xFB88,QString::fromUtf8("د"));
    UN_Arabic.insert(0xFB89,QString::fromUtf8("د"));
    UN_Arabic.insert(0xFEA9,QString::fromUtf8("د"));
    UN_Arabic.insert(0xFEAA,QString::fromUtf8("د"));

    UN_Arabic.insert(0x68C,QString::fromUtf8("ذ"));
    UN_Arabic.insert(0xFC5B,QString::fromUtf8("ذ"));
    UN_Arabic.insert(0xFEAB,QString::fromUtf8("ذ"));
    UN_Arabic.insert(0xFEAC,QString::fromUtf8("ذ"));

    UN_Arabic.insert(0x691,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x692,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x693,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x694,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x695,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x696,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x6EF,QString::fromUtf8("ر"));
    UN_Arabic.insert(0x75B,QString::fromUtf8("ر"));
    UN_Arabic.insert(0xFB8C,QString::fromUtf8("ر"));
    UN_Arabic.insert(0xFB8D,QString::fromUtf8("ر"));
    UN_Arabic.insert(0xFC5C,QString::fromUtf8("ر"));
    UN_Arabic.insert(0xFEAD,QString::fromUtf8("ر"));
    UN_Arabic.insert(0xFEAE,QString::fromUtf8("ر"));

    UN_Arabic.insert(0x697,QString::fromUtf8("ز"));
    UN_Arabic.insert(0x76B,QString::fromUtf8("ز"));
    UN_Arabic.insert(0x76C,QString::fromUtf8("ز"));
    UN_Arabic.insert(0xFEAF,QString::fromUtf8("ز"));
    UN_Arabic.insert(0xFEB0,QString::fromUtf8("ز"));

    UN_Arabic.insert(0x699,QString::fromUtf8("ژ"));
    UN_Arabic.insert(0xFB8A,QString::fromUtf8("ژ"));
    UN_Arabic.insert(0xFB8B,QString::fromUtf8("ژ"));

    UN_Arabic.insert(0x69A,QString::fromUtf8("س"));
    UN_Arabic.insert(0x69B,QString::fromUtf8("س"));
    UN_Arabic.insert(0x75C,QString::fromUtf8("س"));
    UN_Arabic.insert(0xFEB1,QString::fromUtf8("س") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEB2,QString::fromUtf8("س") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEB3,QString::fromUtf8("س"));
    UN_Arabic.insert(0xFEB4,QString::fromUtf8("س"));

    UN_Arabic.insert(0x69C,QString::fromUtf8("ش"));
    UN_Arabic.insert(0x6FA,QString::fromUtf8("ش"));
    UN_Arabic.insert(0xFEB5,QString::fromUtf8("ش") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEB6,QString::fromUtf8("ش") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEB7,QString::fromUtf8("ش"));
    UN_Arabic.insert(0xFEB8,QString::fromUtf8("ش"));

    UN_Arabic.insert(0x69D,QString::fromUtf8("ص"));
    UN_Arabic.insert(0x69E,QString::fromUtf8("ص"));
    UN_Arabic.insert(0xFEB9,QString::fromUtf8("ص") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEBA,QString::fromUtf8("ص") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEBB,QString::fromUtf8("ص"));
    UN_Arabic.insert(0xFEBC,QString::fromUtf8("ص"));

    UN_Arabic.insert(0x6FB,QString::fromUtf8("ض"));
    UN_Arabic.insert(0xFEBD,QString::fromUtf8("ض") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEBE,QString::fromUtf8("ض") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEBF,QString::fromUtf8("ض"));
    UN_Arabic.insert(0xFEC0,QString::fromUtf8("ض"));

    UN_Arabic.insert(0x69F,QString::fromUtf8("ط"));
    UN_Arabic.insert(0xFEC1,QString::fromUtf8("ط") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEC2,QString::fromUtf8("ط") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEC3,QString::fromUtf8("ط"));
    UN_Arabic.insert(0xFEC4,QString::fromUtf8("ط"));

    UN_Arabic.insert(0xFEC5,QString::fromUtf8("ظ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEC6,QString::fromUtf8("ظ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEC7,QString::fromUtf8("ظ"));
    UN_Arabic.insert(0xFEC8,QString::fromUtf8("ظ"));

    UN_Arabic.insert(0x6A0,QString::fromUtf8("ع"));
    UN_Arabic.insert(0x75D,QString::fromUtf8("ع"));
    UN_Arabic.insert(0x75E,QString::fromUtf8("ع"));
    UN_Arabic.insert(0x75F,QString::fromUtf8("ع"));
    UN_Arabic.insert(0xFEC9,QString::fromUtf8("ع") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFECA,QString::fromUtf8("ع") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFECB,QString::fromUtf8("ع"));
    UN_Arabic.insert(0xFECC,QString::fromUtf8("ع"));

    UN_Arabic.insert(0x6FC,QString::fromUtf8("غ"));
    UN_Arabic.insert(0xFECD,QString::fromUtf8("غ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFECE,QString::fromUtf8("غ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFECF,QString::fromUtf8("غ"));
    UN_Arabic.insert(0xFED0,QString::fromUtf8("غ"));

    UN_Arabic.insert(0x6A1,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x6A2,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x6A3,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x6A4,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x6A5,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x6A6,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x760,QString::fromUtf8("ف"));
    UN_Arabic.insert(0x761,QString::fromUtf8("ف"));
    UN_Arabic.insert(0xFB6A,QString::fromUtf8("ف") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB6B,QString::fromUtf8("ف") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB6C,QString::fromUtf8("ف"));
    UN_Arabic.insert(0xFB6D,QString::fromUtf8("ف"));
    UN_Arabic.insert(0xFB6E,QString::fromUtf8("ف") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB6F,QString::fromUtf8("ف") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB70,QString::fromUtf8("ف"));
    UN_Arabic.insert(0xFB71,QString::fromUtf8("ف"));
    UN_Arabic.insert(0xFED1,QString::fromUtf8("ف") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFED2,QString::fromUtf8("ف") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFED3,QString::fromUtf8("ف"));
    UN_Arabic.insert(0xFED4,QString::fromUtf8("ف"));

    UN_Arabic.insert(0x66F,QString::fromUtf8("ق"));
    UN_Arabic.insert(0x6A7,QString::fromUtf8("ق"));
    UN_Arabic.insert(0x6A8,QString::fromUtf8("ق"));
    UN_Arabic.insert(0xFED5,QString::fromUtf8("ق") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFED6,QString::fromUtf8("ق") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFED7,QString::fromUtf8("ق"));
    UN_Arabic.insert(0xFED8,QString::fromUtf8("ق"));

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
    UN_Arabic.insert(0xFB8E,QString::fromUtf8("ک") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB8F,QString::fromUtf8("ک") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB90,QString::fromUtf8("ک"));
    UN_Arabic.insert(0xFB91,QString::fromUtf8("ک"));
    UN_Arabic.insert(0xFBD3,QString::fromUtf8("ک") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBD4,QString::fromUtf8("ک") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBD5,QString::fromUtf8("ک"));
    UN_Arabic.insert(0xFBD6,QString::fromUtf8("ک"));
    UN_Arabic.insert(0xFED9,QString::fromUtf8("ک") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEDA,QString::fromUtf8("ک") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEDB,QString::fromUtf8("ک"));
    UN_Arabic.insert(0xFEDC,QString::fromUtf8("ک"));

    UN_Arabic.insert(0x6B0,QString::fromUtf8("گ"));
    UN_Arabic.insert(0x6B1,QString::fromUtf8("گ"));
    UN_Arabic.insert(0x6B2,QString::fromUtf8("گ"));
    UN_Arabic.insert(0x6B3,QString::fromUtf8("گ"));
    UN_Arabic.insert(0x6B4,QString::fromUtf8("گ"));
    UN_Arabic.insert(0xFB92,QString::fromUtf8("گ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB93,QString::fromUtf8("گ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB94,QString::fromUtf8("گ"));
    UN_Arabic.insert(0xFB95,QString::fromUtf8("گ"));
    UN_Arabic.insert(0xFB96,QString::fromUtf8("گ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB97,QString::fromUtf8("گ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB98,QString::fromUtf8("گ"));
    UN_Arabic.insert(0xFB99,QString::fromUtf8("گ"));
    UN_Arabic.insert(0xFB9A,QString::fromUtf8("گ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB9B,QString::fromUtf8("گ") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB9C,QString::fromUtf8("گ"));
    UN_Arabic.insert(0xFB9D,QString::fromUtf8("گ"));

    UN_Arabic.insert(0x6B5,QString::fromUtf8("ل"));
    UN_Arabic.insert(0x6B6,QString::fromUtf8("ل"));
    UN_Arabic.insert(0x6B7,QString::fromUtf8("ل"));
    UN_Arabic.insert(0x6B8,QString::fromUtf8("ل"));
    UN_Arabic.insert(0x76A,QString::fromUtf8("ل"));
    UN_Arabic.insert(0xFEDD,QString::fromUtf8("ل") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEDE,QString::fromUtf8("ل") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEDF,QString::fromUtf8("ل"));
    UN_Arabic.insert(0xFEE0,QString::fromUtf8("ل"));

    UN_Arabic.insert(0x6FE,QString::fromUtf8("م"));
    UN_Arabic.insert(0x765,QString::fromUtf8("م"));
    UN_Arabic.insert(0x766,QString::fromUtf8("م"));
    UN_Arabic.insert(0xFEE1,QString::fromUtf8("م") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEE2,QString::fromUtf8("م") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEE3,QString::fromUtf8("م"));
    UN_Arabic.insert(0xFEE4,QString::fromUtf8("م"));

    UN_Arabic.insert(0x6B9,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x6BA,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x6BB,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x6BC,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x6BD,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x767,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x768,QString::fromUtf8("ن"));
    UN_Arabic.insert(0x769,QString::fromUtf8("ن"));
    UN_Arabic.insert(0xFB9E,QString::fromUtf8("ن") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFB9F,QString::fromUtf8("ن") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBA0,QString::fromUtf8("ن") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBA1,QString::fromUtf8("ن") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBA2,QString::fromUtf8("ن"));
    UN_Arabic.insert(0xFBA3,QString::fromUtf8("ن"));
    UN_Arabic.insert(0xFEE5,QString::fromUtf8("ن") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEE6,QString::fromUtf8("ن") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEE7,QString::fromUtf8("ن"));
    UN_Arabic.insert(0xFEE8,QString::fromUtf8("ن"));

    UN_Arabic.insert(0x676,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0x677,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0xFBD7,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0xFBD8,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0xFBD9,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0xFBDA,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0xFBDB,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0xFBDC,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0xFBDD,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0xFBDE,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0xFBDF,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0xFE85,QString::fromUtf8("ؤ"));
    UN_Arabic.insert(0xFE86,QString::fromUtf8("ؤ"));

    UN_Arabic.insert(0x6C4,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6C5,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6C6,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6C7,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6C8,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6C9,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6CA,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6CB,QString::fromUtf8("و"));
    UN_Arabic.insert(0x6CF,QString::fromUtf8("و"));
    UN_Arabic.insert(0xFBE0,QString::fromUtf8("و"));
    UN_Arabic.insert(0xFBE1,QString::fromUtf8("و"));
    UN_Arabic.insert(0xFBE2,QString::fromUtf8("و"));
    UN_Arabic.insert(0xFBE3,QString::fromUtf8("و"));
    UN_Arabic.insert(0xFEED,QString::fromUtf8("و"));
    UN_Arabic.insert(0xFEEE,QString::fromUtf8("و"));

    UN_Arabic.insert(0x6BE,QString::fromUtf8("ه"));
    UN_Arabic.insert(0x6FF,QString::fromUtf8("ه"));
    UN_Arabic.insert(0x6C0,QString::fromUtf8("ه"));
    UN_Arabic.insert(0x6C1,QString::fromUtf8("ه"));
    UN_Arabic.insert(0x6C2,QString::fromUtf8("ه"));
    UN_Arabic.insert(0x6C3,QString::fromUtf8("ه"));
    UN_Arabic.insert(0x6D5,QString::fromUtf8("ه"));
    UN_Arabic.insert(0xFBA4,QString::fromUtf8("ه") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBA5,QString::fromUtf8("ه") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBA6,QString::fromUtf8("ه") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBA7,QString::fromUtf8("ه") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBA8,QString::fromUtf8("ه"));
    UN_Arabic.insert(0xFBA9,QString::fromUtf8("ه"));
    UN_Arabic.insert(0xFBAA,QString::fromUtf8("ه") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBAB,QString::fromUtf8("ه"));
    UN_Arabic.insert(0xFBAC,QString::fromUtf8("ه"));
    UN_Arabic.insert(0xFBAD,QString::fromUtf8("ه"));
    UN_Arabic.insert(0xFE93,QString::fromUtf8("ه"));
    UN_Arabic.insert(0xFE94,QString::fromUtf8("ه"));
    UN_Arabic.insert(0xFEE9,QString::fromUtf8("ه") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEEA,QString::fromUtf8("ه") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEEB,QString::fromUtf8("ه"));
    UN_Arabic.insert(0xFEEC,QString::fromUtf8("ه"));

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
    UN_Arabic.insert(0xFBAE,QString::fromUtf8("ی") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBAF,QString::fromUtf8("ی") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBB0,QString::fromUtf8("ی") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBB1,QString::fromUtf8("ی") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBE4,QString::fromUtf8("ی") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBE5,QString::fromUtf8("ی") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBE6,QString::fromUtf8("ی"));
    UN_Arabic.insert(0xFBE7,QString::fromUtf8("ی"));
    UN_Arabic.insert(0xFBFC,QString::fromUtf8("ی") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBFD,QString::fromUtf8("ی") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFBFE,QString::fromUtf8("ی"));
    UN_Arabic.insert(0xFBFF,QString::fromUtf8("ی"));
    UN_Arabic.insert(0xFEEF,QString::fromUtf8("ی") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEF0,QString::fromUtf8("ی") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEF1,QString::fromUtf8("ی") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEF2,QString::fromUtf8("ی") + ARABIC_ZWNJ);
    UN_Arabic.insert(0xFEF3,QString::fromUtf8("ی"));
    UN_Arabic.insert(0xFEF4,QString::fromUtf8("ی"));

    UN_Arabic.insert(0xFBE8,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBE9,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBEA,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBEB,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBEC,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBED,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBEE,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBEF,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBF0,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBF1,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBF2,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBF3,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBF4,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBF5,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBF6,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBF7,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBF8,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBF9,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBFA,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFBFB,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFC5D,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFE89,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFE8A,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFE8B,QString::fromUtf8("ئ"));
    UN_Arabic.insert(0xFE8C,QString::fromUtf8("ئ"));

    UN_Arabic.insert(0xFEF5,QString::fromUtf8("لا"));
    UN_Arabic.insert(0xFEF6,QString::fromUtf8("لا"));
    UN_Arabic.insert(0xFEFB,QString::fromUtf8("لا"));
    UN_Arabic.insert(0xFEFC,QString::fromUtf8("لا"));

    UN_Arabic.insert(0xFEF7,QString::fromUtf8("لأ"));
    UN_Arabic.insert(0xFEF8,QString::fromUtf8("لأ"));

    UN_Arabic.insert(0xFEF9,QString::fromUtf8("لإ"));
    UN_Arabic.insert(0xFEFA,QString::fromUtf8("لإ"));


    UN_Arabic.insert(0xFC00,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC01,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC02,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC03,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC04,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC05,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC06,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC07,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC08,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC09,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC0A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC0B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC0C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC0D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC0E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC0F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC10,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC11,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC12,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC13,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC14,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC15,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC16,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC17,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC18,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC19,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC1A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC1B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC1C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC1D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC1E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC1F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC20,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC21,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC22,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC23,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC24,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC25,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC26,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC27,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC28,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC29,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC2A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC2B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC2C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC2D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC2E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC2F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC30,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC31,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC32,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC33,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC34,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC35,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC36,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC37,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC38,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC39,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC3A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC3B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC3C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC3D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC3E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC3F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC40,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC41,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC42,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC43,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC44,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC45,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC46,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC47,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC48,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC49,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC4A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC4B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC4C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC4D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC4E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC4F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC50,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC51,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC52,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC53,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC54,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC55,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC56,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC57,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC58,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC59,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC5A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));

    UN_Arabic.insert(0xFC64,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC65,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC66,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC67,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC68,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC69,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC6A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC6B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC6C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC6D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC6E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC6F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC70,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC71,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC72,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC73,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC74,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC75,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC76,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC77,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC78,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC79,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC7A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC7B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC7C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC7D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC7E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC7F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC80,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC81,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC82,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC83,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC84,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC85,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC86,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC87,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC88,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC89,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC8A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC8B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC8C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC8D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC8E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC8F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC90,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC91,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC92,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC93,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC94,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC95,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC96,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC97,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC98,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC99,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC9A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC9B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC9C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC9D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC9E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFC9F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCA0,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCA1,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCA2,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCA3,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCA4,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCA5,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCA6,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCA7,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCA8,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCA9,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCAA,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCAB,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCAC,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCAD,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCAE,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCAF,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCB0,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCB1,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCB2,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCB3,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCB4,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCB5,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCB6,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCB7,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCB8,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCB9,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCBA,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCBB,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCBC,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCBD,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCBE,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCBF,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCC0,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCC1,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCC2,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCC3,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCC4,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCC5,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCC6,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCC7,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCC8,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCC9,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCCA,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCCB,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCCC,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCCD,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCCE,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCCF,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCD0,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCD1,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCD2,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCD3,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCD4,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCD5,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCD6,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCD7,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCD8,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCD9,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCDA,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCDB,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCDC,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCDD,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCDE,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCDF,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCE0,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCE1,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCE2,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCE3,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCE4,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCE5,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCE6,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCE7,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCE8,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCE9,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCEA,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCEB,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCEC,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCED,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCEE,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCEF,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCF0,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCF1,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCF2,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCF3,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCF4,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCF5,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCF6,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCF7,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCF8,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCF9,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCFA,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCFB,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCFC,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCFD,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCFE,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFCFF,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD00,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD01,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD02,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD03,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD04,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD05,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD06,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD07,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD08,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD09,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD0A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD0B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD0C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD0D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD0E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD0F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD10,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD11,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD12,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD13,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD14,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD15,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD16,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD17,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD18,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD19,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD1A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD1B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD1C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD1D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD1E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD1F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD20,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD21,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD22,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD23,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD24,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD25,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD26,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD27,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD28,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD29,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD2A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD2B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD2C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD2D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD2E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD2F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD30,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD31,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD32,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD33,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD34,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD35,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD36,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD37,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD38,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD39,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD3A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD3B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD3C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD3D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));

    UN_Arabic.insert(0xFD40,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD41,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD42,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD43,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD44,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD45,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD46,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD47,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD48,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD49,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD4A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD4B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD4C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD4D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD4E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD4F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD50,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD51,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD52,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD53,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD54,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD55,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD56,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD57,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD58,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD59,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD5A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD5B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD5C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD5D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD5E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD5F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD60,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD61,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD62,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD63,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD64,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD65,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD66,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD67,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD68,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD69,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD6A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD6B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD6C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD6D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD6E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD6F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD70,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD71,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD72,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD73,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD74,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD75,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD76,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD77,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD78,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD79,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD7A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD7B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD7C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD7D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD7E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD7F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD80,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD81,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD82,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD83,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD84,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD85,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD86,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD87,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD88,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD89,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD8A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD8B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD8C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD8D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD8E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD8F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD90,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD91,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD92,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD93,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD94,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD95,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD96,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD97,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD98,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD99,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD9A,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD9B,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD9C,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD9D,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD9E,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFD9F,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDA0,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDA1,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDA2,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDA3,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDA4,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDA5,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDA6,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDA7,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDA8,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDA9,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDAA,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDAB,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDAC,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDAD,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDAE,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDAF,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDB0,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDB1,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDB2,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDB3,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDB4,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDB5,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDB6,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDB7,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDB8,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDB9,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDBA,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDBB,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDBC,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDBD,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDBE,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDBF,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDC0,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDC1,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDC2,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDC3,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDC4,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDC5,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDC6,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDC7,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDC8,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDC9,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDCA,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDCB,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDCC,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDCD,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDCE,QString::fromUtf8("MIXED_ARABIC_LETTERS"));
    UN_Arabic.insert(0xFDCF,QString::fromUtf8("MIXED_ARABIC_LETTERS"));

    UN_Arabic.insert(0xFDF0,QString::fromUtf8("صلی"));
    UN_Arabic.insert(0xFDF1,QString::fromUtf8("قلی"));
    UN_Arabic.insert(0xFDF2,QString::fromUtf8("الله"));
    UN_Arabic.insert(0xFDF3,QString::fromUtf8("اکبر"));
    UN_Arabic.insert(0xFDF4,QString::fromUtf8("محمد"));
    UN_Arabic.insert(0xFDF5,QString::fromUtf8("صلی"));
    UN_Arabic.insert(0xFDF6,QString::fromUtf8("رسول"));
    UN_Arabic.insert(0xFDF7,QString::fromUtf8("علیه"));
    UN_Arabic.insert(0xFDF8,QString::fromUtf8("وسلم"));
    UN_Arabic.insert(0xFDF9,QString::fromUtf8("صلی"));
    UN_Arabic.insert(0xFDFA,QString::fromUtf8("صلی الله علیه و سلم"));
    UN_Arabic.insert(0xFDFB,QString::fromUtf8("جل جلاله"));

    UN_Arabic.insert(0xFDFD,QString::fromUtf8("سم"));


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
        (_ucs4>=0xFF65 && _ucs4<=0xFFDC) ||
        (_ucs4>=0x31A0 && _ucs4<=0x31BF) ||
        (_ucs4>=0x31F0 && _ucs4<=0x31FF))
        return QString(QChar(_ucs4));

    ///@ref CJK
    if ((_ucs4>=0x3400 && _ucs4 <=0x9FFF) ||
        (_ucs4>=0xF900 && _ucs4 <=0xFAFF) ||
        (_ucs4>=0xFE30 && _ucs4 <=0xFE4F))
        return QString(QChar(_ucs4));

    ///@ref Yi Syllables
    if (_ucs4>=0xA000 && _ucs4 <=0xA48F)
        return QString(QChar(_ucs4));

    ///@ref Syloti Nagri
    if (_ucs4>=0xA800 && _ucs4 <=0xA82F)
        return QString(QChar(_ucs4));

    ///@ref Syloti Nagri
    if (_ucs4>=0xA840 && _ucs4 <=0xA87F)
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

static QMap<QChar::Script, ScriptBasedNormalizer> QCharScriptToNormalizerMap {
    {QChar::Script_Unknown, NULL},
    {QChar::Script_Inherited, NULL},
    {QChar::Script_Common, latin2Ascii},
    {QChar::Script_Latin, latin2Ascii},
    {QChar::Script_Greek, greek2Ascii},
    {QChar::Script_Cyrillic, cyrillic2Ascii},
    {QChar::Script_Armenian, armenina2Ascii},
    {QChar::Script_Hebrew, acceptAll},
    {QChar::Script_Arabic, arabicNormalizer},
    {QChar::Script_Syriac, acceptAll},
    {QChar::Script_Thaana, acceptAll},
    {QChar::Script_Devanagari, acceptAll},
    {QChar::Script_Bengali, acceptAll},
    {QChar::Script_Gurmukhi, acceptAll},
    {QChar::Script_Gujarati, acceptAll},
    {QChar::Script_Oriya, acceptAll},
    {QChar::Script_Tamil, acceptAll},
    {QChar::Script_Telugu, acceptAll},
    {QChar::Script_Kannada, acceptAll},
    {QChar::Script_Malayalam, acceptAll},
    {QChar::Script_Sinhala, acceptAll},
    {QChar::Script_Thai, acceptAll},
    {QChar::Script_Lao, acceptAll},
    {QChar::Script_Tibetan, acceptAll},
    {QChar::Script_Myanmar, acceptAll},
    {QChar::Script_Georgian, acceptAll},
    {QChar::Script_Hangul, acceptAll},
    {QChar::Script_Ethiopic, NULL},
    {QChar::Script_Cherokee, NULL},
    {QChar::Script_CanadianAboriginal, NULL},
    {QChar::Script_Ogham, acceptAll},
    {QChar::Script_Runic, acceptAll},
    {QChar::Script_Khmer, acceptAll},
    {QChar::Script_Mongolian, NULL},
    {QChar::Script_Hiragana, NULL},
    {QChar::Script_Katakana, NULL},
    {QChar::Script_Bopomofo, NULL},
    {QChar::Script_Han, NULL},
    {QChar::Script_Yi, NULL},
    {QChar::Script_OldItalic, NULL},
    {QChar::Script_Gothic, NULL},
    {QChar::Script_Deseret, NULL},
    {QChar::Script_Tagalog, NULL},
    {QChar::Script_Hanunoo, NULL},
    {QChar::Script_Buhid, NULL},
    {QChar::Script_Tagbanwa, NULL},
    {QChar::Script_Coptic, NULL},
    {QChar::Script_Limbu, NULL},
    {QChar::Script_TaiLe, NULL},
    {QChar::Script_LinearB, NULL},
    {QChar::Script_Ugaritic, NULL},
    {QChar::Script_Shavian, NULL},
    {QChar::Script_Osmanya, NULL},
    {QChar::Script_Cypriot, NULL},
    {QChar::Script_Braille, acceptAll},
    {QChar::Script_Buginese, NULL},
    {QChar::Script_NewTaiLue, NULL},
    {QChar::Script_Glagolitic, NULL},
    {QChar::Script_Tifinagh, NULL},
    {QChar::Script_SylotiNagri, NULL},
    {QChar::Script_OldPersian, NULL},
    {QChar::Script_Kharoshthi, NULL},
    {QChar::Script_Balinese, NULL},
    {QChar::Script_Cuneiform, NULL},
    {QChar::Script_Phoenician, NULL},
    {QChar::Script_PhagsPa, NULL},
    {QChar::Script_Nko, acceptAll},
    {QChar::Script_Sundanese, NULL},
    {QChar::Script_Lepcha, NULL},
    {QChar::Script_OlChiki, NULL},
    {QChar::Script_Vai, NULL},
    {QChar::Script_Saurashtra, NULL},
    {QChar::Script_KayahLi, NULL},
    {QChar::Script_Rejang, NULL},
    {QChar::Script_Lycian, NULL},
    {QChar::Script_Carian, NULL},
    {QChar::Script_Lydian, NULL},
    {QChar::Script_Cham, NULL},
    {QChar::Script_TaiTham, NULL},
    {QChar::Script_TaiViet, NULL},
    {QChar::Script_Avestan, NULL},
    {QChar::Script_EgyptianHieroglyphs, NULL},
    {QChar::Script_Samaritan, NULL},
    {QChar::Script_Lisu, NULL},
    {QChar::Script_Bamum, NULL},
    {QChar::Script_Javanese, NULL},
    {QChar::Script_MeeteiMayek, NULL},
    {QChar::Script_ImperialAramaic, NULL},
    {QChar::Script_OldSouthArabian, NULL},
    {QChar::Script_InscriptionalParthian, NULL},
    {QChar::Script_InscriptionalPahlavi, NULL},
    {QChar::Script_OldTurkic, NULL},
    {QChar::Script_Kaithi, NULL},
    {QChar::Script_Batak, NULL},
    {QChar::Script_Brahmi, NULL},
    {QChar::Script_Mandaic, NULL},
    {QChar::Script_Chakma, NULL},
    {QChar::Script_MeroiticCursive, NULL},
    {QChar::Script_MeroiticHieroglyphs, NULL},
    {QChar::Script_Miao, NULL},
    {QChar::Script_Sharada, NULL},
    {QChar::Script_SoraSompeng, NULL},
    {QChar::Script_Takri, NULL}
};

/*
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
*/

}
}
}
}

#endif // TARGOMAN_NLPLIBS_TARGOMANTP_PRIVATE_UNICODE_HPP
