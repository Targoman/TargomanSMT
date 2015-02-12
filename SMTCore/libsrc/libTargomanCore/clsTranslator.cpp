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
#include "Private/FeatureFunctions/PhraseTable/PhraseTable.h"

namespace Targoman{
namespace Core {

using namespace Private;

clsTranslator::clsTranslator(const QString &_inputStr) :
    pPrivate(new Private::clsTranslatorPrivate(_inputStr))
{
}

clsTranslator::~clsTranslator()
{
    ///@note Just to suppress compiler error using QScoppedPointer
}

void clsTranslator::init(const stuTranslatorConfigs& _configs)
{
    Q_UNUSED(_configs)

    FeatureFunction::PhraseTable::instance();
    InputDecomposer::clsInput::init();
    SearchGraphBuilder::clsSearchGraphBuilder::init();
    NBestFinder::clsNBestFinder::init();
    OutputComposer::clsOutputComposer::init();
}

stuTranslationOutput clsTranslator::translate()
{
    if (!this->pPrivate->Initialized)
        throw exTargomanCore("Translator is not initialized");

    //Input was decomposed in constructor
    this->pPrivate->SGB->matchPhrase();
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
