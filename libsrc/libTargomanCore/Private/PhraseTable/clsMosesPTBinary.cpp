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

#include "clsMosesPTBinary.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable {

clsMosesPTBinary::clsMosesPTBinary()
{
    this->init();
}

void clsMosesPTBinary::load()
{
    // find out which feature function can be applied in this decode step
    //PhraseDictionary::SetFeaturesToApply()
}

void clsMosesPTBinary::initializeForInput(const Input::clsInput &_source)
{
    //ReduceCache();

}

void clsMosesPTBinary::cleanUpAfterSentenceProcessing(const Input::clsInput &_source)
{

}

}
}
}
}
