/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
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
    QDir ApplicationDir(QCoreApplication::applicationDirPath());
    Configs.NormalizationFile = ApplicationDir.absoluteFilePath("../conf/Normalization.conf");
    Configs.AbbreviationsFile = ApplicationDir.absoluteFilePath("../conf/Abbreviations.tbl");
    Configs.SpellCorrectorBaseConfigPath = ApplicationDir.absoluteFilePath("../conf/SpellCorrectors");
    QVariantHash PersianSpellCorrector;
    PersianSpellCorrector.insert("Active", true);
    Configs.SpellCorrectorLanguageBasedConfigs.insert("fa", PersianSpellCorrector);
    Targoman::NLPLibs::TargomanTextProcessor::instance().init(Configs);
}

QTEST_MAIN(UnitTest)

