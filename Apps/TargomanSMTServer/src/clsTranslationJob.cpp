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


namespace Targoman {
namespace Apps {

using namespace SMT;
using namespace NLPLibs;
using namespace Common::Configuration;

clsTranslationJob::clsTranslationJob(bool _brief, bool _keepAsSource)
{
    this->Brief = _brief;
    this->KeepAsSource = _keepAsSource;
}

QVariantList clsTranslationJob::doJob(const QString &_inputStr)
{
        QVariantList Result = QtConcurrent::blockingMappedReduced<QVariantList>(
            _inputStr.split("\n"),
            std::bind(&clsTranslationJob::mapLineTranslation,this, std::placeholders::_1),
            std::bind(&clsTranslationJob::reduceLineTranslation,this, std::placeholders::_1, std::placeholders::_2),
            QtConcurrent::OrderedReduce);

    return Result;
}

stuTranslationOutput clsTranslationJob::mapLineTranslation(const QString &_line)
{
    if (this->KeepAsSource)
        return Translator::translate(_line, this->Brief);

    QStringList Sentences = TargomanTextProcessor::instance().text2IXML(
                _line,
                ConfigManager::instance().getConfig("/Common/Language/Source").toString()).replace(
                " . ", " .\n").split("\n");

    stuTranslationOutput Result = QtConcurrent::blockingMappedReduced<stuTranslationOutput>(
                Sentences,
                std::bind(&clsTranslationJob::mapSentenceTranslation,this, std::placeholders::_1),
                std::bind(&clsTranslationJob::reduceSentenceTranslation,this, std::placeholders::_1, std::placeholders::_2),
                QtConcurrent::OrderedReduce);
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
        _result.insert(1,QVariantList());//IndexSection
        _result.insert(2,QVariantList());//AlignmentSection
    }

    QVariantList TranslationSection = _result[0].toList();
    TranslationSection.insert(TranslationSection.size(),
                QVariantList()<<_intermediate.NormalizedSource<<_intermediate.Translation);
    _result[0] = TranslationSection;

/*    QVariantList IndexSection = _result[1].toList();
    IndexSection.insert(IndexSection.size(),
                QVariantList()<<_intermediate.MetaInfo.<<_intermediate.Translation);
    _result[1] = IndexSection;*/

//    QVariantList& AlignmentSection = _result[2];

    return;
}

void clsTranslationJob::reduceSentenceTranslation(stuTranslationOutput &_result,
                                                  const stuTranslationOutput &_intermediate)
{
    _result.NormalizedSource.append(" ");
    _result.NormalizedSource.append(_intermediate.NormalizedSource);
    _result.Translation.append(" ");
    _result.Translation.append(_intermediate.Translation);
    _result.MetaInfo.append(_intermediate.MetaInfo);
}

}
}
