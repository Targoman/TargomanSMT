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

#include "clsJanePlainRuleTable.h"
#include "libTargomanCommon/Logger.h"
#include "libTargomanCommon/Configuration/Validators.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace RuleTable {

using namespace Common;
using namespace Common::Configuration;
TARGOMAN_REGISTER_MODULE(clsJanePlainRuleTable)

tmplConfigurable<QString> clsJanePlainRuleTable::FileName(clsJanePlainRuleTable::baseConfigPath() + "/FileName",
                                                  "Filename where phrase table is stored",
                                                  ""/*,
                                                  new Validators::clsPathValidator(PathAccess::File|PathAccess::Readable)*/);


clsJanePlainRuleTable::clsJanePlainRuleTable():
    intfRuleTable(clsJanePlainRuleTable::moduleName())
{
}

clsJanePlainRuleTable::~clsJanePlainRuleTable()
{
    //Just to suppress compiler error when using QScoppedPointer
}

void clsJanePlainRuleTable::init()
{
    TargomanLogInfo(5, "Loading plain text rule set from: " + this->FileName.value());

    this->PrefixTree.reset(new RulesPrefixTree_t());
}

void clsJanePlainRuleTable::partialSortRuleTreesWithLM(const std::vector<double> &_scalingFactors,
                                                unsigned _rulesCostsBegin,
                                                size_t _observationHistogramSize)
{
//TODO
    Q_UNUSED(_scalingFactors)
    Q_UNUSED(_rulesCostsBegin)
    Q_UNUSED(_observationHistogramSize)

}

}
}
}
}
