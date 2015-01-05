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
    //ReduceCache(); // disabled as we are using our own cache implementation

    /*vector<float> weight = staticData.GetWeights(this);
    if(m_numScoreComponents!=weight.size()) {
      std::stringstream strme;
      UTIL_THROW2("ERROR: mismatch of number of scaling factors: " << weight.size()
                  << " " << m_numScoreComponents);
    }*/ //Temporarily ignored as seems to be unused



}

void clsMosesPTBinary::cleanUpAfterSentenceProcessing(const Input::clsInput &_source)
{

}

}
}
}
}
