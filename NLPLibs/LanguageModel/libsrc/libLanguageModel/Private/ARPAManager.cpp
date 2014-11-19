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

#include "ARPAManager.h"

#include <QString>
#include <QStringList>
#include <QVector>

#include <fstream>
#include <sstream>

#include "libTargomanCommon/clsCmdProgressBar.h"
#include "libTargomanCommon/Logger.h"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/FastOperations.hpp"

namespace Targoman {
namespace NLPLibs {
namespace Private {

TARGOMAN_DEFINE_ENHANCED_ENUM_BEGIN(enuParseState)
Looking4Start,
Started,
NGram
TARGOMAN_DEFINE_ENUM_END

ARPAManager* ARPAManager::Instance = NULL;

QString ActorUUID;

ARPAManager::ARPAManager()
{
    TARGOMAN_REGISTER_ACTOR("LM::ARPAManager");
}

#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')

inline float fastASCII2Float (const char *pFloatString, size_t& _lastPos)
{
    qint64 IntValue, Scale = 1;
    qint16 Sign;
    const char*  StartOfString = pFloatString;

    Sign = 1;
    if (*pFloatString == '-') {
        Sign = -1;
        ++pFloatString;
    } else if (*pFloatString == '+') {
        ++pFloatString;
    }

    // Get digits before decimal point or exponent, if any.
    for (IntValue = 0; valid_digit(*pFloatString); ++pFloatString) {
        IntValue = IntValue * 10 + (*pFloatString - '0');
    }

    // Get digits after decimal point, if any.
    if (*pFloatString == '.') {
        pFloatString += 1;
        while (valid_digit(*pFloatString) && Scale < 10000000L) {
            IntValue = IntValue * 10 + (*pFloatString - '0');
            Scale*=10;
            ++pFloatString;
        }
    }

    _lastPos = pFloatString - StartOfString;
    // Return signed and scaled floating point result.
    return ((float)(Sign * IntValue))/(float)Scale;
}

inline const char* skip2NonSpace(const char*& _str){
    while(*_str)
        if(*_str == ' ' || *_str == '\t' || *_str == '\r' || *_str == '\n')
            _str++;
        else
            return _str;
    return _str;
}

inline const char* skip2Space(const char*& _str){
    while(*_str)
        if(*_str == ' ' || *_str == '\t' || *_str == '\r' || *_str == '\n')
            return _str;
        else
            _str++;
    return _str;
}
quint8 ARPAManager::load(const QString &_file, clsBaseModel* _model)
{
    TargomanLogInfo(5, "Loading ARPA File: " + _file);

    std::ifstream File(_file.toUtf8().constData());
    if (File.is_open() == false)
        throw exARPAManager("Unable to open ARPA File: " + _file);

    std::string LineString;
    std::stringstream LineStream;

    quint8        NGramOrder, MaxGram = 0;
    quint32       LineNo = 0, Count = 0;
    float         Prob, Backoff;
    bool          IsOK;
    enuParseState::Type ParseState = enuParseState::Looking4Start;
    QMap<quint8, quint32> NGramCounts;
    size_t Dummy;
    size_t Pos,SpacePos;
    const char* StartOfNGram, *EndOfNGram, *StartOfBackoff;
  //  Targoman::Common::clsCmdProgressBar ProgressBar;
    NGram_t NGram;

    /*   QString       Line;
    QStringList   LineParts;
*/


    while (std::getline(File, LineString)) {
        LineNo++;

        if (LineString.size() == LM_MAX_VALID_ARPA_LINE)
            throw exARPAManager(QString("Invalid ARPA file as line <%1> is longer than: %2 characters").arg(
                                    LineNo).arg(
                                    LM_MAX_VALID_ARPA_LINE));

        //LineString = Targoman::Common::fastTrimStdString(LineString);

        Targoman::Common::fastTrimStdString(LineString);

        if (LineString.empty() || LineString.at(0) == '#')
            continue;

        switch (ParseState) {
        case enuParseState::Looking4Start:
            if (LineString=="\\data\\")
                ParseState = enuParseState::Started;
            break;
        case enuParseState::Started:
        {
            QString Line = QString::fromStdString(LineString);
            if (Line.startsWith("ngram")){
                QStringList LineParts = Line.split("=", QString::SkipEmptyParts);
                if (LineParts.size() != 2)
                    throw exARPAManager(QString("Invalid ARPA file as it has invalid Ngram count Def. at line: %1").arg(
                                            Line));
                NGramOrder = LineParts.first().remove("ngram").trimmed().toUInt(&IsOK);
                if (!IsOK || NGramOrder == 0)
                    throw exARPAManager(QString("Invalid ARPA NGram Description at line: %1").arg(LineNo));
                if (NGramOrder > LM_MAX_ORDER)
                    throw exARPAManager(QString("NGram Order grater than %1 is not supported").arg(LM_MAX_ORDER));

                Count = LineParts.last().trimmed().toUInt(&IsOK);

                if (!IsOK)
                    throw exARPAManager(QString("Invalid ARPA NGram Description at line: %1").arg(LineNo));

                NGramCounts.insert(NGramOrder, Count);

                MaxGram = qMax(MaxGram, NGramOrder);
            }else if (Line.startsWith('\\') && Line.endsWith("-grams:")){
                NGramOrder = Line.split("-").first().remove(0,1).toInt(&IsOK);
                if (!IsOK || NGramOrder != 1)
                    throw exARPAManager("Invalid ARPA file as it has been started with greater order NGram than 1");

                for (int i=1; i<MaxGram; i++){
                    if (NGramCounts.value(MaxGram) == 0)
                        throw exARPAManager(QString("ARPA File has no item for %1Gram which is prior to %2Gram").arg(
                                                i).arg(MaxGram));
                }

                //ProgressBar.reset("Loading 1 Gram Items",NGramCounts.value(1));
                ParseState = enuParseState::NGram;
                NGram.resize(1);
                Count = 0;
            }else
                throw exARPAManager(QString("Invalid Identifier at line: %1").arg(LineNo));
            break;
        }
        case enuParseState::NGram:
            Count++;

            if (LineString.at(0) == '\\'){
                QString Line = QString::fromStdString(LineString);
                if (Line.endsWith("-grams:")){
                    if (Count < NGramCounts.value(NGramOrder))
                        throw exARPAManager(QString("There are less Items specified for Ngram=%1 than specified: %2").arg(
                                                NGramOrder).arg(NGramCounts.value(NGramOrder)));

                    NGramOrder = Line.split("-").first().remove(0,1).toInt(&IsOK);
                    if (!IsOK || NGramOrder < 2)
                        throw exARPAManager(QString("Invalid ARPA file as invalid gram section is found at line: %1").arg(LineNo));
                    Count = 0;
                    NGram.resize(NGramOrder);
                    //ProgressBar.reset(QString("Loading %1 Gram Items").arg(NGramOrder),NGramCounts.value(NGramOrder));
                }else if (Line == "\\end\\"){
                    if (Count < NGramCounts.value(NGramOrder))
                        throw exARPAManager(QString("There are less Items specified for Ngram=%1 than specified: %2").arg(
                                                NGramOrder).arg(NGramCounts.value(NGramOrder)));
                    TargomanLogInfo(5, "ARPA File Loaded");
                    return MaxGram;
                }else
                    throw exARPAManager(QString("Invalid Tag at Line: %1").arg(LineNo));
            }else{
                Count++;
/*                if (Count > NGramCounts.value(NGramOrder))
                    throw exARPAManager(QString("There are more Items specified for Ngram=%1 than specified: %2").arg(
                                            NGramOrder).arg(NGramCounts.value(NGramOrder)));*/
                Prob = fastASCII2Float(LineString.c_str(), Pos);

                EndOfNGram = LineString.c_str() + Pos;
                skip2NonSpace(EndOfNGram);
                StartOfNGram = EndOfNGram;
                if (!*EndOfNGram)
                    throw exARPAManager(QString("Invalid count of Tokens 1 vs 0"));
                skip2Space(EndOfNGram);
                *((char*)EndOfNGram) = '\0';

                if (NGramOrder == 1 &&
                    strcmp(StartOfNGram, LM_UNKNOWN_WORD) &&
                    strcmp(StartOfNGram, LM_BEGIN_SENTENCE) &&
                    strcmp(StartOfNGram, LM_END_SENTENCE)){
                    _model->add2Vocab(StartOfNGram);
                }
                //NGram[0] = 1;
                NGram[0] =(_model->vocab().getIndex(StartOfNGram));
                for (int i=1; i<NGramOrder; ++i){
                    skip2NonSpace(++EndOfNGram);
                    if (!*EndOfNGram)
                        throw exARPAManager(QString("Invalid count of Tokens %1 vs %2 at line: %3").arg(
                                                i).arg(
                                                NGramOrder).arg(
                                                LineNo));
                    StartOfNGram = EndOfNGram;
                    skip2Space(EndOfNGram);
                    *((char*)EndOfNGram) = '\0';
                    //NGram[i] = 1;
                    NGram[i] = (_model->vocab().getIndex(StartOfNGram));
                }
                if (EndOfNGram - LineString.c_str() < LineString.size()){
                    StartOfBackoff = EndOfNGram+1;
                    skip2NonSpace(StartOfBackoff);
                    Backoff = fastASCII2Float(StartOfBackoff, Dummy);
                }else
                    Backoff = 0;
                if (Prob >=0)
                    throw exARPAManager(QString("Invalid Positive backoff at line: %1").arg(LineNo));

                _model->insert(NGram, Prob, Backoff);
            }
        default:
            break;
        }
    }
    TargomanLogInfo(5, "ARPA File Loaded");

    return MaxGram;
}


}
}
}
