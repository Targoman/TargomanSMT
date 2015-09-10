/*************************************************************************
 * ISO639 - A simple library to validate and convert ISO639
 * Copyright (C) 2014  S.Mohammad M. Ziabary <mehran.m@aut.ac.ir>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *************************************************************************/
/**
 @author S.Mohammad M. Ziabary <mehran.m@aut.ac.ir>
 */

#ifndef ISO639_H
#define ISO639_H

#include <malloc.h>

typedef struct stuISO639Info{
    const char* ISO639_1;  /* 2 character code (ISO 639)  ascii */
    const char* ISO639_2T; /* 3 character code (ISO 639-2/T) ascii */
    const char* ISO639_2B; /* 3 character code (ISO 639-2/B) ascii */
    const char* FullName;  /* full language name in UTF */
} stuISO639Info;

static const stuISO639Info ISO639Table[] = {
    { "ab", "abk", "abk", "Abkhazian"},
    { "", "ace", "ace", "Achinese"},
    { "", "ach", "ach", "Acoli"},
    { "", "ada", "ada", "Adangme"},
    { "aa", "aar", "aar", "Afar"},
    { "", "afh", "afh", "Afrihili"},
    { "af", "afr", "afr", "Afrikaans"},
    { "", "afa", "afa", "Afro-Asiatic (Other)"},
    { "", "aka", "aka", "Akan"},
    { "", "akk", "akk", "Akkadian"},
    { "sq", "sqi", "alb", "Albanian"},
    { "", "ale", "ale", "Aleut"},
    { "", "alg", "alg", "Algonquian languages"},
    { "", "ajm", "ajm", "Aljamia"},
    { "", "tut", "tut", "Altaic (Other)"},
    { "am", "amh", "amh", "Amharic"},
    { "", "apa", "apa", "Apache languages"},
    { "ar", "ara", "ara", "Arabic"},
    { "", "arc", "arc", "Aramaic"},
    { "", "arp", "arp", "Arapaho"},
    { "", "arn", "arn", "Araucanian"},
    { "", "arw", "arw", "Arawak"},
    { "hy", "hye", "arm", "Armenian"},
    { "", "art", "art", "Artificial (Other)"},
    { "as", "asm", "asm", "Assamese"},
    { "", "ath", "ath", "Athapascan languages"},
    { "", "aus", "aus", "Australian languages"},
    { "", "map", "map", "Austronesian (Other)"},
    { "", "ava", "ava", "Avaric"},
    { "", "ave", "ave", "Avestan"},
    { "", "awa", "awa", "Awadhi"},
    { "ay", "aym", "aym", "Aymara"},
    { "az", "aze", "aze", "Azerbaijani"},
    { "", "nah", "nah", "Aztec"},
    { "", "ban", "ban", "Balinese"},
    { "", "bat", "bat", "Baltic (Other)"},
    { "", "bal", "bal", "Baluchi"},
    { "", "bam", "bam", "Bambara"},
    { "", "bai", "bai", "Bamileke languages"},
    { "", "bad", "bad", "Banda"},
    { "", "bnt", "bnt", "Bantu (Other)"},
    { "", "bas", "bas", "Basa"},
    { "ba", "bak", "bak", "Bashkir"},
    { "eu", "eus", "baq", "Basque"},
    { "", "btk", "btk", "Batak (Indonesia)"},
    { "", "bej", "bej", "Beja"},
    { "", "bem", "bem", "Bemba"},
    { "bn", "ben", "ben", "Bengali"},
    { "", "ber", "ber", "Berber (Other)"},
    { "", "bho", "bho", "Bhojpuri"},
    { "bh", "bih", "bih", "Bihari"},
    { "", "bik", "bik", "Bikol"},
    { "", "bin", "bin", "Bini"},
    { "bi", "bis", "bis", "Bislama"},
    { "", "bra", "bra", "Braj"},
    { "br", "bre", "bre", "Breton"},
    { "", "bug", "bug", "Buginese"},
    { "bg", "bul", "bul", "Bulgarian"},
    { "", "bua", "bua", "Buriat"},
    { "my", "mya", "bur", "Burmese"},
    { "be", "bel", "bel", "Byelorussian"},
    { "", "cad", "cad", "Caddo"},
    { "", "car", "car", "Carib"},
    { "ca", "cat", "cat", "Catalan"},
    { "", "cau", "cau", "Caucasian (Other)"},
    { "", "ceb", "ceb", "Cebuano"},
    { "", "cel", "cel", "Celtic (Other)"},
    { "", "cai", "cai", "Central American Indian (Other)"},
    { "", "chg", "chg", "Chagatai"},
    { "", "cmc", "cmc", "Chamic languages"},
    { "", "cha", "cha", "Chamorro"},
    { "", "che", "che", "Chechen"},
    { "", "chr", "chr", "Cherokee"},
    { "", "chy", "chy", "Cheyenne"},
    { "", "chb", "chb", "Chibcha"},
    { "zh", "zho", "chi", "Chinese"},
    { "", "chn", "chn", "Chinook jargon"},
    { "", "chp", "chp", "Chipewyan"},
    { "", "cho", "cho", "Choctaw"},
    { "", "chu", "chu", "Church Slavic"},
    { "", "chk", "chk", "Chuukese"},
    { "", "chv", "chv", "Chuvash"},
    { "", "cop", "cop", "Coptic"},
    { "", "cor", "cor", "Cornish"},
    { "co", "cos", "cos", "Corsican"},
    { "", "cre", "cre", "Cree"},
    { "", "mus", "mus", "Creek"},
    { "", "crp", "crp", "Creoles and pidgins (Other)"},
    { "", "cpe", "cpe", "Creoles and pidgins, English-based (Other)"},
    { "", "cpf", "cpf", "Creoles and pidgins, French-based (Other)"},
    { "", "cpp", "cpp", "Creoles and pidgins, Portuguese-based (Other)"},
    { "hr", "hrv", "scr", "Croatian"},
    { "", "cus", "cus", "Cushitic (Other)"},
    { "cs", "ces", "cze", "Czech"},
    { "", "dak", "dak", "Dakota"},
    { "da", "dan", "dan", "Danish"},
    { "", "day", "day", "Dayak"},
    { "", "del", "del", "Delaware"},
    { "", "din", "din", "Dinka"},
    { "", "div", "div", "Divehi"},
    { "", "doi", "doi", "Dogri"},
    { "", "dgr", "dgr", "Dogrib"},
    { "", "dra", "dra", "Dravidian (Other)"},
    { "", "dua", "dua", "Duala"},
    { "", "dum", "dum", "Dutch, Middle (ca. 1050-1350)"},
    { "nl", "nld", "dut", "Dutch"},
    { "", "dyu", "dyu", "Dyula"},
    { "dz", "dzo", "dzo", "Dzongkha"},
    { "", "efi", "efi", "Efik"},
    { "", "egy", "egy", "Egyptian (Ancient)"},
    { "", "eka", "eka", "Ekajuk"},
    { "", "elx", "elx", "Elamite"},
    { "", "enm", "enm", "English, Middle (1100-1500)"},
    { "", "ang", "ang", "English, Old (ca. 450-1100)"},
    { "en", "eng", "eng", "English"},
    { "eo", "epo", "epo", "Esperanto"},
    { "et", "est", "est", "Estonian"},
    { "", "eth", "eth", "Ethiopic"},
    { "", "ewe", "ewe", "Ewe"},
    { "", "ewo", "ewo", "Ewondo"},
    { "fo", "fao", "fao", "Faroese"},
    { "", "fan", "fan", "Fang"},
    { "", "fat", "fat", "Fanti"},
    { "fj", "fij", "fij", "Fijian"},
    { "fi", "fin", "fin", "Finnish"},
    { "", "fiu", "fiu", "Finno-Ugrian (Other)"},
    { "", "fon", "fon", "Fon"},
    { "", "frm", "frm", "French, Middle (ca. 1400-1600)"},
    { "", "fro", "fro", "French, Old (ca. 842-1400)"},
    { "fr", "fra", "fre", "French"},
    { "fy", "fry", "fry", "Frisian"},
    { "", "fur", "fur", "Friulian"},
    { "", "ful", "ful", "Fulah"},
    { "", "gaa", "gaa", "Ga"},
    { "gd", "gdh", "gae", "Gaelic (Scots)"},
    { "gl", "glg", "glg", "Gallegan"},
    { "", "lug", "lug", "Ganda"},
    { "", "gay", "gay", "Gayo"},
    { "", "gba", "gba", "Gbaya"},
    { "", "gez", "gez", "Geez"},
    { "ka", "kat", "geo", "Georgian"},
    { "", "gmh", "gmh", "German, Middle High (ca. 1050-1500)"},
    { "", "goh", "goh", "German, Old High (ca. 750-1050)"},
    { "de", "deu", "ger", "German"},
    { "", "gem", "gem", "Germanic (Other)"},
    { "", "gil", "gil", "Gilbertese"},
    { "", "gon", "gon", "Gondi"},
    { "", "gor", "gor", "Gorontalo"},
    { "", "got", "got", "Gothic"},
    { "", "grb", "grb", "Grebo"},
    { "", "grc", "grc", "Greek, Ancient (to 1453)"},
    { "el", "ell", "gre", "Greek, Modern (1453-)"},
    { "gn", "grn", "grn", "Guarani"},
    { "gu", "guj", "guj", "Gujarati"},
    { "", "gwi", "gwi", "Gwich'in"},
    { "", "hai", "hai", "Haida"},
    { "ha", "hau", "hau", "Hausa"},
    { "", "haw", "haw", "Hawaiian"},
    { "iw", "heb", "heb", "Hebrew"},
    { "", "her", "her", "Herero"},
    { "", "hil", "hil", "Hiligaynon"},
    { "", "him", "him", "Himachali"},
    { "hi", "hin", "hin", "Hindi"},
    { "", "hmo", "hmo", "Hiri Motu"},
    { "", "hit", "hit", "Hittite"},
    { "", "hmn", "hmn", "Hmong"},
    { "hu", "hun", "hun", "Hungarian"},
    { "", "hup", "hup", "Hupa"},
    { "", "iba", "iba", "Iban"},
    { "is", "isl", "ice", "Icelandic"},
    { "", "ibo", "ibo", "Igbo"},
    { "", "ijo", "ijo", "Ijo"},
    { "", "ilo", "ilo", "Iloko"},
    { "", "inc", "inc", "Indic (Other)"},
    { "", "ine", "ine", "Indo-European (Other)"},
    { "in", "ind", "ind", "Indonesian"},
    { "ia", "ina", "ina", "Interlingua (International Auxilary Language Association)"},
    { "ie", "ile", "ile", "Interlingue"},
    { "iu", "iku", "iku", "Inuktitut"},
    { "ik", "ipk", "ipk", "Inupiak"},
    { "", "ira", "ira", "Iranian (Other)"},
    { "", "mga", "mga", "Irish, Middle (900-1200)"},
    { "", "sga", "sga", "Irish, Old (to 900)"},
    { "ga", "gai", "iri", "Irish"},
    { "", "iro", "iro", "Iroquoian languages"},
    { "it", "ita", "ita", "Italian"},
    { "ja", "jpn", "jpn", "Japanese"},
    { "jw", "jaw", "jav", "Javanese"},
    { "", "jrb", "jrb", "Judeo-Arabic"},
    { "", "jpr", "jpr", "Judeo-Persian"},
    { "", "kab", "kab", "Kabyle"},
    { "", "kac", "kac", "Kachin"},
    { "kl", "kal", "kal", "Kalaallisut (Greenlandic)"},
    { "", "kam", "kam", "Kamba"},
    { "kn", "kan", "kan", "Kannada"},
    { "", "kau", "kau", "Kanuri"},
    { "", "kaa", "kaa", "Kara-Kalpak"},
    { "", "kar", "kar", "Karen"},
    { "ks", "kas", "kas", "Kashmiri"},
    { "", "kaw", "kaw", "Kawi"},
    { "kk", "kaz", "kaz", "Kazakh"},
    { "", "kha", "kha", "Khasi"},
    { "km", "khm", "khm", "Khmer"},
    { "", "khi", "khi", "Khoisan (Other)"},
    { "", "kho", "kho", "Khotanese"},
    { "", "kik", "kik", "Kikuyu"},
    { "", "kmb", "kmb", "Kimbundu"},
    { "rw", "kin", "kin", "Kinyarwanda"},
    { "ky", "kir", "kir", "Kirghiz"},
    { "", "kom", "kom", "Komi"},
    { "", "kon", "kon", "Kongo"},
    { "", "kok", "kok", "Konkani"},
    { "ko", "kor", "kor", "Korean"},
    { "", "kos", "kos", "Kosraean"},
    { "", "kpe", "kpe", "Kpelle"},
    { "", "kro", "kro", "Kru"},
    { "", "kua", "kua", "Kuanyama"},
    { "", "kum", "kum", "Kumyk"},
    { "ku", "kur", "kur", "Kurdish"},
    { "", "kru", "kru", "Kurukh"},
    { "", "kus", "kus", "Kusaie"},
    { "", "kut", "kut", "Kutenai"},
    { "", "lad", "lad", "Ladino"},
    { "", "lah", "lah", "Lahnda"},
    { "", "lam", "lam", "Lamba"},
    { "lo", "lao", "lao", "Lao"},
    { "la", "lat", "lat", "Latin"},
    { "lv", "lav", "lav", "Latvian"},
    { "", "ltz", "ltz", "Letzeburgesch"},
    { "", "lez", "lez", "Lezghian"},
    { "ln", "lin", "lin", "Lingala"},
    { "lt", "lit", "lit", "Lithuanian"},
    { "", "loz", "loz", "Lozi"},
    { "", "lub", "lub", "Luba-Katanga"},
    { "", "lua", "lua", "Luba-Lulua"},
    { "", "lui", "lui", "Luiseno"},
    { "", "lun", "lun", "Lunda"},
    { "", "luo", "luo", "Luo (Kenya and Tanzania)"},
    { "", "lus", "lus", "Lushai"},
    { "mk", "mkd", "mac", "Macedonian"},
    { "", "mad", "mad", "Madurese"},
    { "", "mag", "mag", "Magahi"},
    { "", "mai", "mai", "Maithili"},
    { "", "mak", "mak", "Makasar"},
    { "mg", "mlg", "mlg", "Malagasy"},
    { "ms", "msa", "may", "Malay"},
    { "ml", "mal", "mal", "Malayalam"},
    { "mt", "mlt", "mlt", "Maltese"},
    { "", "mdr", "mdr", "Mandar"},
    { "", "man", "man", "Mandingo"},
    { "", "mni", "mni", "Manipuri"},
    { "", "mno", "mno", "Manobo languages"},
    { "", "max", "max", "Manx"},
    { "mi", "mri", "mao", "Maori"},
    { "mr", "mar", "mar", "Marathi"},
    { "", "chm", "chm", "Mari"},
    { "", "mah", "mah", "Marshall"},
    { "", "mwr", "mwr", "Marwari"},
    { "", "mas", "mas", "Masai"},
    { "", "myn", "myn", "Mayan languages"},
    { "", "men", "men", "Mende"},
    { "", "mic", "mic", "Micmac"},
    { "", "min", "min", "Minangkabau"},
    { "", "mis", "mis", "Miscellaneous languages"},
    { "", "moh", "moh", "Mohawk"},
    { "mo", "mol", "mol", "Moldavian"},
    { "", "mkh", "mkh", "Mon-Khmer (Other)"},
    { "", "lol", "lol", "Mongo"},
    { "mn", "mon", "mon", "Mongolian"},
    { "", "mos", "mos", "Mossi"},
    { "", "mul", "mul", "Multiple languages"},
    { "", "mun", "mun", "Munda languages"},
    { "na", "nau", "nau", "Nauru"},
    { "", "nav", "nav", "Navajo"},
    { "", "nde", "nde", "Ndebele, North"},
    { "", "nbl", "nbl", "Ndebele, South"},
    { "", "ndo", "ndo", "Ndonga"},
    { "ne", "nep", "nep", "Nepali"},
    { "", "new", "new", "Newari"},
    { "", "nia", "nia", "Nias"},
    { "", "nic", "nic", "Niger-Kordofanian (Other)"},
    { "", "ssa", "ssa", "Nilo-Saharan (Other)"},
    { "", "niu", "niu", "Niuean"},
    { "", "non", "non", "Norse, Old"},
    { "", "nai", "nai", "North American Indian (Other)"},
    { "no", "nor", "nor", "Norwegian"},
    { "", "nub", "nub", "Nubian languages"},
    { "", "nym", "nym", "Nyamwezi"},
    { "", "nya", "nya", "Nyanja"},
    { "", "nyn", "nyn", "Nyankole"},
    { "", "nyo", "nyo", "Nyoro"},
    { "", "nzi", "nzi", "Nzima"},
    { "oc", "oci", "oci", "Occitan (post 1500)"},
    { "", "oji", "oji", "Ojibwa"},
    { "or", "ori", "ori", "Oriya"},
    { "om", "orm", "orm", "Oromo"},
    { "", "osa", "osa", "Osage"},
    { "", "oss", "oss", "Ossetic"},
    { "", "oto", "oto", "Otomian languages"},
    { "", "pal", "pal", "Pahlavi"},
    { "", "pau", "pau", "Palauan"},
    { "", "pli", "pli", "Pali"},
    { "", "pam", "pam", "Pampanga"},
    { "", "pag", "pag", "Pangasinan"},
    { "pa", "pan", "pan", "Panjabi"},
    { "", "pap", "pap", "Papiamento"},
    { "", "paa", "paa", "Papuan (Other)"},
    { "", "peo", "peo", "Persian, Old (ca. 600-400 B.C.)"},
    { "fa", "fas", "per", "Persian"},
    { "", "phi", "phi", "Philippine (Other)"},
    { "", "phn", "phn", "Phoenician"},
    { "", "pon", "pon", "Pohnpeian"},
    { "pl", "pol", "pol", "Polish"},
    { "pt", "por", "por", "Portuguese"},
    { "", "pra", "pra", "Prakrit languages"},
    { "", "pro", "pro", "Provençal, Old (to 1500)"},
    { "ps", "pus", "pus", "Pushto"},
    { "qu", "que", "que", "Quechua"},
    { "rm", "roh", "roh", "Rhaeto-Romance"},
    { "", "raj", "raj", "Rajasthani"},
    { "", "rap", "rap", "Rapanui"},
    { "", "rar", "rar", "Rarotongan"},
    { "", "roa", "roa", "Romance (Other)"},
    { "ro", "ron", "rum", "Romanian"},
    { "", "rom", "rom", "Romany"},
    { "rn", "run", "run", "Rundi"},
    { "ru", "rus", "rus", "Russian"},
    { "", "sal", "sal", "Salishan languages"},
    { "", "sam", "sam", "Samaritan Aramaic"},
    { "", "smi", "smi", "Sami languages"},
    { "sm", "smo", "smo", "Samoan"},
    { "", "sad", "sad", "Sandawe"},
    { "sg", "sag", "sag", "Sango"},
    { "sa", "san", "san", "Sanskrit"},
    { "", "sat", "sat", "Santali"},
    { "", "srd", "srd", "Sardinian"},
    { "", "sas", "sas", "Sasak"},
    { "", "sco", "sco", "Scots"},
    { "", "sel", "sel", "Selkup"},
    { "", "sem", "sem", "Semitic (Other)"},
    { "sr", "srp", "scc", "Serbian"},
    { "sh", "scr", "scr", "Serbo-Croatian"},
    { "", "srr", "srr", "Serer"},
    { "", "shn", "shn", "Shan"},
    { "sn", "sna", "sna", "Shona"},
    { "", "sid", "sid", "Sidamo"},
    { "", "bla", "bla", "Siksika"},
    { "sd", "snd", "snd", "Sindhi"},
    { "si", "sin", "sin", "Singhalese"},
    { "", "sit", "sit", "Sino-Tibetan (Other)"},
    { "", "sio", "sio", "Siouan languages"},
    { "ss", "ssw", "ssw", "Siswant"},
    { "", "den", "den", "Slave (Athapascan)"},
    { "", "sla", "sla", "Slavic (Other)"},
    { "sk", "slk", "slo", "Slovak"},
    { "sl", "slv", "slv", "Slovenian"},
    { "", "sog", "sog", "Sogdian"},
    { "", "nso", "nso", "Sotho, Northern"},
    { "so", "som", "som", "Somali"},
    { "", "son", "son", "Songhai"},
    { "", "snk", "snk", "Soninke"},
    { "", "wen", "wen", "Sorbian languages"},
    { "st", "sot", "sot", "Sotho, Southern"},
    { "", "sai", "sai", "South American Indian (Other)"},
    { "es", "spa", "spa", "Spanish"},
    { "su", "sun", "sun", "Sudanese"},
    { "", "suk", "suk", "Sukuma"},
    { "", "sux", "sux", "Sumerian"},
    { "", "sus", "sus", "Susu"},
    { "sw", "swa", "swa", "Swahili"},
    { "", "ssw", "ssw", "Swati"},
    { "sv", "swe", "swe", "Swedish"},
    { "", "syr", "syr", "Syriac"},
    { "tl", "tgl", "tgl", "Tagalog"},
    { "", "tah", "tah", "Tahitian"},
    { "", "tai", "tai", "Tai (Other)"},
    { "tg", "tgk", "tgk", "Tajik"},
    { "", "tmh", "tmh", "Tamashek"},
    { "ta", "tam", "tam", "Tamil"},
    { "tt", "tat", "tat", "Tatar"},
    { "te", "tel", "tel", "Telugu"},
    { "", "ter", "ter", "Tereno"},
    { "", "tet", "tet", "Tetum"},
    { "th", "tha", "tha", "Thai"},
    { "bo", "bod", "tib", "Tibetan"},
    { "", "tig", "tig", "Tigre"},
    { "ti", "tir", "tir", "Tigrinya"},
    { "", "tem", "tem", "Timne"},
    { "", "tiv", "tiv", "Tiv"},
    { "", "tli", "tli", "Tlingit"},
    { "", "tpi", "tpi", "Tok Pisin"},
    { "", "tkl", "tkl", "Tokelau"},
    { "to", "tog", "tog", "Tonga (Nyasa)"},
    { "", "ton", "ton", "Tonga (Tonga Islands)"},
    { "", "tsi", "tsi", "Tsimshian"},
    { "ts", "tso", "tso", "Tsonga"},
    { "tn", "tsn", "tsn", "Tswana"},
    { "", "tum", "tum", "Tumbuka"},
    { "", "ota", "ota", "Turkish, Ottoman (1500-1928)"},
    { "tr", "tur", "tur", "Turkish"},
    { "tk", "tuk", "tuk", "Turkmen"},
    { "", "tvl", "tvl", "Tuvalu"},
    { "", "tyv", "tyv", "Tuvinian"},
    { "tw", "twi", "twi", "Twi"},
    { "", "uga", "uga", "Ugaritic"},
    { "ug", "uig", "uig", "Uighur"},
    { "uk", "ukr", "ukr", "Ukrainian"},
    { "", "umb", "umb", "Umbundu"},
    { "", "und", "und", "Undetermined"},
    { "ur", "urd", "urd", "Urdu"},
    { "uz", "uzb", "uzb", "Uzbek"},
    { "", "vai", "vai", "Vai"},
    { "", "ven", "ven", "Venda"},
    { "vi", "vie", "vie", "Vietnamese"},
    { "vo", "vol", "vol", "Volapük"},
    { "", "vot", "vot", "Votic"},
    { "", "wak", "wak", "Wakashan languages"},
    { "", "wal", "wal", "Walamo"},
    { "", "war", "war", "Waray"},
    { "", "was", "was", "Washo"},
    { "cy", "cym", "wel", "Welsh"},
    { "wo", "wol", "wol", "Wolof"},
    { "xh", "xho", "xho", "Xhosa"},
    { "", "yao", "yao", "Yao"},
    { "", "yap", "yap", "Yapese"},
    { "ji", "yid", "yid", "Yiddish"},
    { "yo", "yor", "yor", "Yoruba"},
    { "", "ypk", "ypk", "Yupik languages"},
    { "", "znd", "znd", "Zande"},
    { "", "zap", "zap", "Zapotec"},
    { "", "zen", "zen", "Zenaga"},
    { "za", "zha", "zha", "Zhuang"},
    { "zu", "zul", "zul", "Zulu"},
    { "", "zun", "zun", "Zuni"},
    { 0, 0, 0, 0 }
};


typedef struct stuISO639TreeNode{
    char Key;
    stuISO639TreeNode* FirstChild;
    stuISO639TreeNode* SiblingNode;
    const stuISO639Info* Info;
}stuISO639TreeNode;

/**
 * @brief _ISO639newNode Private method to construct a new stuISO639TreeNode
 * @param _key Key to assign to then constructed node
 * @return new created node
 */
static stuISO639TreeNode* _ISO639newNode(char _key, const stuISO639Info* _info){
    (void)_ISO639newNode; //suppress compiler warning

    stuISO639TreeNode* Node = (stuISO639TreeNode*)malloc(sizeof(stuISO639TreeNode));
    Node->Key = _key;
    Node->Info = _info;
    Node->SiblingNode = NULL;
    Node->FirstChild = NULL;
    return Node;
}

/**
 * @brief _ISO639followOrCreate Traverse tree by input key or create new node storing proposed info
 * @param _key Key of current node
 * @param _currNode Node from traverse is starting
 */
static void _ISO639followOrCreate(char _key,
                                  stuISO639TreeNode**& _pCurrNode,
                                  const stuISO639Info* _info){
    (void)_ISO639followOrCreate; //suppress compiler warning

    if(*_pCurrNode){
        while(true){
            if ((*_pCurrNode)->Key == _key){
                if((*_pCurrNode)->Info == NULL)
                    (*_pCurrNode)->Info = _info;
                _pCurrNode = &(*_pCurrNode)->FirstChild;
                break;
            }else if((*_pCurrNode)->SiblingNode){
                _pCurrNode = &(*_pCurrNode)->SiblingNode;
            }else{
                (*_pCurrNode)->SiblingNode = _ISO639newNode(_key,_info);
                _pCurrNode = &(*_pCurrNode)->SiblingNode->FirstChild;
                break;
            }
        }
    }else{
        *_pCurrNode = _ISO639newNode(_key,_info);
        _pCurrNode = &(*_pCurrNode)->FirstChild;
    }
}

#ifndef NULL
#define NULL 0
#endif

#ifndef likely
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#endif

static stuISO639TreeNode* ISO639TreeRoot = NULL;
static const stuISO639Info*     InvalidInfo;

/**
 * @brief init initializes ISO639Tree to perform fast search.
 * This function will be called implicitily by other functions if needed so no need to call it separately
 */
static void ISO639init(){
    (void)ISO639init; //suppress compiler warning
    if (unlikely(ISO639TreeRoot != NULL))
        return;

    const stuISO639Info* Iter = ISO639Table;
    ISO639TreeRoot = _ISO639newNode(0,NULL);

    while(Iter->FullName){
        //Insert ISO639_1
        if(Iter->ISO639_1[0] != '\0'){
            stuISO639TreeNode** pCurrNode            = &ISO639TreeRoot->FirstChild;
            _ISO639followOrCreate(Iter->ISO639_1[0], pCurrNode, NULL);
            _ISO639followOrCreate(Iter->ISO639_1[1], pCurrNode, Iter);
        }

        //Insert ISO639_2B
        if(Iter->ISO639_2B[0] != '\0'){
            stuISO639TreeNode** pCurrNode            = &ISO639TreeRoot->FirstChild;
            _ISO639followOrCreate(Iter->ISO639_2B[0], pCurrNode, NULL);
            _ISO639followOrCreate(Iter->ISO639_2B[1], pCurrNode, NULL);
            _ISO639followOrCreate(Iter->ISO639_2B[2], pCurrNode, Iter);
        }

        //Insert ISO639_2T
        if(Iter->ISO639_2T[0] != '\0'){
            stuISO639TreeNode** pCurrNode            = &ISO639TreeRoot->FirstChild;
            _ISO639followOrCreate(Iter->ISO639_2T[0], pCurrNode, NULL);
            _ISO639followOrCreate(Iter->ISO639_2T[1], pCurrNode, NULL);
            _ISO639followOrCreate(Iter->ISO639_2T[2], pCurrNode, Iter);
        }
        ++Iter;
    }
    InvalidInfo = Iter;
}

/**
 * @brief ISO639getInfo is used to get corresponding struct based on input code
 * @param _code must be ISO639-1 OR ISO639-2/B OR ISO639-2/T
 * @return if found a valid stuISO639 struct with corresponding fields
 *         else a stuISO639 struct with all fields set to NULL
 */
static const stuISO639Info& ISO639getInfo(const char* _code){
    if (unlikely(ISO639TreeRoot == NULL))
        ISO639init();
    if (_code == NULL || _code[0] == '\0')
        return *InvalidInfo;
    stuISO639TreeNode* GoalNode = ISO639TreeRoot;
    stuISO639TreeNode* CurrNode = ISO639TreeRoot->FirstChild;
    const char* CodeIter = _code;
    while (CodeIter && *CodeIter && GoalNode){
        GoalNode = NULL;
        if(CurrNode){
            while(true){
                if (CurrNode->Key == *CodeIter){
                    GoalNode = CurrNode;
                    CurrNode = CurrNode->FirstChild;
                    break;
                }else if(CurrNode->SiblingNode)
                    CurrNode = CurrNode->SiblingNode;
                else
                    break;
            }
        }else
            break;
        ++CodeIter;
    }

    if (GoalNode)
        return *GoalNode->Info;
    else
        return *InvalidInfo;
}

/**
 * @brief ISO639isValid checks wheter input code is valid ISO639 code or not
 * @param _code two or three characters code
 * @return true on valid ISO639 codes else false
 */
static bool ISO639isValid(const char* _code){
    (void)ISO639isValid; //suppress compiler warning
    return ISO639getInfo(_code).FullName != 0;
}

/**
 * @brief ISO639getAlpha2 Retrieve ISO639-1 code of input code.
 * @param _code Input ISO639 code
 * @return ISO639-1 code on valid input codes else a stuISO639 struct with all fields set to NULL
 */
static const char* ISO639getAlpha2(const char* _code){
    (void)ISO639getAlpha2; //suppress compiler warning
    return ISO639getInfo(_code).ISO639_1;
}

/**
 * @brief ISO639getAlpha3B Retrieve ISO639-2/B code of input code.
 * @param _code Input ISO639 code
 * @return ISO639-2/B code on valid input codes else a stuISO639 struct with all fields set to NULL
 */
static const char* ISO639getAlpha3B(const char* _code){
    (void)ISO639getAlpha3B; //suppress compiler warning
    return ISO639getInfo(_code).ISO639_2B;
}

/**
 * @brief ISO639getAlpha3B Retrieve ISO639-2/T code of input code.
 * @param _code Input ISO639 code
 * @return ISO639-2/T code on valid input codes else a stuISO639 struct with all fields set to NULL
 */
static const char* ISO639getAlpha3T(const char* _code){
    (void)ISO639getAlpha3T; //suppress compiler warning
    return ISO639getInfo(_code).ISO639_2T;
}

/**
 * @brief ISO639getName Retrieve ISO639 name of input code
 * @param _code Input ISO639 code
 * @return ISO name on valid ISO codes and NULL on invalid inputs.
 */
static const char* ISO639getName(const char* _code){
    (void)ISO639getName; //suppress compiler warning
    return ISO639getInfo(_code).FullName;
}

#endif // ISO639_H
