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
    TargomanLogHappy(5, "Translator Initialize successfully");
}

stuTranslationOutput Translator::translate(const QString &_inputStr,
                                           bool _justTranslationString,
                                           bool _isIXML)
{
    if (TranslatorInitialized == false)
        throw exTargomanCore("Translator is not initialized");

    QTime start = QTime::currentTime();

    InputDecomposer::clsInput Input(_inputStr, _isIXML);
    SearchGraphBuilder::clsSearchGraph  SearchGraph(Input.tokens());
    OutputComposer::clsOutputComposer   OutputComposer(Input, SearchGraph);

    if (_justTranslationString){
        stuTranslationOutput Output;
        Output.Translation = OutputComposer.translationString();
        int Elapsed = start.elapsed();
#ifndef SMT
        TargomanLogDebug(5, "Translation [" << Elapsed / 1000.0 << "s]"<<
                         _inputStr << " => " << Output.Translation);
#else
        QString InputWord = _inputStr;
        TargomanLogDebug(5, "Transliteration [" << Elapsed / 1000.0 << "s]" <<
                         InputWord.replace(" ", "") << " => " << Output.Translation.replace(" ", ""));
#endif
        return Output;
    }else
        return OutputComposer.translationOutput();
}

void Translator::saveBinaryRuleTable(const QString &_filePath)
{
    if (TranslatorInitialized == false)
        throw exTargomanCore("Translator is not initialized");
    clsSearchGraph::saveBinaryRuleTable(_filePath);
}

}
}
