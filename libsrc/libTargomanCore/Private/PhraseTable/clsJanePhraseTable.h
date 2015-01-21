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

#ifndef TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSJANEPHRASETABLE_H
#define TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSJANEPHRASETABLE_H

#include "intfPhraseTable.hpp"
#include "libTargomanCommon/Configuration.h"
#include "libTargomanCommon/PrefixTree/tmplFullVectorFilePrefixTree.hpp"
#include "clsRuleNode.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable {

TARGOMAN_ADD_EXCEPTION_HANDLER(exJanePhraseTable, exPhraseTable);

class clsJanePhraseTable : intfPhraseTable
{
public:
    clsJanePhraseTable();

    virtual void init();

    /**
     * @brief newSentence is empty because it is not important for us that we have a new sentence
     */
    void newSentence() {}
    bool isBinary() { return false; }

    void partialSortRuleTreesWithLM(const std::vector<double> &_scalingFactors,
                                    unsigned _rulesCostsBegin,
                                    size_t _observationHistogramSize);

    static QString baseConfigPath(){return intfPhraseTable::baseConfigPath() + "/JainePlain";}

protected:
     static Targoman::Common::clsConfigurable<QString> FileName;
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSJANEPHRASETABLE_H
