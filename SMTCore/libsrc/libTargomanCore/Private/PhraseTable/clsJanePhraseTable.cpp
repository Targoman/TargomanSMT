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

#include "clsJanePhraseTable.h"
#include "libTargomanCommon/Logger.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable {

using namespace Common;

clsConfigurable<QString> clsJanePhraseTable::FileName(clsJanePhraseTable::baseConfigPath() + "/FileName",
                                                  "Filename where phrase table is stored",
                                                  "",
                                                  new Validators::clsPathValidator(PathAccess::File|PathAccess::Readable));


clsJanePhraseTable::clsJanePhraseTable(
        /*const Core::Configuration &config,
         *  StaticAlphabetRef sourceAlphabet,
         *  StaticAlphabetRef targetAlphabet,
         *  bool needCostsNames*/)
{
    TARGOMAN_REGISTER_ACTOR("JanePTPlain")
}

void clsJanePhraseTable::init()
{
    TargomanLogInfo(5, "Loading plain text rule set from: " + this->FileName.value());

    //TODO complete me
    //if (this->RulePrefixTree)
      //throw
    //    delete this->RulePrefixTree;
    //this->RulesPrefixTree = new RulePrefixTree;

}

void clsJanePhraseTable::partialSortRuleTreesWithLM(const std::vector<double> &_scalingFactors,
                                                unsigned _rulesCostsBegin,
                                                size_t _observationHistogramSize)
{

}

}
}
}
}
