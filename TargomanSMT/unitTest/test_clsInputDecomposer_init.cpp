#include "UnitTest.h"
#include "libTargomanTextProcessor/TextProcessor.h"
using namespace UnitTestNameSpace;
using namespace InputDecomposer;
void clsUnitTest::test_clsInputDecomposer_init()
{
    QStringList userDefinedTags;
    userDefinedTags << "tag1" << "tag2";
    gConfigs.SourceVocab.clear();
    gConfigs.Separator.setFromVariant(",");
    clsInput::UserDefinedTags.setFromVariant(
                userDefinedTags.join(gConfigs.Separator.value())
                );
    clsInput::IsIXML.setFromVariant(true);
    clsInput::DoNormalize.setFromVariant(false);
    clsInput::init("");

    foreach(const QString& tag, userDefinedTags) {
        QVERIFY(gConfigs.SourceVocab.find(tag) != gConfigs.SourceVocab.end());
        QVERIFY(clsInput::SpecialTags.find(tag) !=
                clsInput::SpecialTags.end());
    }

    for (int i=0; i<Targoman::NLPLibs::enuTextTags::getCount(); i++)
    {
        QString tag = Targoman::NLPLibs::enuTextTags::toStr((Targoman::NLPLibs::enuTextTags::Type)i);
        QVERIFY(gConfigs.SourceVocab.find(tag) != gConfigs.SourceVocab.end());
        QVERIFY(clsInput::SpecialTags.find(tag) !=
                clsInput::SpecialTags.end());
    }
}


