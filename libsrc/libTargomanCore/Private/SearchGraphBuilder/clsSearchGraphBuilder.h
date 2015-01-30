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

#ifndef TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHBUILDER_H
#define TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHBUILDER_H

#include "Private/PhraseTable/intfPhraseTable.hpp"
#include "Private/InputDecomposer/clsInput.h"

namespace Targoman{
namespace Core {
namespace Private{
namespace SearchGraphBuilder {

class clsSearchGraphBuilder
{
public:
    clsSearchGraphBuilder();

    static void init();
    void matchPhrase(const QList<InputDecomposer::clsToken*> &_input);
    void parseSentence();

private:
    //TODO maybe QsharedDataArray
    QList<QVector<const PhraseTable::clsRuleNode*>> PhraseMatchTable; //TODO convert to cacheable

    static PhraseTable::intfPhraseTable& PhraseTable;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHBUILDER_H
