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

#ifndef TARGOMAN_APPS_APPE4SMT_H
#define TARGOMAN_APPS_APPE4SMT_H

#include "libTargomanCommon/clsSafeCoreApplication.h"
#include "libTargomanCommon/CmdIO.h"
#include "libTargomanCommon/Configuration/intfRPCExporter.hpp"
#include "libTargomanCommon/JSONConversationProtocol.h"

namespace Targoman {
namespace Apps {


class appE4SMT : public Common::Configuration::intfRPCExporter
{
    Q_OBJECT
public:
    appE4SMT(){
        this->exportMyRPCs();
    }


public slots:
    void slotExecute();

private slots:
    void slotValidateAgent(INOUT QString&        _user,
                          const QString&        _pass,
                          const QString&        _ip,
                          OUTPUT bool&          _canView,
                          OUTPUT bool&          _canChange);

    void slotPong(Targoman::Common::stuPong& _pong);

private slots:
    Common::Configuration::stuRPCOutput rpcNormalize(const QVariantMap&_args);
    Common::Configuration::stuRPCOutput rpcText2IXML(const QVariantMap&);
    Common::Configuration::stuRPCOutput rpcIXML2Text(const QVariantMap&);
    Common::Configuration::stuRPCOutput rpcTokenize(const QVariantMap&);

private:
    void processDir(const QString& _dir, const QString &_basePath);
    void processFile(const QString &_inputFile, const QString& _outFile);
    QStringList retrieveFileItems(const QString& _filePath);
};

}
}
#endif // TARGOMAN_APPS_APPE4SMT_H
