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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#include "Translator.h"

#include "libTargomanTextProcessor/TextProcessor.h"
#include "Private/InputDecomposer/clsInput.h"
#include "Private/SearchGraphBuilder/clsSearchGraph.h"
#include "Private/OutputComposer/clsOutputComposer.h"
#include "Private/OOVHandler/OOVHandler.h"

namespace Targoman{
/**
 * @brief Base namespace of TargomanSMT library surrounding all other namespaces
 */
namespace SMT {

using namespace Private;
using namespace NLPLibs;
using namespace Private::OOV;
using namespace SearchGraphBuilder;

static bool TranslatorInitialized = false;

void Translator::init(const QString _configFilePath)
{
    if (TranslatorInitialized){
        TargomanWarn(5, "Reinitialization of translator has no effect");
        return;
    }

    InputDecomposer::clsInput::init(_configFilePath);
    gConfigs.EmptyLMScorer.reset(gConfigs.LM.getInstance<Proxies::intfLMSentenceScorer>());
    gConfigs.EmptyLMScorer->init(false);

    SearchGraphBuilder::clsSearchGraph::init(_configFilePath);
    OOVHandler::instance().initialize();

    TranslatorInitialized = true;
}

stuTranslationOutput Translator::translate(const QString &_inputStr, bool _justTranslationString)
{
    if (TranslatorInitialized == false)
        throw exTargomanCore("Translator is not initialized");

    InputDecomposer::clsInput Input(_inputStr);
    SearchGraphBuilder::clsSearchGraph  SearchGraph(Input.tokens());
    OutputComposer::clsOutputComposer   OutputComposer(Input, SearchGraph);

    if (_justTranslationString){
        stuTranslationOutput Output;
        Output.Translation = OutputComposer.translationString();
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
