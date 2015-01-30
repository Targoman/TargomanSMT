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

quint8 ARPAManager::load(const QString &_file, intfBaseModel* _model)
{
    TargomanLogInfo(5, "Loading ARPA File: " + _file);

    std::ifstream File(_file.toUtf8().constData());
    if (File.is_open() == false)
        throw exARPAManager("Unable to open ARPA File: " + _file);

    std::string LineString;

    quint8        NGramOrder, MaxGram = 0;
    quint32       LineNo = 0, Count = 0;
    float         Prob, Backoff;
    bool          IsOK;
    enuParseState::Type ParseState = enuParseState::Looking4Start;
    QVector<quint32> NGramCounts;
    size_t Dummy;
    size_t Pos;

    const char* StartOfNGram, *EndOfNGram, *StartOfBackoff;
    Targoman::Common::clsCmdProgressBar ProgressBar;

    while (std::getline(File, LineString)) {
        ++LineNo;

        if (LineString.size() == LM_MAX_VALID_ARPA_LINE)
            throw exARPAManager(QString("Invalid ARPA file as line <%1> is longer than: %2 characters").arg(
                                    LineNo).arg(
                                    LM_MAX_VALID_ARPA_LINE));

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

                NGramCounts.resize(NGramOrder);
                NGramCounts.insert(NGramOrder, Count);

                MaxGram = qMax(MaxGram, NGramOrder);
            }else if (Line.startsWith('\\') && Line.endsWith("-grams:")){
                NGramOrder = Line.split("-").first().remove(0,1).toInt(&IsOK);
                if (!IsOK || NGramOrder != 1)
                    throw exARPAManager("Invalid ARPA file as it has been started with greater order NGram than 1");

                quint32 MaxItems = 0;
                for (int i=1; i<MaxGram; ++i){
                    if (NGramCounts.value(i) == 0)
                        throw exARPAManager(QString("ARPA File has no item for %1Gram which is prior to %2Gram").arg(
                                                i).arg(MaxGram));
                    MaxItems += NGramCounts.value(i);
                }
                _model->init(MaxItems);

                ProgressBar.reset("Loading 1-Gram Items",NGramCounts.value(1));
                ParseState = enuParseState::NGram;
                Count = 0;
            }else
                throw exARPAManager(QString("Invalid Identifier at line: %1").arg(LineNo));
            break;
        }
        case enuParseState::NGram:
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
                    ProgressBar.reset(QString("Loading %1-Gram Items").arg(NGramOrder),NGramCounts.value(NGramOrder));
                }else if (Line == "\\end\\"){
                    if (Count < NGramCounts.value(NGramOrder))
                        throw exARPAManager(QString("There are less Items specified for Ngram=%1 than specified: %2").arg(
                                                NGramOrder).arg(NGramCounts.value(NGramOrder)));
                    TargomanLogInfo(5, "ARPA File Loaded. " + _model->getStatsStr());

                    return MaxGram;
                }else
                    throw exARPAManager(QString("Invalid Tag at Line: %1").arg(LineNo));
            }else{
                if (Count > NGramCounts.value(NGramOrder))
                    throw exARPAManager(QString("There are more Items specified for Ngram=%1 than specified: %2 vs %3").arg(
                                            NGramOrder).arg(Count).arg(NGramCounts.value(NGramOrder)));
                Prob = Targoman::Common::fastASCII2Float(LineString.c_str(), Pos);
                if (Prob > 0)
                    throw exARPAManager(QString("Invalid positive probaility at line : %1").arg(LineNo));

                EndOfNGram = LineString.c_str() + Pos;
                Targoman::Common::fastSkip2NonSpace(EndOfNGram);
                StartOfNGram = EndOfNGram;
                if (!*EndOfNGram)
                    throw exARPAManager(QString("Invalid count of Tokens 1 vs 0"));
                Targoman::Common::fastSkip2Space(EndOfNGram);
                for (int i=1; i<NGramOrder; ++i){
                    Targoman::Common::fastSkip2NonSpace(++EndOfNGram);
                    if (!*EndOfNGram)
                        throw exARPAManager(QString("Invalid count of Tokens %1 vs %2 at line: %3").arg(
                                                i).arg(
                                                NGramOrder).arg(
                                                LineNo));
                    Targoman::Common::fastSkip2Space(EndOfNGram);
                }
                *((char*)EndOfNGram) = '\0';

                if (EndOfNGram - LineString.c_str() < (qint64)LineString.size()){
                    StartOfBackoff = EndOfNGram+1;
                    Targoman::Common::fastSkip2NonSpace(StartOfBackoff);
                    Backoff = Targoman::Common::fastASCII2Float(StartOfBackoff, Dummy);
                }else
                    Backoff = 0;
                if (Prob >=0)
                    throw exARPAManager(QString("Invalid Positive backoff at line: %1").arg(LineNo));

                _model->insert(StartOfNGram, Prob, Backoff);
                ++Count;
                ProgressBar.setValue(Count);
            }
        default:
            break;
        }
    }
    TargomanLogInfo(5, "ARPA File Loaded. " + _model->getStatsStr());

    return MaxGram;
}


}
}
}
