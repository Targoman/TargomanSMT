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

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>

#include "libTargomanCommon/Macros.h"

namespace Targoman {
namespace NLPLibs {
namespace Private {

TARGOMAN_DEFINE_ENHANCED_ENUM_BEGIN(enuParseState)
Looking4Start,
Started,
NGram
TARGOMAN_DEFINE_ENUM_END

ARPAManager* ARPAManager::Instance = NULL;

ARPAManager::ARPAManager()
{
}

quint8 ARPAManager::load(const QString &_file, clsBaseModel* _model)
{
    QFile ARPAFile(_file);
    QTextStream ARPAStream(&ARPAFile);
    ARPAStream.setCodec("UTF-8");
    ARPAFile.open(QFile::ReadOnly);

    if (ARPAFile.isReadable() == false)
        throw exARPAManager("Unable to open ARPA File: " + _file);

    QString       Line;
    QStringList   LineParts;
    enuParseState::Type ParseState = enuParseState::Looking4Start;
    quint8        NGramOrder, Count, MaxGram = 0;
    quint32       LineNo = 0;
    float         Prob, Backoff;
    bool          IsOK;
    QMap<quint8, quint32> NGramCounts;
    QList<WordIndex_t> NGram;

    while (!ARPAStream.atEnd()){
        LineNo++;
        Line = ARPAStream.readLine(LM_MAX_VALID_ARPA_LINE).trimmed();
        if (Line.size() == LM_MAX_VALID_ARPA_LINE)
            throw exARPAManager(QString("Invalid ARPA file as line <%1> is longer than: %2 characters").arg(
                                    LineNo).arg(
                                    LM_MAX_VALID_ARPA_LINE));

        if (Line.isEmpty() || Line.startsWith("#"))
            continue;

        switch (ParseState) {
        case enuParseState::Looking4Start:
            if (Line=="\\data\\")
                ParseState = enuParseState::Started;
            break;
        case enuParseState::Started:
            if (Line.startsWith("ngram")){
                LineParts = Line.split("=", QString::SkipEmptyParts);
                if (LineParts.size() != 2)
                    throw exARPAManager(QString("Invalid ARPA file as it has invalid Ngram count Def. at line: %1").arg(
                                            Line));
                NGramOrder = LineParts.first().remove("ngram").trimmed().toUInt(&IsOK);
                if (!IsOK || NGramOrder == 0)
                    throw exARPAManager(QString("Invalid ARPA NGram Description at line: %1").arg(Line));
                if (NGramOrder > LM_MAX_ORDER)
                    throw exARPAManager(QString("NGram Order grater than %1 is not supported").arg(LM_MAX_ORDER));

                Count = LineParts.last().trimmed().toUInt(&IsOK);
                if (!IsOK)
                    throw exARPAManager(QString("Invalid ARPA NGram Description at line: %1").arg(Line));

                NGramCounts.insert(NGramOrder, Count);
                MaxGram = qMax(MaxGram, NGramOrder);
            }else if (Line.startsWith("\\") && Line.endsWith("-grams:")){
                NGramOrder = Line.split("-").first().remove(0,1).toInt(&IsOK);
                if (!IsOK || NGramOrder != 1)
                    throw exARPAManager("Invalid ARPA file as it has been started with greater order NGram than 1");

                for (int i=1; i<MaxGram; i++){
                    if (NGramCounts.value(MaxGram) == 0)
                        throw exARPAManager(QString("ARPA File has no item for %1Gram which is prior to %2Gram").arg(
                                                i).arg(MaxGram));
                }

                //TODO check for overflow on unigram

                ParseState = enuParseState::NGram;
                Count = 0;
            }
            break;
        case enuParseState::NGram:
            if (Line.startsWith("\\") && Line.endsWith("-grams:")){
                if (Count < NGramCounts.value(NGramOrder))
                    throw exARPAManager(QString("There are less Items specified for Ngram=%1 than specified: %2").arg(
                            NGramOrder).arg(NGramCounts.value(NGramOrder)));
                NGramOrder = Line.split("-").first().remove(0,1).toInt(&IsOK);
                if (!IsOK || NGramOrder < 2)
                    throw exARPAManager(QString("Invalid ARPA file as invalid gram section is found at line: %1").arg(Line));
                Count = 0;
            }else if (Line == "\\end\\"){
                if (Count < NGramCounts.value(NGramOrder))
                    throw exARPAManager(QString("There are less Items specified for Ngram=%1 than specified: %2").arg(
                            NGramOrder).arg(NGramCounts.value(NGramOrder)));
                return MaxGram;
            }else{
                LineParts = Line.trimmed().split("\t", QString::SkipEmptyParts);

                if (LineParts.size() < 2)
                    throw exARPAManager(QString("Invalid ARPA Info at line: %1").arg(Line));

                if (NGramOrder == 1 &&
                        LineParts.at(1) != LM_UNKNOWN_WORD &&
                        LineParts.at(1) != LM_BEGIN_SENTENCE &&
                        LineParts.at(1) != LM_END_SENTENCE)
                    _model->add2Vocab(LineParts.at(1));

                Prob=LineParts.first().toFloat(&IsOK);
                if (!IsOK)
                    throw exARPAManager(QString("Invalid ARPA Info at line: %1").arg(Line));

                Backoff = LineParts.size() > 2 ? LineParts.last().toFloat(&IsOK) : 0;
                if (!IsOK)
                    throw exARPAManager(QString("Invalid ARPA Info at line: %1").arg(Line));

                //TODO error checking
                NGram.clear();
                foreach(const QString& Word, LineParts.mid(1, LineParts.size() -
                                                           (LineParts.size() > 2 ? 2 : 1))){
                    NGram.append(_model->vocab().getIndex(Word));
                }

                _model->insert(NGram, Prob, Backoff);

                Count++;
                if (Count > NGramCounts.value(NGramOrder))
                    throw exARPAManager(QString("There are more Items specified for Ngram=%1 than specified: %2").arg(
                            NGramOrder).arg(NGramCounts.value(NGramOrder)));
            }
            break;
        default:
            break;
        }
    }

    return MaxGram;
}


}
}
}
