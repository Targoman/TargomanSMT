/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include "libTargomanTextProcessor/TextProcessor.h"
#include "clsInput.h"

using Targoman::NLPLibs::TextProcessor;

namespace Targoman {
namespace Core {
namespace Private {
namespace Input {

clsInput::clsInput()
{

}

void clsInput::parsePlain(const QString &_inputStr, const QString& _lang)
{
    this->parseRichIXML(TextProcessor::instance().text2RichIXML(_inputStr, _lang));
}

void clsInput::parseRichIXML(const QString &_inputIXML)
{

}

}
}
}
}
