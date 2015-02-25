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

#include "clsTranslator.h"

#include "Private/clsTranslator_p.h"
#include "libTargomanTextProcessor/TextProcessor.h"

namespace Targoman{
namespace Core {

using namespace Private;
using namespace NLPLibs;

bool clsTranslatorPrivate::Initialized = false;

clsTranslator::clsTranslator(const QString &_inputStr) :
    pPrivate(new Private::clsTranslatorPrivate(_inputStr))
{
    TargomanDebug(5,_inputStr);
}

clsTranslator::~clsTranslator()
{
    ///@note Just to suppress compiler error using QScoppedPointer
}

void clsTranslator::init(const QString _configFilePath)
{
    TextProcessor::instance().init(_configFilePath);
    InputDecomposer::clsInput::init();
    gConfigs.EmptyLMScorer.reset(gConfigs.LM.getInstance<LanguageModel::intfLMSentenceScorer>());
    //Load vocab by LM
    gConfigs.EmptyLMScorer->init(false);

    SearchGraphBuilder::clsSearchGraphBuilder::init(_configFilePath);
    //continue to load rest of LM
    //gConfigs.EmptyLMScorer->init(false);

    NBestFinder::clsNBestFinder::init();
    OutputComposer::clsOutputComposer::init();

    clsTranslatorPrivate::Initialized = true;
}

stuTranslationOutput clsTranslator::translate()
{
    if (clsTranslatorPrivate::Initialized == false)
        throw exTargomanCore("Translator is not initialized");

    //Input was decomposed in constructor
    this->pPrivate->SGB->matchPhrase();
    this->pPrivate->SGB->parseSentence();

    stuTranslationOutput Output;
    // find NBest
    // Output composer
    // IO -Alignment
    // Postprocess

    return Output;
}
}
}
