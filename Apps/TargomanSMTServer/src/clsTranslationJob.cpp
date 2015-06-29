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
 */

#include "clsTranslationJob.h"
#include <QtConcurrent/QtConcurrent>
#include <functional>
#include "libTargomanTextProcessor/TextProcessor.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/Helpers.hpp"
#include "libTargomanCommon/Types.h"

namespace Targoman {
namespace Apps {

using namespace SMT;
using namespace NLPLibs;
using namespace Common::Configuration;

QString clsTranslationJob::SourceLanguage;

clsTranslationJob::clsTranslationJob(bool _brief, bool _keepAsSource)
{
    this->Brief = _brief;
    this->KeepAsSource = _keepAsSource;
    TargomanTextProcessor::instance();
}

QVariantList clsTranslationJob::doJob(const QString &_inputStr)
{
    QStringList Lines = _inputStr.trimmed().split("\n");
    QVariantList Result = QtConcurrent::blockingMappedReduced<QVariantList>(
                Lines,
                std::bind(&clsTranslationJob::mapLineTranslation,this, std::placeholders::_1),
                std::bind(&clsTranslationJob::reduceLineTranslation,this, std::placeholders::_1, std::placeholders::_2),
                QtConcurrent::OrderedReduce);

    return Result;
}

stuTranslationOutput clsTranslationJob::mapLineTranslation(const QString &_line)
{
    if(_line.isEmpty())
        return stuTranslationOutput();

    stuTranslationOutput Result;
    bool SpellCorrected = false;
    QString TaggedSource = TargomanTextProcessor::instance().text2IXML(
                _line,
                SpellCorrected,
                clsTranslationJob::SourceLanguage);


    if (this->KeepAsSource){
        Result = Translator::translate(_line, this->Brief);
    }else{

        QStringList NormalizedSentences = TaggedSource.replace(
                    " . ", " .\n").split("\n",QString::SkipEmptyParts);

        Result = QtConcurrent::blockingMappedReduced<stuTranslationOutput>(
                    NormalizedSentences,
                    std::bind(&clsTranslationJob::mapSentenceTranslation,this, std::placeholders::_1),
                    std::bind(&clsTranslationJob::reduceSentenceTranslation,this, std::placeholders::_1, std::placeholders::_2),
                    QtConcurrent::OrderedReduce);
    }

    Result.TaggedSource = TaggedSource.replace('\n',' ');
    Result.SpellCorrected = SpellCorrected;
    Result.OriginalText   = _line;
    return Result;
}

stuTranslationOutput clsTranslationJob::mapSentenceTranslation(const QString &_ixml)
{
    return Translator::translate(_ixml, this->Brief);
}

void clsTranslationJob::reduceLineTranslation(QVariantList &_result,
                                              const stuTranslationOutput &_intermediate)
{
    if (_result.isEmpty()){
        _result.insert(0,QVariantList());//TranslationSection
        if (this->Brief == false){
            _result.insert(1,QVariantList());//IndexSection
            _result.insert(2,QVariantList());//AlignmentSection
        }
    }

    QVariantList TranslationSection = (_result[0].toList().isEmpty() ? QVariantList() : _result[0].toList());
    TranslationSection.insert(TranslationSection.size(),
                              QVariantList()<<_intermediate.Translation<<_intermediate.TaggedSource);
    _result[0] = TranslationSection;

    if (this->Brief == false){
        QVariantList PhraseIndexList;
        QVariantList AlignInfoList;
        QStringList TranslationWords = _intermediate.Translation.split(" ", QString::SkipEmptyParts);
        QStringList SourceWords = _intermediate.TaggedSource.split(' ', QString::SkipEmptyParts);

        QVector<stuPos> Tagged2OrigianlCharMap =
                Common::getCorrespondence(_intermediate.OriginalText,
                                          _intermediate.TaggedSource);

        QList<stuPos>  TaggedWordsCharMap;
        size_t LastAcceptedChar = 0;
        for(int i=0; i<SourceWords.size(); ++i){
            while(_intermediate.TaggedSource.at(LastAcceptedChar) == ' ')
                ++LastAcceptedChar;
            TaggedWordsCharMap.append(stuPos(LastAcceptedChar,
                                             LastAcceptedChar+SourceWords.at(i).size()));
            LastAcceptedChar+=SourceWords.at(i).size();
        }

/*        qDebug()<<TaggedWordsCharMap;
        qDebug()<<Tagged2OrigianlCharMap;
*/
        foreach(const stuTranslationOutput::stuMetaInfo& MetaInfo, _intermediate.MetaInfo){
            QString TargetPhrase = TranslationWords.at(MetaInfo.TargetWordsPos.start());
            for(size_t i=MetaInfo.TargetWordsPos.start() + 1; i<MetaInfo.TargetWordsPos.end(); ++i)
                TargetPhrase.append(" %1").arg(TranslationWords.at(i));
           PhraseIndexList.insert(PhraseIndexList.size(), QVariantList()<<TargetPhrase<<PhraseIndexList.size());

           QString SourcePhrase = SourceWords.at(MetaInfo.SourceWordsPos.start());
           for(size_t i=MetaInfo.SourceWordsPos.start() + 1; i<MetaInfo.SourceWordsPos.end(); ++i)
               SourcePhrase.append(" %1").arg(SourceWords.at(i));

           QVariantList TranslationOptions;
           bool IsFirstOption = true;
           foreach(const QString& Option, MetaInfo.TranslationOptions){
               TranslationOptions.insert(TranslationOptions.size(),QVariantList()<<Option<<IsFirstOption);
               if (IsFirstOption)
                   IsFirstOption = false;
           }

           QVariantList AlignInfo;
           AlignInfo.insert(0,SourcePhrase);
           AlignInfo.insert(1,AlignInfoList.size());
           AlignInfo.insert(2,TranslationOptions);


           QVariantList CharAlignInfo;
           qint32 FirstChar = INT_MAX;
           qint32 LastChar  = 0;

           stuPos TaggedSourceCharRange(TaggedWordsCharMap.at(MetaInfo.SourceWordsPos.start()).start(),
                                        TaggedWordsCharMap.at(MetaInfo.SourceWordsPos.end() - 1).end());
           for (size_t i=TaggedSourceCharRange.start(); i < TaggedSourceCharRange.end(); ++i){
               if (Tagged2OrigianlCharMap.at(i).isValid()){
                   FirstChar = qMin(Tagged2OrigianlCharMap.at(i).first, FirstChar);
                   LastChar = qMax(Tagged2OrigianlCharMap.at(i).second, LastChar);
               }
           }

           while (_intermediate.OriginalText.at(FirstChar) == ' ') ++FirstChar;
           while (_intermediate.OriginalText.at(LastChar - 1) == ' ') --LastChar;

           CharAlignInfo.insert(CharAlignInfo.size(), QVariantList()<<FirstChar<<LastChar);
/*           if (OriginalSourceCharRange.first > 0 && _intermediate.OriginalText.at(OriginalSourceCharRange.first - 1) != ' '){

           }
           if (OriginalSourceCharRange.second < _intermediate.OriginalText.size() &&
               _intermediate.OriginalText.at(OriginalSourceCharRange.second) != ' '){

           }*/


           AlignInfo.insert(3,CharAlignInfo);
           AlignInfo.insert(4,QVariantList()<<(quint16)MetaInfo.SourceWordsPos.start()<<(quint16)MetaInfo.SourceWordsPos.end());

           AlignInfoList.insert(AlignInfoList.size(), AlignInfo);
        }

        QVariantList IndexSection = _result[1].toList();
        IndexSection.insert(IndexSection.size(), PhraseIndexList);
        _result[1] = IndexSection;

        QVariantList AlignSection = _result[2].toList();
        AlignSection.insert(AlignSection.size(), AlignInfoList);
        _result[2] = AlignSection;
    }
    return;
}

void clsTranslationJob::reduceSentenceTranslation(stuTranslationOutput &_result,
                                                  const stuTranslationOutput &_intermediate)
{
    size_t Offset = 0;
    if (_result.Translation.size()){
        _result.Translation.append(" ");
        Offset = _result.Translation.count(' ');
    }

    _result.Translation.append(_intermediate.Translation);

    foreach(stuTranslationOutput::stuMetaInfo MetaInfo, _intermediate.MetaInfo){
        MetaInfo.SourceWordsPos.first  += Offset;
        MetaInfo.SourceWordsPos.second += Offset;
        MetaInfo.TargetWordsPos.first  += Offset;
        MetaInfo.TargetWordsPos.second += Offset;
        _result.MetaInfo.append(MetaInfo);
    }
}

}
}
