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
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include "UnitTest.h"
#include "libTargomanTextProcessor/TextProcessor.h"
using namespace UnitTestNameSpace;
using namespace InputDecomposer;
void clsUnitTest::test_clsInputDecomposer_init()
{
    QStringList userDefinedTags;
    userDefinedTags << "tag1" << "tag2";
    gConfigs.SourceVocab.clear();
    clsInput::TagSeparator.setFromVariant(",");
    clsInput::UserDefinedTags.setFromVariant(
                userDefinedTags.join(clsInput::TagSeparator.value())
                );
    clsInput::IsIXML.setFromVariant(true);
    clsInput::DoNormalize.setFromVariant(false);
    clsInput::init(QSharedPointer<QSettings>());

    foreach(const QString& tag, userDefinedTags) {
        QVERIFY(gConfigs.SourceVocab.find(tag) == gConfigs.SourceVocab.end());
        QVERIFY(clsInput::SpecialTags.find(tag) !=
                clsInput::SpecialTags.end());
    }

    for (int i=0; i<Targoman::NLPLibs::enuTextTags::getCount(); i++)
    {
        QString tag = Targoman::NLPLibs::enuTextTags::toStr((Targoman::NLPLibs::enuTextTags::Type)i);
        QVERIFY(gConfigs.SourceVocab.find(tag) == gConfigs.SourceVocab.end());
        QVERIFY(clsInput::SpecialTags.find(tag) !=
                clsInput::SpecialTags.end());
    }
}


