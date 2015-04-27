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

#include "TSMonitor.h"
#include "libTargomanCommon/JSONConversationProtocol.h"

namespace Targoman {
namespace Apps{
namespace Modules {

using namespace Common::Configuration;

tmplConfigurable<quint16> TSMonitor::UpdtaeInterval(
        TSMonitor::i"UpdtaeInterval")

void TSMonitor::run()
{
    try{
        this->start();
        this->exec();
    }catch(...){

    }
}

void TSMonitor::updateInfo()
{
    QMutexLocker(&this->Lock);

}

void TSMonitor::timerEvent(QTimerEvent *ev)
{
    this->updateInfo();
}

}
}
}
