#include "clsEmailHandler.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace IXMLTagHandler {

TARGOMAN_REGISTER_MODULE(clsEmailHandler);
using namespace RuleTable;


TargetRulesContainer_t clsEmailHandler::getTargetRules(const QString& _token)
{
    Q_UNUSED(_token);
    QList<WordIndex_t> TargetPhrase;
    TargetPhrase.append(gConfigs.EmptyLMScorer->getWordIndex(_token));
    return QList<RuleTable::clsTargetRule>() << clsTargetRule::createMinCostTargetRule(TargetPhrase, false);
}

QString clsEmailHandler::getDefaultTranslation(const QString& _token)
{
    Q_UNUSED(_token);
    return QString();
}

}
}
}
}
}


