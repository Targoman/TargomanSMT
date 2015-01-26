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

#ifndef TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHBUILDER_H
#define TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHBUILDER_H

#include "Private/PhraseTable/clsRuleNode.h"
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
    //QsharedDataArray
    QList<QVector<PhraseTable::clsRuleNode*> > PhraseMatchTable; //TODO convert to cacheable
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_SEARCHGRAPHBUILDER_CLSSEARCHGRAPHBUILDER_H
