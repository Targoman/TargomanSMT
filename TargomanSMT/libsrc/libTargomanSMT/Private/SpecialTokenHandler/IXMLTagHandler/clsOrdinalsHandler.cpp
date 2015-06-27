#include "clsOrdinalsHandler.h"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace IXMLTagHandler {

TARGOMAN_REGISTER_MODULE(clsOrdinalsHandler);
using namespace RuleTable;


TargetRulesContainer_t clsOrdinalsHandler::getTargetRules(const QString& _token)
{
    Q_UNUSED(_token);
    QList<WordIndex_t> TargetPhrase;
    TargetPhrase.append(gConfigs.EmptyLMScorer->getWordIndex(_token));
    return QList<RuleTable::clsTargetRule>() << clsTargetRule::createMinCostTargetRule(TargetPhrase, false);
}

QString clsOrdinalsHandler::getDefaultTranslation(const QString& _token)
{
    Q_UNUSED(_token);
    return QString();
}

}
}
}
}
}


