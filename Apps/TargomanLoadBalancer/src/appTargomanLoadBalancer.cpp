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
#include <unistd.h>
#include <QFile>
#include <QTextStream>
#include "appTargomanLoadBalancer.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/CmdIO.h"
#include "Modules/TSMonitor.h"

namespace Targoman {
namespace Apps {

using namespace Common;
using namespace Modules;
void appTargomanLoadBalancer::slotExecute()
{
    try{
        while(1){
            sleep(3);
        TSMonitor::instance().bestServer();
        }
    }catch(exTargomanBase& e){
        TargomanError(e.what());
    }catch (std::exception &e){
        TargomanError(e.what());
    }catch(...){
        TargomanError("FATAL Unrecognized exception");
    }

    QCoreApplication::exit(-1);
}

}
}
