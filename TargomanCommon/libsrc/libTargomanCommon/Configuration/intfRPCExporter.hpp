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

#ifndef TARGOMAN_COMMON_CONFIGURATION_INTFRPCEXPORTER_HPP
#define TARGOMAN_COMMON_CONFIGURATION_INTFRPCEXPORTER_HPP

#include <QObject>
#include <QVariantMap>
#include "libTargomanCommon/Types.h"

namespace Targoman {
namespace Common {
namespace Configuration {

struct stuRPCOutput{
    QVariant    DirectResult;
    QVariantMap IndirectResult;

    stuRPCOutput(const QVariant& _direct = QVariant(),
                 const QVariantMap& _indirect = QVariantMap()) :
        DirectResult(_direct),
        IndirectResult(_indirect)
    {}

    stuRPCOutput(quint8 _errCode, const QString& _errString):
        DirectResult(_errCode){
        IndirectResult.insert("err",_errString);
    }
};

class intfRPCExporter : public QObject{
    Q_OBJECT
public:
    intfRPCExporter(QObject *_parent = NULL) :
        QObject(_parent)
    {}

    /**
     * @brief exportMyRPCs will detect and export acceptable method to RPC registry.
     * Acceptable methods must have following conditions:
     *  1- their name must start with rpc or asyncRPC keyword
     *  2- must be defined as an slot
     *  3- their signature must be
     *        stuRPCOutput RPC_NAME(QVariantMap _input)
     * This method must be called in subclasses constructor
     */
    void exportMyRPCs();
};

}
}
}
Q_DECLARE_METATYPE(Targoman::Common::Configuration::stuRPCOutput)


#endif // TARGOMAN_COMMON_CONFIGURATION_INTFRPCEXPORTER_HPP
