#ifndef TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_CLSSpecialNumberHANDLER_H
#define TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_CLSSpecialNumberHANDLER_H
#include "intfIXMLTagHandlerModule.hpp"

namespace Targoman{
namespace SMT {
namespace Private{
namespace SpecialTokenHandler {
namespace IXMLTagHandler {

class clsSpecialNumberHandler : public intfIXMLTagHandlerModule
{
public:
    clsSpecialNumberHandler(quint64 _instanceID): intfIXMLTagHandlerModule(this->tagName(), this->moduleName(), _instanceID)
    { }
    ~clsSpecialNumberHandler();
    TargetRulesContainer_t getTargetRules(const QString& _token);
    QString getDefaultTranslation(const QString& _token);
private:
    TARGOMAN_DEFINE_TAG_HANDLER_MODULE(Targoman::NLPLibs::enuTextTags::Type::SpecialNumber, "clsSpecialNumberHandler", clsSpecialNumberHandler);

};


}
}
}
}
}

#endif // TARGOMAN_SMT_PRIVATE_SPECIAL_TOKEN_HANDLER_CLSSpecialNumberHANDLER_H
