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
    clsInputDecomposer::UserDefinedTags.setFromVariant(
                userDefinedTags.join(gConfigs.Separator.value())
                );
    clsInputDecomposer::IsIXML.setFromVariant(true);
    clsInputDecomposer::DoNormalize.setFromVariant(false);
    clsInputDecomposer::init("");

    foreach(const QString& tag, userDefinedTags) {
        QVERIFY(gConfigs.SourceVocab.find(tag) != gConfigs.SourceVocab.end());
        QVERIFY(clsInputDecomposer::SpecialTags.find(tag) !=
                clsInputDecomposer::SpecialTags.end());
    }

    for (int i=0; i<Targoman::NLPLibs::enuTextTags::getCount(); i++)
    {
        QString tag = Targoman::NLPLibs::enuTextTags::toStr((Targoman::NLPLibs::enuTextTags::Type)i);
        QVERIFY(gConfigs.SourceVocab.find(tag) != gConfigs.SourceVocab.end());
        QVERIFY(clsInputDecomposer::SpecialTags.find(tag) !=
                clsInputDecomposer::SpecialTags.end());
    }
}


