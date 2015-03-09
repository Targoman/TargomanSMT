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

#include "clsTranslationJob.h"
#include "libTargomanSMT/clsTranslator.h"
#include "TranslationWriter.h"

namespace Targoman {
namespace Apps {

using namespace SMT;

clsTranslationJob::clsTranslationJob(quint64 _index, const QString& _sourceString)
{
    this->SourceString = _sourceString;
    this->Index = _index;
}

void clsTranslationJob::run()
{
    clsTranslator Translator(this->SourceString);
    TranslationWriter::instance().writeTranslation(
                this->Index,
                Translator.translate(true).Translation);
}

}
}
