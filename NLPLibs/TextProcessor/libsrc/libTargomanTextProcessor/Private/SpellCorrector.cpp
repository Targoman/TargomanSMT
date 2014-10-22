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

#include "SpellCorrector.h"
#include <iostream>
#include <QRegExp>

#include <QFile>
#include <QStringList>

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

bool SpellCorrector::init(const QHash<QString, QVariantHash>& _settings)
{
    foreach(const QString& Lang, _settings.keys()){
        intfSpellCorrector* SpellChecker = this->Processors.value(Lang);
        if (SpellChecker){
            if (SpellChecker->init(_settings.value(Lang)) == false)
                return false;
        }else{
            TargomanLogWarn(5, QString("Spell Corrector for %1(%2) is not available").arg(
                                Lang).arg(ISO639getName(Lang.toAscii().constData())));
        }
    }

    foreach (const QString Lang, this->Processors.keys()){
        if (this->Processors[Lang] && this->Processors[Lang]->active() == false){
            delete this->Processors[Lang];
            this->Processors[Lang] = NULL;
        }
    }

    return true;
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
            if (_interactive && Processor->canBeCheckedInteractive(Token)){
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
                Phrase = FinalPhrase;
                Output.clear();
                Tokens = Phrase.trimmed().replace("  "," ").replace("  "," ").split(" ", QString::SkipEmptyParts);
                for(size_t FromTokenIndex=0; FromTokenIndex<=Tokens.size() - MaxTokens; FromTokenIndex++){
                    for (size_t i=0; i<MaxTokens; i++)
                        MultiWordBuffer.append(Tokens.at(FromTokenIndex+i));
                    Normalized = Processor->process(MultiWordBuffer);
                    if (Normalized.size()){
                        Output += Normalized;
                        if (Normalized.split(" ").size() >= (int)MaxTokens){
                            Tokens.removeAt(FromTokenIndex);
                            foreach(const QString& MidTokens, Normalized.split(" ", QString::SkipEmptyParts))
                                Tokens.insert(FromTokenIndex,MidTokens);
                            FromTokenIndex--; // As Token at FromTokenIndex has changed so reprocess it
                        }
                    }
                }
            }while(Output.trimmed() != Phrase.trimmed());
        }
    }while(Output.trimmed() != FinalPhrase.trimmed());

    return Output;
}

}
}
}
