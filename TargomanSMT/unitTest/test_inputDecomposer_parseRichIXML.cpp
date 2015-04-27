#include "UnitTest.h"
#include "libTargomanTextProcessor/TextProcessor.h"

using namespace UnitTestNameSpace;
using namespace InputDecomposer;
using namespace Targoman::Common;



void clsUnitTest::test_clsInputDecomposer_parseRichIXML()
{

    QString ixmlString = "Saeed Torabzadeh <Number>24</Number> <Unknown P=21>dasd</Unknown>";
    clsInputDecomposer::IsIXML.setFromVariant(true);
    clsInputDecomposer::DoNormalize.setFromVariant(false);


    clsInputDecomposer InputDecomposer(ixmlString);

    //----------------allocateWordIndex----------------------------//

    for (int i=0; i<Targoman::NLPLibs::enuTextTags::getCount(); i++)
        clsInputDecomposer::SpecialTags.insert(Targoman::NLPLibs::enuTextTags::toStr((Targoman::NLPLibs::enuTextTags::Type)i));

    foreach(const QString& Tag, clsInputDecomposer::SpecialTags){
        WordIndex_t WordIndex = gConfigs.SourceVocab.size() + 1;
        gConfigs.SourceVocab.insert(Tag, WordIndex);
    }

    QStringList sentenceWords = {"Saeed", "Torabzadeh"};

    foreach(const QString& word, sentenceWords){
        WordIndex_t WordIndex = gConfigs.SourceVocab.size() + 1;
        gConfigs.SourceVocab.insert(word, WordIndex);
    }

    //-------------------------------------------------------------//

    //InputDecomposer.parseRichIXML(ixmlString);
    //InputDecomposer.tokens().at(0).

}

