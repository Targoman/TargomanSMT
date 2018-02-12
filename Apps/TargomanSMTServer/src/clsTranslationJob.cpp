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
QString clsTranslationJob::TargetLanguage;

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
        Result = Translator::translate(_line, this->Brief ? enuOutputFormat::JustBestTranslation : enuOutputFormat::BestTranslationAndPhraseSuggestions);
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
    Result.OriginalSource   = _line;
    return Result;
}

stuTranslationOutput clsTranslationJob::mapSentenceTranslation(const QString &_ixml)
{
    return Translator::translate(_ixml, this->Brief ? enuOutputFormat::JustBestTranslation : enuOutputFormat::BestTranslationAndPhraseSuggestions);
}

//TODO this method must be revised and optimized after upgrading WebInterface
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
                              QVariantList()<<
                              TargomanTextProcessor::instance().ixml2Text(_intermediate.Translations.first())<<
                              _intermediate.TaggedSource);
    _result[0] = TranslationSection;

    if (this->Brief == false){
        QMap<size_t, QString> PhraseIndexMap;
        QMap<size_t, QVariantList> AlignInfoMap;
        QStringList TranslationWords = _intermediate.Translations.first().split(" ", QString::SkipEmptyParts);
        QStringList SourceWords = _intermediate.TaggedSource.split(' ', QString::SkipEmptyParts);

        QVector<stuPos> Tagged2OrigianlCharMap =
                Common::getCorrespondence(_intermediate.OriginalSource,
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

        foreach(const stuTranslationOutput::stuPhraseAlternatives& MetaInfo, _intermediate.BestTranslationPhraseAlternatives){

            QString TargetPhrase;
            if ((size_t)TranslationWords.size() > MetaInfo.TargetWordsPos.start()){
                TargetPhrase = TranslationWords.at(MetaInfo.TargetWordsPos.start());
                for(size_t i=MetaInfo.TargetWordsPos.start() + 1;
                    (size_t)TranslationWords.size() > i && i<MetaInfo.TargetWordsPos.end();
                    ++i){
                    TargetPhrase.append(" ").append(TranslationWords.at(i));
                }
            }else{
                TargomanDebug(5, "TranslationWords Failed"<<
                              "TranslationWords.size()"<<TranslationWords.size()<<
                              "MetaInfo.TargetWordsPos.start()"<<MetaInfo.TargetWordsPos.start())
            }

            PhraseIndexMap.insert(MetaInfo.TargetWordsPos.start(),
                                   TargomanTextProcessor::instance().ixml2Text(TargetPhrase,
                                                                               clsTranslationJob::TargetLanguage));

            QString SourcePhrase;
            if((size_t)SourceWords.size() > MetaInfo.SourceWordsPos.start()){
                SourcePhrase = SourceWords.at(MetaInfo.SourceWordsPos.start());
                for(size_t i=MetaInfo.SourceWordsPos.start() + 1;
                    (size_t)SourceWords.size() > i && i<MetaInfo.SourceWordsPos.end();
                    ++i){
                    SourcePhrase.append(" ").append(SourceWords.at(i));
                }
            }else{
                TargomanDebug(5, "SourceWords Failed"<<
                              "SourceWords.size()"<<SourceWords.size()<<
                              "MetaInfo.SourceWordsPos.start()"<<MetaInfo.SourceWordsPos.start())
            }


            QVariantList TranslationOptions;
            bool IsFirstOption = true;
            foreach(const QString& Option, MetaInfo.Alternatives){
                TranslationOptions.insert(TranslationOptions.size(),
                                          QVariantList()<<
                                          TargomanTextProcessor::instance().ixml2Text(Option)<<
                                          IsFirstOption);
                if (IsFirstOption)
                    IsFirstOption = false;
            }

            QVariantList AlignInfo;
            AlignInfo.insert(0,SourcePhrase);
            AlignInfo.insert(1,AlignInfoMap.size());
            AlignInfo.insert(2,TranslationOptions);


            QVariantList CharAlignInfo;
            qint32 FirstChar = INT_MAX;
            qint32 LastChar  = 0;

            if (((size_t)TaggedWordsCharMap.size()) > MetaInfo.SourceWordsPos.start() &&
                ((size_t)TaggedWordsCharMap.size()) > MetaInfo.SourceWordsPos.end() -1){
                stuPos TaggedSourceCharRange(TaggedWordsCharMap.at(MetaInfo.SourceWordsPos.start()).start(),
                                             TaggedWordsCharMap.at(MetaInfo.SourceWordsPos.end() - 1).end());

                for (size_t i=TaggedSourceCharRange.start(); i < TaggedSourceCharRange.end(); ++i){
                    if (Tagged2OrigianlCharMap.at(i).isValid()){
                        FirstChar = qMin(Tagged2OrigianlCharMap.at(i).first, FirstChar);
                        LastChar = qMax(Tagged2OrigianlCharMap.at(i).second, LastChar);
                    }
                }
                while (_intermediate.OriginalSource.size() > FirstChar &&
                       _intermediate.OriginalSource.at(FirstChar) == ' ') ++FirstChar;
                while (LastChar > 0 && _intermediate.OriginalSource.at(LastChar - 1) == ' ') --LastChar;
                CharAlignInfo.insert(CharAlignInfo.size(), QVariantList()<<FirstChar<<LastChar);
            }else{
                TargomanDebug(5,"TaggedSourceCharRange Failed"<<
                              "TaggedWordsCharMap.size()"<<TaggedWordsCharMap.size()<<
                              "MetaInfo.SourceWordsPos.start()"<<MetaInfo.SourceWordsPos.start()<<
                              "MetaInfo.SourceWordsPos.end() -1"<<MetaInfo.SourceWordsPos.end() -1
                              )
                CharAlignInfo.insert(CharAlignInfo.size(), QVariantList()<<0<<0);
            }

            AlignInfo.insert(3,CharAlignInfo);
            AlignInfo.insert(4,QVariantList()<<
                             (quint16)MetaInfo.SourceWordsPos.start()<<(quint16)MetaInfo.SourceWordsPos.end());

            AlignInfoMap.insert(MetaInfo.TargetWordsPos.start(), AlignInfo);
        }

        QVariantList PhraseIndexList;
        foreach(const QString& Phrase, PhraseIndexMap.values())
            PhraseIndexList.insert(PhraseIndexList.size(),
                                   QVariantList()<<
                                   Phrase<<
                                   PhraseIndexList.size());

        QVariantList AlignInfoList;
        foreach(const QVariantList& Info, AlignInfoMap.values()){
            QVariantList UpdatedInfo = Info;
            UpdatedInfo[1] = AlignInfoList.size();
            AlignInfoList.insert(AlignInfoList.size(),UpdatedInfo);
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
    if (_result.Translations.first().size()){
        _result.Translations[0].append(" ");
        Offset = _result.Translations.first().count(' ');
    }

    _result.Translations[0].append(_intermediate.Translations.first());

    foreach(stuTranslationOutput::stuPhraseAlternatives MetaInfo, _intermediate.BestTranslationPhraseAlternatives){
        MetaInfo.SourceWordsPos.first  += Offset;
        MetaInfo.SourceWordsPos.second += Offset;
        MetaInfo.TargetWordsPos.first  += Offset;
        MetaInfo.TargetWordsPos.second += Offset;
        _result.BestTranslationPhraseAlternatives.append(MetaInfo);
    }
}

}
}
