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

#ifndef TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSMOSESPDTA_H
#define TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSMOSESPDTA_H

#include "intfPhraseTable.hpp"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable {

//Reimplementation of PhraseDictionaryTreeAdaptor and PDTAimp of moses
class clsMosesPTBinary : public intfPhraseTable
{
public:
    clsMosesPTBinary();

    void load(); // @todo Maybe must be removed as it was implemented in FF
    void initializeForInput(Input::clsInput const& _source);
    void cleanUpAfterSentenceProcessing(Input::clsInput const& _source);

    /*static void registerConfigs(const QString& _configPath){
        intfPhraseTable::registerConfigs(_configPath + "/MosesPTBinary");
    }*/

};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSMOSESPDTA_H
