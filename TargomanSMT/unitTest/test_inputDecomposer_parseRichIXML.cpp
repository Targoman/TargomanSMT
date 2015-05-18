#include "UnitTest.h"
#include "libTargomanTextProcessor/TextProcessor.h"

using namespace UnitTestNameSpace;
using namespace InputDecomposer;
using namespace Targoman::Common;



void clsUnitTest::test_clsInputDecomposer_parseRichIXML()
{

    QString ixmlString = "Saeed Torabzadeh <Number>24</Number><Unknown P=\"21\">dasd</Unknown>";
    clsInput::IsIXML.setFromVariant(true);
    clsInput::DoNormalize.setFromVariant(false);
    gConfigs.SourceVocab.clear();
    clsInput::SpecialTags.clear();

    //----------------allocateWordIndex----------------------------//

    clsInput::SpecialTags.insert("Unknown");

    for (int i=0; i<Targoman::NLPLibs::enuTextTags::getCount(); i++)
        clsInput::SpecialTags.insert(Targoman::NLPLibs::enuTextTags::toStr((Targoman::NLPLibs::enuTextTags::Type)i));

    foreach(const QString& Tag, clsInput::SpecialTags){
        WordIndex_t WordIndex = gConfigs.SourceVocab.size() + 1;
        gConfigs.SourceVocab.insert(Tag, WordIndex);
    }

    QStringList sentenceWords = {"Saeed", "Torabzadeh"};

    foreach(const QString& word, sentenceWords){
        WordIndex_t WordIndex = gConfigs.SourceVocab.size() + 1;
        gConfigs.SourceVocab.insert(word, WordIndex);
    }

    //-------------------------------------------------------------//

    clsInput InputDecomposer(ixmlString, false);


    /*
    PRINT_QVERIFY(InputDecomposer.tokens().size());
    for(int i = 0; i < InputDecomposer.tokens().size(); ++i)
    {
        PRINT_QVERIFY(InputDecomposer.tokens().at(i).string(),i);
        if(InputDecomposer.tokens().at(i).tagStr() == "")
            PRINT_QVERIFY(InputDecomposer.tokens().at(i).wordIndex(),i);
        if(InputDecomposer.tokens().at(i).tagStr() != "")
            PRINT_QVERIFY(InputDecomposer.tokens().at(i).tagStr(),i);
        PRINT_QVERIFY(InputDecomposer.tokens().at(i).attrs().size(),i);
        for(int j = 0;  j < InputDecomposer.tokens().at(i).attrs().size(); ++j)
        {
            PRINT_QVERIFY(InputDecomposer.tokens().at(i).attrs().keys().at(j),i,j);
            PRINT_QVERIFY(InputDecomposer.tokens().at(i).attrs().values().at(j).toString(),i,j);
        }
    }
    //*/


    QVERIFY( InputDecomposer.tokens().size() == 4 );
    QVERIFY( InputDecomposer.tokens().at(0).string() == "Saeed" );
    QVERIFY( InputDecomposer.tokens().at(0).wordIndex() == 12 );
    QVERIFY( InputDecomposer.tokens().at(0).attrs().size() == 0 );
    QVERIFY( InputDecomposer.tokens().at(1).string() == "Torabzadeh" );
    QVERIFY( InputDecomposer.tokens().at(1).wordIndex() == 13 );
    QVERIFY( InputDecomposer.tokens().at(1).attrs().size() == 0 );
    QVERIFY( InputDecomposer.tokens().at(2).string() == "24" );
    QVERIFY( InputDecomposer.tokens().at(2).tagStr() == "Number" );
    QVERIFY( InputDecomposer.tokens().at(2).attrs().size() == 0 );
    QVERIFY( InputDecomposer.tokens().at(3).string() == "dasd" );
    QVERIFY( InputDecomposer.tokens().at(3).tagStr() == "Unknown" );
    QVERIFY( InputDecomposer.tokens().at(3).attrs().size() == 1 );
    QVERIFY( InputDecomposer.tokens().at(3).attrs().keys().at(0) == "P" );
    QVERIFY( InputDecomposer.tokens().at(3).attrs().values().at(0).toString() == "21" );


}

