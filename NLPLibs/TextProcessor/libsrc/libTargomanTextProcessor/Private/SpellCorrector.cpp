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

#include <iostream>
#include <QRegExp>
#include <QTextStream>
#include <QFile>
#include <QStringList>

#include "SpellCorrector.h"

/****************************/
#include "SpellCheckers/clsPersianSpellCorrector.h"
/****************************/

namespace Targoman {
namespace NLPLibs {
namespace Private {

SpellCorrector* SpellCorrector::Instance = NULL;

SpellCorrector::SpellCorrector()
{
    this->Processors.insert("fa", new clsPersianSpellCorrector);
}

void SpellCorrector::init(const QHash<QString, QVariantHash>& _settings)
{
    foreach(const QString& Lang, _settings.keys()){
        intfSpellCorrector* SpellChecker = this->Processors.value(Lang);
        if (SpellChecker){
            SpellChecker->init(_settings.value(Lang));
        }else{
            TargomanLogWarn(5, QString("Spell Corrector for %1(%2) is not available").arg(
                                Lang).arg(ISO639getName(Lang.toAscii().constData())));
        }
    }

    QString LangCode;
    foreach (const QString Lang, this->Processors.keys()){
        LangCode = ISO639getAlpha2(Lang.toAscii().constData());
        if (this->Processors[Lang] && this->Processors[Lang]->active() == false){
            delete this->Processors[Lang];
            this->Processors[Lang] = NULL;
        }
    }
}

QString SpellCorrector::process(const QString& _lang, const QString& _inputStr, bool _interactive)
{
    intfSpellCorrector* Processor = this->Processors[_lang];
    if (!Processor)
        return _inputStr;

    QString Output = _inputStr;
    QString FinalPhrase;
    QString Phrase;
    QString Normalized;
    QStringList Tokens;
    QStringList MultiWordBuffer;
    /**/
    //Correct all single words
    do{
        Phrase = Output;
        Output.clear();
        Tokens = Phrase.trimmed().replace("  "," ").replace("  "," ").split(" ", QString::SkipEmptyParts);
        foreach(const QString& Token, Tokens){
            Normalized =  Processor->process(Token);
            if (Normalized.size())
                Output += Normalized + " ";
            else if (_interactive && Processor->canBeCheckedInteractive(Token)){
                std::cout<<"What to do with: <"<<Token.toUtf8().constData()<<">"<<std::endl;
                bool ValidSelection=false;
                while (!ValidSelection)
                {
                    std::cout<<"Press: (1: accept, 2: Normalize to)"<<std::endl;
                    QString Result = QChar((char)std::cin.get());
                    switch(Result.toInt ())
                    {
                    case 1:
                    {
                        Processor->storeAutoCorrectTerm(Token,Token);
                        ValidSelection = true;
                        break;
                    }
                    case 2:
                    {
                        std::string Buffer;

                        std::cout<<"Normalize: <"<<Token.toUtf8().constData()<<"> to:"<<std::endl;
                        std::cin >>Buffer;
                        Processor->storeAutoCorrectTerm(Token,QString::fromUtf8(Buffer.c_str()));
                        ValidSelection = true;
                        break;
                    }
                    default:
                        std::cout<<"Invalid Selection."<<std::endl;
                        break;
                    }
                }
            }else
                Output += Token + " ";
        }
    }while(Output.trimmed() != Phrase.trimmed());

    //Process Phrase starting to convert all bi-tokens then tri-tokens, so on until there are no more changes
    //Re-process whole phrase until there are no more changes
    do{
        FinalPhrase = Output;
        for (size_t MaxTokens=2; MaxTokens< Processor->maxAutoCorrectTokens(); ++MaxTokens){
            do{
                Phrase = Output;
                Output.clear();
                Tokens = Phrase.trimmed().replace("  "," ").replace("  "," ").split(" ", QString::SkipEmptyParts);
                for(size_t FromTokenIndex=0; FromTokenIndex<=Tokens.size() - MaxTokens; FromTokenIndex++){
                    MultiWordBuffer.clear();
                    for (size_t i=0; i<MaxTokens; i++)
                        MultiWordBuffer.append(Tokens.at(FromTokenIndex+i));

                    Normalized = Processor->process(MultiWordBuffer);
                    if (Normalized.size()){
                        Output += Normalized + " ";
                        if (Normalized.split(" ").size() >= (int)MaxTokens){
                            Tokens.removeAt(
                                        FromTokenIndex);
                            foreach(const QString& MidTokens, Normalized.split(" ", QString::SkipEmptyParts))
                                Tokens.insert(FromTokenIndex,MidTokens);
                            FromTokenIndex--; // As Token at FromTokenIndex has changed so reprocess it
                        }else
                            ++FromTokenIndex;
                    }else
                        Output += MultiWordBuffer.takeFirst() + " ";
                }
                Output += ((QStringList)Tokens.mid(Tokens.size()-MaxTokens + 1)).join(" ");
            }while(Output.trimmed() != Phrase.trimmed());
        }
    }while(Output.trimmed() != FinalPhrase.trimmed());

    return Output;
}

/**************************************************************************************************/
bool intfSpellCorrector::init(const QVariantHash _settings)
{
    this->AutoCorrectFile = _settings.value("ConfigPath").toString();
    if (this->AutoCorrectFile.isEmpty())
        throw exSpellCorrecter(this->Lang + "ConfigPath not provided for initialization");

    this->Active = _settings.value("Active").toBool();

    QFile ConfigFile(this->AutoCorrectFile);
    ConfigFile.open(QIODevice::ReadOnly);
    if(!ConfigFile.isReadable ())
        throw exSpellCorrecter("Unable to open file: <"+this->AutoCorrectFile+">.");

    QTextStream ConfigStream(&ConfigFile);
    ConfigStream.setCodec("UTF-8");

    QString ConfigLine;
    int CommentIndex = -1;
    int LineNumber = 0;
    bool IsEOF = false;
    stuConfigStep ConfigStep;

    while (!ConfigStream.atEnd() && !IsEOF)
    {
        ConfigLine = ConfigStream.readLine().trimmed();
        LineNumber++;
        if (ConfigLine.startsWith("##") || ConfigLine.trimmed().isEmpty())
            continue;

        if ((CommentIndex = ConfigLine.indexOf("##")) >= 0)
            ConfigLine.truncate(CommentIndex);

        ConfigLine = ConfigLine.trimmed();

        if (ConfigLine.startsWith("[") && ConfigLine.endsWith("]")){
            if (ConfigLine == "[EOF]")
                IsEOF = true;
            else{
                ConfigStep = this->ConfigSteps.value(ConfigLine.mid(1,ConfigLine.size() - 2));
                if (ConfigStep.KeyValStorage == NULL)
                    throw exSpellCorrecter("Invalid step definition in ");
            }
            continue;
        }

        if (ConfigLine.startsWith("NEW "))
            ConfigLine.remove(0, 4);

        if (ConfigStep.IsKeyVal){
            QStringList Pair = ConfigLine.split('=');
            if (Pair.size() == 2){
                QString Key = Normalizer::instance().normalize(Pair[0].trimmed());
                QString Val = Normalizer::instance().normalize(Pair[1].trimmed());
                ConfigStep.KeyValStorage->insert(Key, Val);
                this->MaxAutoCorrectTokens = qMax(this->MaxAutoCorrectTokens,
                                                  Key.split(" ", QString::SkipEmptyParts).size());
            }
            else {
                throw exSpellCorrecter(QString("Invalid Word Pair at line: %1 ==> %2").arg(LineNumber).arg(ConfigLine));
            }
        }else{
            QString N = Normalizer::instance().normalize(ConfigLine);
            ConfigStep.ListStorage->insert(Normalizer::instance().normalize(ConfigLine.trimmed()));
        }
    }

    if (!IsEOF)
        throw exSpellCorrecter("Invalid SpellCorrecter file as EOF section not found");

    TargomanInfo(5, this->Lang + " Config file loaded:");
    foreach (const QString& Step, this->ConfigSteps.keys())
        if (this->ConfigSteps.value(Step).IsKeyVal){
            TargomanInfo(5, "\t" + Step + ": " +
                            QString::number(this->ConfigSteps.value(Step).KeyValStorage->size()) + " Entries");
        }else{
            TargomanInfo(5, "\t" + Step + ": " +
                            QString::number(this->ConfigSteps.value(Step).ListStorage->size()) + " Entries");
        }

    this->MaxAutoCorrectTokens = qMax(4, this->MaxAutoCorrectTokens);
    return this->postInit(_settings);
}

}
}
}
