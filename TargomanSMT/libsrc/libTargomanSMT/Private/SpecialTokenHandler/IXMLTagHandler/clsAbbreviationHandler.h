#ifndef TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_CLSABBREVIATIONHANDLER_H
#define TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_CLSABBREVIATIONHANDLER_H
#include "intfIXMLTagHandlerModule.hpp"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace IXMLTagHandler {

class clsAbbreviationHandler : public intfIXMLTagHandlerModule
{
public:
    clsAbbreviationHandler(quint64 _instanceID): intfIXMLTagHandlerModule(this->tagName(), this->moduleName(), _instanceID)
    { }
    ~clsAbbreviationHandler();
    TargetRulesContainer_t getTargetRules(const QString& _token);
    QString getDefaultTranslation(const QString& _token);
private:
    TARGOMAN_DEFINE_TAG_HANDLER_MODULE(Targoman::NLPLibs::enuTextTags::Type::Abbreviation, "clsAbbreviationHandler", clsAbbreviationHandler);

};


}
}
}
}
}

#endif // TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_CLSABBREVIATIONHANDLER_H
