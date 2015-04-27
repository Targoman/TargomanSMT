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

#include <QFile>
#include <QTextStream>
#include "appTargomanLoadBalancer.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/CmdIO.h"

namespace Targoman {
namespace Apps {

using namespace Common;

void appTargomanLoadBalancer::slotExecute()
{
    try{

        QCoreApplication::exit(0);
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
