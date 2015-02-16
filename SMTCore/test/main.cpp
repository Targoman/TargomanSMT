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

#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCore/Private/GlobalConfigs.h"
#include "libTargomanCommon/Logger.h"
//#include "libTargomanCore/clsTranslator.h"
#include "libTargomanCommon/clsSafeCoreApplication.h"

using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;
using namespace Targoman::Core;

int main(int argc, char *argv[])
{
    try{
        clsSafeCoreApplication App(argc, argv);

        ConfigManager::instance().init("kdsjh", QStringList()<<"--save"<<"-c"<<"../../Example/Model-fa2en/Targoman.conf");

        clsTranslator::init(ConfigManager::instance().configFilePath());

        clsTranslator MyTranslator(QStringLiteral("این یک آزمایش است."));
        MyTranslator.translate();
    }catch(exTargomanBase& e){
        TargomanError(e.what());
    }
}


