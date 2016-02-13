#!/bin/bash

MosesIniFilePath="$1"

TemplateConfig="[App]\nMaxThreads=5\nMode=Translation\n\n[Common]\nLanguage\Destination=en\nLanguage\Source=fa\nLanguage\WorkingMode=Decode\nModules\LM=clsKenLMProxy\nModules\RuleTable=clsMosesPlainRuleTable\n\n[FeatureFunctions]\n%FeatureFunctions%\n\n[IO]\nDebugDetail="true,false,false,false"\nDebugLevel=0\nErrorDetail="true,false,false,false"\nHappyDetail="true,false,false,false"\nHappyLevel=5\nInfoDetail="true,false,false,false"\nInfoLevel=5\nNormalDetail="true,false,false,false"\nNormalLevel=5\nWarningDetail="true,false,false,false"\nWarningLevel=5\n\n[IXMLTagHandler]\nHandlers=\nIgnoreUserDefinedTags=true\nKeepUnknownUserDefinedTags=false\n\n[Input]\nDoNormalize=true\nIsIXML=false\nTagNamedEntities=false\nTagSeparator=\nUserDefinedTags=\n\n[KenLMProxy]\nFilePath=%LmFilePath%\n\n[Logger]\nDebugDetail="true,true"\nDebugLevel=5\nDontShow=false\nErrorDetail="true,true"\nFile=\nHappyDetail="true,true"\nHappyLevel=5\nInfoDetail="true,true"\nInfoLevel=5\nWarningDetail="true,true"\nWarningLevel=5\n\n[MosesPlainRuleTable]\nMaxRuleNodeTargetRuleCount=%MaxRuleNodeTargetRuleCount%\nPhraseTableFilePath=%PhraseTableFilePath%\nReorderingTableFilePath=%ReorderingTableFilePath%\nWordAlignmentFilePath=%WordAlignmentFilePath%\n\n[NBest]\nMaxSuggestions=5\n\n[OOVHandler]\nCheckDifferentLetterCases=false\nHandlers=KeepSource\n\n[SearchGraphBuilder]\nDoComputePositionSpecificRestCosts=false\nHardReorderingJumpLimit=%MaxReorderingJump%\nKeepRecombined=true\nMaxCardinalityContainerSize=200\nMaxTargetPhraseCount=%MaxTargetPhraseCount%\nPrimaryCoverageShare=0\nPrunePreInsertion=true\nReorderingConstraintMaximumRuns=3\nSearchBeamWidth=5\n\n[TargomanLM]\nDefaultUnknownBackoff=0\nDefaultUnknownProb=-100\n;Just to suppress an error report due to bug in configuration module\nFilePath=%LmFilePath%\nUseIndexBasedModel=true\nVerifyBinaryChecksum=true\n"

MosesIniContent=$(tr '\n' '#' < "$MosesIniFilePath")#

PhraseTableParams=$(echo "$MosesIniContent" | perl -pe 's%.*\[ttable-file\]\s*#*\s*(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(.*?)#.*%"\1" "\2" "\3" "\4" "\5"%')
eval "PhraseTableParams=($PhraseTableParams)"
if [ ${#PhraseTableParams[@]} -ne 5 ]; then
    echo -e "Invalid moses ini file. Translation table should specify five parameters but got:\n${PhraseTableParams[@]}\n" >&2
    exit -1
fi
Type=${PhraseTableParams[0]}
SourceFactors=${PhraseTableParams[1]}
TargetFactors=${PhraseTableParams[2]}
NumberOfPhraseTableScores=${PhraseTableParams[3]}
PhraseTableFilePath=${PhraseTableParams[4]}
if [ $Type -ne 0 ]; then
    echo -e "Only textual phrase tables are supported.\nType should be 0 but is $Type\n" >&2
    exit -1
fi
if [ $SourceFactors -ne 0 ]; then
    echo -e "Factored translation models are not supported.\nSource factor should be 0 but is $SourceFactors\n" >&2
    exit -1
fi
if [ $TargetFactors -ne 0 ]; then
    echo -e "Factored translation models are not supported.\nTarget factor should be 0 but is $TargetFactors\n" >&2
    exit -1
fi
ReorderingTableParams=$(echo "$MosesIniContent" | perl -pe 's%.*\[distortion-file\]\s*#+\s*(\d+)-(\d+)\s+(\S+)\s+(\d+)\s+(.*?)#.*%"\1" "\2" "\3" "\4" "\5"%')
eval "ReorderingTableParams=($ReorderingTableParams)"
if [ ${#ReorderingTableParams[@]} -ne 5 ]; then
    echo -e "Invalid moses ini file. Reordering table should specify five parameters but got:\n${ReorderingTableParams[@]}\n" >&2
    exit -1
fi
SourceFactors=${ReorderingTableParams[0]}
TargetFactors=${ReorderingTableParams[1]}
NumberOfReorderingScores=${ReorderingTableParams[3]}
ReorderingTableFilePath=${ReorderingTableParams[4]}
if [ $SourceFactors -ne 0 ]; then
    echo -e "Factored translation models are not supported.\nSource factor for lexical reordering should be 0 but is $SourceFactors\n" >&2
    exit -1
fi
if [ $TargetFactors -ne 0 ]; then
    echo -e "Factored translation models are not supported.\nTarget factor for lexical reordering should be 0 but is $TargetFactors\n" >&2
    exit -1
fi
if [ $NumberOfReorderingScores -ne 3 ] && [ $NumberOfReorderingScores -ne 6 ]; then
    echo -e "Only forward and bidirectional form of M(onotone)S(wap)D(iscontinous) lexical reordering is supported.\nShould have 3 or 6 scores but has $NumberOfReorderingScores scores\n" >&2
    exit -1
fi
LanguageModelParams=$(echo "$MosesIniContent" | perl -pe 's%.*\[lmodel-file\]\s*#+\s*(\d+)\s+(\d+)\s+(\d+)\s+(.*?)#.*%"\1" "\2" "\3" "\4"%')
eval "LanguageModelParams=($LanguageModelParams)"
if [ ${#LanguageModelParams[@]} -ne 4 ]; then
    echo -e "Language model section should specifiy four parameters but got:\n${LanguageModelParams[@]}\n" >&2
    exit -1
fi
Type=${LanguageModelParams[0]}
SourceFactors=${LanguageModelParams[1]}
LanguageModelOrder=${LanguageModelParams[2]}
LanguageModelFilePath=${LanguageModelParams[3]}
if [ $Type -ne 0 ] && [ $Type -ne 8 ]; then
    echo -e "Only ARPA and KenLM binary language model formats are supported. The type should be 0 or 8 but got $Type\n" >&2
    exit -1
fi
if [ $SourceFactors -ne 0 ]; then
    echo -e "Factored translation models are not supported.\nSource factor for language model should be 0 but is $SourceFactors\n" >&2
    exit -1
fi
PhraseTableLambdas=$(echo "$MosesIniContent" | perl -pe 's%.*\[weight-t\]\s*#\s*((?:[\-\d\.]+\s*#+\s*)+)[\[#].*%\1%;s%#% %g;s%\s+$%%g')
eval "PhraseTableLambdas=($PhraseTableLambdas)"
if [ ${#PhraseTableLambdas[@]} -ne $NumberOfPhraseTableScores ]; then
    echo -e "Number of phrase table scores does not match its lambdas.\nShould be $NumberOfPhraseTableScores but is ${#PhraseTableLambdas[@]}\n" >&2
    exit -1
fi
LexicalReorderingLambdas=$(echo "$MosesIniContent" | perl -pe 's%.*\[weight-d\]\s*#\s*((?:[\-\d\.]+\s*#+\s*)+)[\[#].*%\1%;s%#% %g;s%\s+$%%g')
eval "LexicalReorderingLambdas=($LexicalReorderingLambdas)"
if [ ${#LexicalReorderingLambdas[@]} -ne $((NumberOfReorderingScores+1)) ]; then
    echo -e "Number of lexical reordering scores does not match its lambdas.\nShould be $NumberOfReorderingScores+1 but is ${#LexicalReorderingLambdas[@]}\n" >&2
    exit -1
fi
LanguageModelLambdas=$(echo "$MosesIniContent" | perl -pe 's%.*\[weight-l\]\s*#\s*((?:[\-\d\.]+\s*#+\s*)+)[\[#].*%\1%;s%#% %g;s%\s+$%%g')
eval "LanguageModelLambdas=($LanguageModelLambdas)"
if [ ${#LanguageModelLambdas[@]} -ne 1 ]; then
    echo -e "Number of language model lambdas must be 1 but is ${#LanguageModelLambdas[@]}\n" >&2
    exit -1
fi
WordPenaltyLambdas=$(echo "$MosesIniContent" | perl -pe 's%.*\[weight-w\]\s*#\s*((?:[\-\d\.]+\s*#+\s*)+)[\[#].*%\1%;s%#% %g;s%\s+$%%g')
eval "WordPenaltyLambdas=($WordPenaltyLambdas)"
if [ ${#WordPenaltyLambdas[@]} -ne 1 ]; then
    echo -e "Number of word penalty lambdas must be 1 but is ${#WordPenaltyLambdas[@]}\n" >&2
    exit -1
fi
MaxReorderingJump=$(echo "$MosesIniContent" | perl -pe 's%.*\[distortion-limit\]\s*#\s*((?:[\d]+\s*#+\s*)+)[\[#].*%\1%;s%#% %g;s%\s+$%%g')
eval "MaxReorderingJump=($MaxReorderingJump)"
if [ ${#MaxReorderingJump[@]} -ne 1 ]; then
    echo -e "Max Reordering Jump must be a single number but is ${#MaxReorderingJump[@]}\n" >&2
    exit -1
fi
MaxTargetPhraseCount=$(echo "$MosesIniContent" | perl -pe 's%.*\[ttable-limit\]\s*#\s*((?:[\d]+\s*#+\s*)+)[\[#].*%\1%;s%#% %g;s%\s+$%%g')
eval "MaxTargetPhraseCount=($MaxTargetPhraseCount)"
if [ ${#MaxTargetPhraseCount[@]} -ne 1 ]; then
    echo -e "Max target phrase count must be a single number but is ${#MaxTargetPhraseCount[@]}\n" >&2
    exit -1
fi
FeatureFunctionContent="LanguageModel\\ScalingFactor=${LanguageModelLambdas[0]}"
for((i=0; i < ${#PhraseTableLambdas[@]}; ++i)); do
    FeatureFunctionContent="$FeatureFunctionContent\nPhraseTable\\ScalingFactors\\PhraseFeature$i=${PhraseTableLambdas[i]}"
done
if [ ${#LexicalReorderingLambdas[@]} -eq 7 ]; then
    ReorderingIsBidirectional="true"
else
    ReorderingIsBidirectional="false"
fi
FeatureFunctionContent="$FeatureFunctionContent\nReorderingJump\\MaximumJumpWidth=${MaxReorderingJump[0]}"
FeatureFunctionContent="$FeatureFunctionContent\nReorderingJump\\ScalingFactor=${LexicalReorderingLambdas[0]}"
FeatureFunctionContent="$FeatureFunctionContent\nLexicalReordering\\IsBidirectional=$ReorderingIsBidirectional"
FeatureFunctionContent="$FeatureFunctionContent\nLexicalReordering\\ForwardMonotone=${LexicalReorderingLambdas[4]}"
FeatureFunctionContent="$FeatureFunctionContent\nLexicalReordering\\ForwardSwap=${LexicalReorderingLambdas[5]}"
FeatureFunctionContent="$FeatureFunctionContent\nLexicalReordering\\ForwardDiscontinous=${LexicalReorderingLambdas[6]}"
FeatureFunctionContent="$FeatureFunctionContent\nLexicalReordering\\BackwardMonotone=${LexicalReorderingLambdas[1]}"
FeatureFunctionContent="$FeatureFunctionContent\nLexicalReordering\\BackwardSwap=${LexicalReorderingLambdas[2]}"
FeatureFunctionContent="$FeatureFunctionContent\nLexicalReordering\\BackwardDiscontinous=${LexicalReorderingLambdas[3]}"
FeatureFunctionContent="$FeatureFunctionContent\nWordPenalty\\ScalingFactor=${WordPenaltyLambdas[0]}"
FeatureFunctionContent="$FeatureFunctionContent\nUnknownWordPenalty\\ScalingFactor=1.0"

FeatureFunctionContent=$(echo -e $FeatureFunctionContent | perl -pe 's%\n%\\\\n%g')

TemplateConfig=$(echo "$TemplateConfig" | perl -pe 's|%FeatureFunctions%|'"$FeatureFunctionContent"'|;s|%LmFilePath%|'"$LanguageModelFilePath"'|g;s|%MaxRuleNodeTargetRuleCount%|'"$MaxTargetPhraseCount"'|g;s|%PhraseTableFilePath%|'"$PhraseTableFilePath"'|g;s|%ReorderingTableFilePath%|'"$ReorderingTableFilePath"'|g;s|%WordAlignmentFilePath%||g;s|%MaxReorderingJump%|'"$MaxReorderingJump"'|g;s|%MaxTargetPhraseCount%|'"$MaxTargetPhraseCount"'|g;')

echo -e $TemplateConfig
