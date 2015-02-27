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

#include "UnitTest.h"

using namespace Targoman::Common;
using namespace Targoman::NLPLibs;

void UnitTest::initTestCase(){
    //TARGOMAN_REGISTER_ACTOR("testLibCommon");
//    Targoman::Common::Logger::instance().init("log.log");
    Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.setLevel(1);
    Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.setDetails(true);
    Targoman::Common::Logger::instance().setActive(false);

    Targoman::NLPLibs::TargomanTextProcessor::stuConfigs Configs;
    Configs.NormalizationFile = "../conf/Normalization.conf";
    Configs.AbbreviationsFile = "../conf/Abbreviations.tbl";
    Configs.SpellCorrectorBaseConfigPath = "../conf/SpellCorrectors";
    QVariantHash PersianSpellCorrector;
    PersianSpellCorrector.insert("Active", true);
    Configs.SpellCorrectorLanguageBasedConfigs.insert("fa", PersianSpellCorrector);
    Targoman::NLPLibs::TargomanTextProcessor::instance().init(Configs);
}

QTEST_MAIN(UnitTest)

