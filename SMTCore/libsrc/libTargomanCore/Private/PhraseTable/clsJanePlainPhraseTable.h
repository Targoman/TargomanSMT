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

#ifndef TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSJANEPHRASETABLE_H
#define TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSJANEPHRASETABLE_H

#include "intfPhraseTable.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/PrefixTree/tmplFullVectorFilePrefixTree.hpp"
#include "clsRuleNode.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable {

TARGOMAN_ADD_EXCEPTION_HANDLER(exJanePhraseTable, exPhraseTable);

class clsJanePlainPhraseTable : intfPhraseTable
{
public:
    clsJanePlainPhraseTable();
    ~clsJanePlainPhraseTable();

    virtual void init();

    /**
     * @brief newSentence is empty because it is not important for us that we have a new sentence
     */
    void newSentence() {}
    bool isBinary() { return false; }

    void partialSortRuleTreesWithLM(const std::vector<double> &_scalingFactors,
                                    unsigned _rulesCostsBegin,
                                    size_t _observationHistogramSize);

private:
    static Targoman::Common::Configuration::tmplConfigurable<QString> FileName;
    TARGOMAN_DEFINE_MODULE("JanePTPlain", clsJanePlainPhraseTable)
};

TARGOMAN_REGISTER_MODULE(clsJanePlainPhraseTable)
}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSJANEPHRASETABLE_H
