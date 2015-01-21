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

#ifndef TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSJANEPTPLAIN_H
#define TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSJANEPTPLAIN_H

#include "intfPhraseTable.hpp"
#include "libTargomanCommon/Configuration.h"
#include "libTargomanCommon/PrefixTree/tmplFullVectorFilePrefixTree.hpp"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable {

TARGOMAN_ADD_EXCEPTION_HANDLER(exJanePTPlain, exPhraseTable);

typedef Common::PrefixTree::GFullVectorFilePrefixTree<RulesForSourcePart> RulePrefixTree;

class clsJanePTPlain : intfPhraseTable
{
public:
    clsJanePTPlain(/*const Core::Configuration &config,
                   StaticAlphabetRef sourceAlphabet,
                   StaticAlphabetRef targetAlphabet,
                   bool needCostsNames=true*/);

    virtual void init();

    /*! \brief empty function
     *  \note it is not important for us that we have a new sentence
     */
    void newSentence() {}
    bool isBinary() { return false; }

    void partialSortRuleTreesWithLM(const std::vector<double> &_scalingFactors,
                                    unsigned _rulesCostsBegin,
                                    size_t _observationHistogramSize);
    //virtual std::string getConfigText() const;
    static QString baseConfigPath(){return intfPhraseTable::baseConfigPath() + "/JainPlain";}

protected:
     static Targoman::Common::clsConfigurable<QString> FileName;

    //bool    warnedAboutFewCounts_
    //TODO there were some other methods not imported including
};

}
}
}
}
#endif // TARGOMAN_CORE_PRIVATE_PHRASETABLE_CLSJANEPTPLAIN_H
