#ifndef TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_CLSURLHANDLER_H
#define TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_CLSURLHANDLER_H
#include "intfIXMLTagHandlerModule.hpp"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace IXMLTagHandler {

class clsURLHandler : public intfIXMLTagHandlerModule
{
public:
    clsURLHandler(quint64 _instanceID): intfIXMLTagHandlerModule(this->tagName(), this->moduleName(), _instanceID)
    { }
    ~clsURLHandler();
    TargetRulesContainer_t getTargetRules(const QString& _token);
    QString getDefaultTranslation(const QString& _token);
private:
    TARGOMAN_DEFINE_TAG_HANDLER_MODULE(Targoman::NLPLibs::enuTextTags::Type::URL, "clsURLHandler", clsURLHandler);

};


}
}
}
}
}

#endif // TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_CLSURLHANDLER_H
