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

#include "Configs.h"

namespace Targoman {
namespace Apps {

using namespace Common;
using namespace Common::Configuration;

tmplConfigurable<QString>     gConfigs::InputFile(
        gConfigs::appConfig("InputFile"),
        "Input file path to convert",
        "",
        Validators::tmplPathAccessValidator<
            (enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Readable),
            false>,
        "f",
        "FILE_PATH",
        "input-file");

tmplConfigurable<QString>     gConfigs::InputText(
        gConfigs::appConfig("InputText"),
        "Input Text to translate",
        "",
        ReturnTrueCrossValidator,
        "i",
        "TEXT",
        "input-text");

tmplConfigurable<QString>     gConfigs::OutputFile(
        gConfigs::appConfig("Output"),
        "output path to write translation",
        "",
        Validators::tmplPathAccessValidator<
            (enuPathAccess::Type)(enuPathAccess::File | enuPathAccess::Writeatble),
            false>,
        "o",
        "FILE_PATH",
        "output-file");

tmplConfigurable<quint8>     gConfigs::MaxThreads(
        gConfigs::appConfig("MaxThreads"),
        "Maximum Concurrent Translations",
        5,
        Validators::tmplNumericValidator<quint8, 0, 64>,
        "t",
        "MAX_THREADS",
        "max-threads");

}
}
