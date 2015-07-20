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

#ifndef TARGOMAN_APPS_APPTARGOMANLOADBALANCER_H
#define TARGOMAN_APPS_APPTARGOMANLOADBALANCER_H

#include "libTargomanCommon/clsSafeCoreApplication.h"
#include "libTargomanCommon/Types.h"


namespace Targoman {
namespace Apps{

class appTargomanLoadBalancer : public QObject
{
    Q_OBJECT
public:
    appTargomanLoadBalancer() {}

public slots:
    void slotExecute();

private slots:
    void slotValidateAgent(QString &_user, const QString &_pass, const QString &_ip, bool &_canView, bool &_canChange);
    void slotPong(Targoman::Common::stuPong &_pong);

};

}
}

#endif // TARGOMAN_APPS_APPTARGOMANLOADBALANCER_H
