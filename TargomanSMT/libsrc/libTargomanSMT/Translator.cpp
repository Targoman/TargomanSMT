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
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include "Translator.h"

#include "libTargomanTextProcessor/TextProcessor.h"
#include "Private/InputDecomposer/clsInput.h"
#include "Private/SearchGraphBuilder/clsSearchGraph.h"
#include "Private/OutputComposer/clsOutputComposer.h"
#include "Private/SpecialTokenHandler/OOVHandler/OOVHandler.h"
#include "Private/SpecialTokenHandler/IXMLTagHandler/IXMLTagHandler.h"
// TODO: This header must be included in OOVHandler module
#include "Private/Proxies/Transliteration/intfTransliterator.h"
#include "Private/Proxies/NamedEntityRecognition/intfNamedEntityRecognizer.h"
#include "Private/N-BestFinder/NBestPath.h"
#include<iostream>

namespace Targoman{
/**
 * @brief Base namespace of TargomanSMT library surrounding all other namespaces
 */
namespace SMT {

using namespace Private;
using namespace NLPLibs;
using namespace Private::SpecialTokenHandler::OOV;
using namespace Private::SpecialTokenHandler::IXMLTagHandler;
using namespace SearchGraphBuilder;


static bool TranslatorInitialized = false;

void Translator::init(QSharedPointer<QSettings> _configSettings)
{
    if (TranslatorInitialized){
        TargomanWarn(5, "Reinitialization of translator has no effect");
        return;
    }

    InputDecomposer::clsInput::init(_configSettings);
    gConfigs.EmptyLMScorer.reset(gConfigs.LM.getInstance<Proxies::LanguageModel::intfLMSentenceScorer>());
    gConfigs.EmptyLMScorer->init(false);

    OOVHandler::instance().initialize();
    IXMLTagHandler::instance().initialize();
    SearchGraphBuilder::clsSearchGraph::init(_configSettings);

    TranslatorInitialized = true;
    TargomanLogHappy(5, "Translator Initialized successfully");
}

stuTranslationOutput Translator::translate(const QString &_inputStr,
                                           bool _justTranslationString, bool _getNBest,
                                           bool _isIXML)
{
    if (TranslatorInitialized == false)
        throw exTargomanCore("Translator is not initialized");

    QTime start = QTime::currentTime();

    InputDecomposer::clsInput Input(_inputStr, _isIXML);
    SearchGraphBuilder::clsSearchGraph  SearchGraph(Input.tokens());
    OutputComposer::clsOutputComposer   OutputComposer(Input, SearchGraph);

    stuTranslationOutput Output;
    if (_justTranslationString){
        Output.Translation = OutputComposer.translationString();
    }else{
        Output = OutputComposer.translationOutput();
    }
    int Elapsed = start.elapsed();
#ifndef SMT
    TargomanLogInfo(7, "Translation [" << Elapsed / 1000.0 << "s]"<<
                     _inputStr << " => " << Output.Translation);
#else
    QString InputWord = _inputStr;
    TargomanLogInfo(7, "Transliteration [" << Elapsed / 1000.0 << "s]" <<
                     InputWord.replace(" ", "") << " => " << Output.Translation.replace(" ", ""));
#endif

    if(!_getNBest)
        return Output;

    NBestFinder::NBestPath::Container_t Storage;
    start = QTime::currentTime();
    NBestFinder::NBestPath::retrieveNBestPaths(Storage, SearchGraph, OutputComposer);

    for(int i = 0; i < Storage.size(); i++){
        Output.NBestTranslations.append(Storage[i].printPath(OutputComposer));
    }

    Elapsed = start.elapsed();
    TargomanLogInfo(9, "NBest Creation [" << Elapsed / 1000.0 << "s]"<<
                     _inputStr);
    return Output;
}

//void Translator::printNBestPath(const QString &_inputStr, int _sentenceNum, bool _isIXML)
//{
//    if (TranslatorInitialized == false)
//        throw exTargomanCore("Translator is not initialized");

//    if(NBestFinder::NBestPath::NBestFile.toVariant().toString().isEmpty())
//        return;

//    InputDecomposer::clsInput Input(_inputStr, _isIXML);
//    SearchGraphBuilder::clsSearchGraph  SearchGraph(Input.tokens());

//    OutputComposer::clsOutputComposer   OutputComposer(Input, SearchGraph);

//    QFile OutFile(NBestFinder::NBestPath::NBestFile.value());
//    if (OutFile.open(QFile::Append), OutFile.isWritable() == false)
//        throw exTargomanCore("Unable to open: "+ OutFile.fileName() + " for Writing");

//    QTextStream Stream(&OutFile);

//    QTime start = QTime::currentTime();
//    Stream.setCodec("UTF-8");
//    NBestFinder::NBestPath::Container_t Storage;

//    NBestFinder::NBestPath::retrieveNBestPaths(Storage, SearchGraph, OutputComposer);

//    for(int i = 0; i < Storage.size(); i++){
//        Stream<< QString::number(_sentenceNum - 1) << " ||| " << Storage[i].printPath(OutputComposer) <<"\n";
//    }

//    int Elapsed = start.elapsed();
//    TargomanLogInfo(1, "NBest Creation [" << Elapsed / 1000.0 << "s]"<<
//                     _inputStr);

//}

void Translator::saveBinaryRuleTable(const QString &_filePath)
{
    if (TranslatorInitialized == false)
        throw exTargomanCore("Translator is not initialized");
    clsSearchGraph::saveBinaryRuleTable(_filePath);
}

void Translator::showWeights()
{

    QString res = "";

    for(auto ffIter = gConfigs.ActiveFeatureFunctions.constBegin(); ffIter != gConfigs.ActiveFeatureFunctions.constEnd(); ffIter++){

        QStringList ffName = ffIter.key().split("/");
        QStringList colName = ffIter.value()->columnNames();
        QList<double> scalingFactors = ffIter.value()->getScalingFactors();
        for(int j = 0; j < scalingFactors.size(); j++){
            if(colName.size() > j) res += ffName.back() + "/" + colName[j];
            else res += ffName.back();
            res += "= " + QString::number(scalingFactors[j]) + "\n";

        }
    }
    //TargomanInlineOut(1, res);
    std::cout << res.toStdString();
}

}
}
