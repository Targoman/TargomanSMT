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

#ifndef TARGOMAN_CORE_PRIVATE_OUTPUTCOMPOSER_CLSOUTPUTCOMPOSER_H
#define TARGOMAN_CORE_PRIVATE_OUTPUTCOMPOSER_CLSOUTPUTCOMPOSER_H

#include "Private/N-BestFinder/clsNBestFinder.h"
#include "Private/SearchGraph/clsSearchGraphNode.h"
#include "libTargomanSMT/Types.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace OutputComposer {

class clsOutputComposer
{
public:
    clsOutputComposer(InputDecomposer::clsInputDecomposer& _inputDecomposer,
                      NBestFinder::clsNBestFinder& _nBestFinder):
        InputDecomposerRef(_inputDecomposer),
        NBestFinderRef(_nBestFinder)
    {}

public:
    stuTranslationOutput translationOutput();
    QString translationString();

private:
    QString getTargetString(const RuleTable::clsTargetRule& _target, const stuPhrasePos& _sourcePos);
    QString nodeTranslation(const SearchGraph::clsSearchGraphNode& _node);

private:
    InputDecomposer::clsInputDecomposer&  InputDecomposerRef;
    NBestFinder::clsNBestFinder&          NBestFinderRef;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_OUTPUTCOMPOSER_CLSOUTPUTCOMPOSER_H
