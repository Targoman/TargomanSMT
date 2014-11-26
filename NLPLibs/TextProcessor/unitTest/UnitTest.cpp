/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
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
    Targoman::NLPLibs::TextProcessor::stuConfigs Configs;
    Configs.NormalizationFile = "../conf/Normalization.conf";
    Configs.AbbreviationsFile = "../conf/Normalization.conf";
    Configs.SpellCorrectorBaseConfigPath = "../conf/SpellCorrectors";
    QVariantHash PersianSpellCorrector;
    PersianSpellCorrector.insert("Active", true);
    Configs.SpellCorrectorLanguageBasedConfigs.insert("fa", PersianSpellCorrector);
    Targoman::NLPLibs::TextProcessor::instance().init(Configs);
}

QTEST_MAIN(UnitTest)

