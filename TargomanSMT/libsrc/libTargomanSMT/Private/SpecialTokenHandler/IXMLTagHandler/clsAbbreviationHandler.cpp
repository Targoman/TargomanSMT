#include "clsAbbreviationHandler.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace IXMLTagHandler {

TARGOMAN_REGISTER_MODULE(clsAbbreviationHandler);
using namespace RuleTable;


TargetRulesContainer_t clsAbbreviationHandler::getTargetRules(const QString& _token)
{
    Q_UNUSED(_token);
    QList<WordIndex_t> TargetPhrase;
    TargetPhrase.append(gConfigs.EmptyLMScorer->getWordIndex(_token));
    return QList<RuleTable::clsTargetRule>() << clsTargetRule::createMinCostTargetRule(TargetPhrase, false);
}

QString clsAbbreviationHandler::getDefaultTranslation(const QString& _token)
{
    Q_UNUSED(_token);
    return QString();
}

}
}
}
}
}


