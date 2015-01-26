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

#include "clsTranslator.h"

#include "Private/clsTranslator_p.h"

namespace Targoman{
namespace Core {

using namespace Private;

clsTranslator::clsTranslator() :
    pPrivate(new Private::clsTranslatorPrivate)
{
}

clsTranslator::~clsTranslator()
{
    ///@note Just to suppress compiler error using QScoppedPointer
}

void clsTranslator::init(const stuTranslatorConfigs& _configs)
{
    Q_UNUSED(_configs)
    InputDecomposer::clsInput::init();
    SearchGraphBuilder::clsSearchGraphBuilder::init();
    NBestFinder::clsNBestFinder::init();
    OutputComposer::clsOutputComposer::init();
}

stuTranslationOutput clsTranslator::translate(const QString &_inputStr)
{
    if (!this->pPrivate->Initialized)
        throw exTargomanCore("Translator is not initialized");

    this->pPrivate->Input->parsePlain(_inputStr, gConfigs.SourceLanguage.value());
    this->pPrivate->SGB->matchPhrase(this->pPrivate->Input->tokens());
    this->pPrivate->SGB->parseSentence();

    stuTranslationOutput Output;
    // Build Search graph
    // find NBest
    // Output composer
    // IO -Alignment
    // Postprocess

    return Output;
}
}
}
