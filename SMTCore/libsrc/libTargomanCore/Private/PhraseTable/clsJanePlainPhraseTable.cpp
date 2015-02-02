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

#include "clsJanePlainPhraseTable.h"
#include "libTargomanCommon/Logger.h"
#include "libTargomanCommon/Configuration/Validators.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable {

using namespace Common;
using namespace Common::Configuration;

tmplConfigurable<QString> clsJanePlainPhraseTable::FileName(clsJanePlainPhraseTable::baseConfigPath() + "/FileName",
                                                  "Filename where phrase table is stored",
                                                  "",
                                                  new Validators::clsPathValidator(PathAccess::File|PathAccess::Readable));


clsJanePlainPhraseTable::clsJanePlainPhraseTable():
    intfPhraseTable(clsJanePlainPhraseTable::moduleName())
{
}

clsJanePlainPhraseTable::~clsJanePlainPhraseTable()
{
    //Just to suppress compiler error when using QScoppedPointer
}

void clsJanePlainPhraseTable::init()
{
    TargomanLogInfo(5, "Loading plain text rule set from: " + this->FileName.value());

    this->PrefixTree.reset(new RulesPrefixTree_t());

  /*  Core::CompressedInputStream rulesFp(fname_);

    size_t rulesRead = 0;
    Core::ProgressIndicator readProgressIndicator("Reading phrases", "rules");
    if (verbosity_ > Core::verbosityLevelNoLog)
        readProgressIndicator.start();
    while (rulesFp.peek() >= 0) {
        std::string line;
        getline(rulesFp, line);
        if (line == "")
            continue;
        ++rulesRead;
        std::vector<std::string> fields(Core::split(line, "#"));

        if (fields.size() < 3) {
            warning() << "bad file format in line " << rulesRead << ": " << line;
            continue;
        }

        //! \todo Include alignment information
        addRule_(fields, whichCosts_, rulesRead);

        if (verbosity_ > Core::verbosityLevelNoLog)
            readProgressIndicator.notify(rulesRead);
    }
    // Activate for memory optimization
    //rulePrefixTree_.compact();
    if (verbosity_ > Core::verbosityLevelNoLog) {
        readProgressIndicator.finish();
        log() << rulesRead << " rules read" << std::endl;

        if (verbosity_ >= Core::verbosityLevelAdditionalLog)
            log() << "Maximum number of non-terminals in a rule: " << maxNumberOfNonTerminals_ << std::endl;
    }*/

    //Also load target vocab in  a Qlist
}

void clsJanePlainPhraseTable::partialSortRuleTreesWithLM(const std::vector<double> &_scalingFactors,
                                                unsigned _rulesCostsBegin,
                                                size_t _observationHistogramSize)
{

}

}
}
}
}
