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
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include <iostream>
#include <QRegExp>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <QDebug>

#include "SpellCorrector.h"

#define DUAL_SPACE "  "

namespace Targoman {
namespace NLPLibs {
namespace TargomanTP{
namespace Private {

/**
 * @brief instructor of this class adds default languages data to the #Processors variable.
 */
SpellCorrector::SpellCorrector():
    refNormalizerInstance(this->refNormalizerInstance)
{}

/**
 * @brief Initializes all languages that are in _settings variable and delete each language from #Processors if it is not active.
 * @param _baseConfigPath base config path of all language specific spellCorrectors.
 * @param _settings input HashMap that its key is language name and its value is language specific settings.
 */
void SpellCorrector::init(const QString& _baseConfigPath, const QHash<QString, QVariantHash>& _settings)
{
    foreach(const QString& Lang, _settings.keys()){
        intfSpellCorrector* SpellCorrector = this->Processors.value(Lang);
        if (SpellCorrector){
            SpellCorrector->init(_baseConfigPath, _settings.value(Lang));
        }else{
            TargomanLogWarn(5, QString("Spell Corrector for %1(%2) is not available").arg(
                                Lang).arg(ISO639getName(Lang.toLatin1().constData())));
        }
    }

    QString LangCode;
    foreach (const QString Lang, this->Processors.keys()){
        LangCode = ISO639getAlpha2(Lang.toLatin1().constData());
        if (this->Processors.value(Lang, NULL) && this->Processors.value(Lang)->active() == false)
            delete this->Processors.take(Lang);
    }
}

/**
 * @brief The main function of SpellCorrector class
 * This function first, corrects all single words.
 * Then, reprocesses multi token groups of input text to unify valid consecutive tokens.
 * @param _lang Language name
 * @param _inputStr Input string
 * @param _interactive Can spell correction process be done intractively or not.
 * @return spell corrected string.
 */

QString SpellCorrector::process(const QString& _lang,
                                const QString& _inputStr,
                                INOUT bool& _changed,
                                bool _interactive)
{
    intfSpellCorrector* Processor = this->Processors.value(_lang, NULL);
    if (!Processor)
        return _inputStr;

    QString Output = _inputStr;
    QString FinalPhrase;
    QString Phrase;
    QString Normalized;
    QStringList Tokens;
    QStringList MultiWordBuffer;
    _changed = false;

    //Correct all single words
    do{
        Phrase = Output;
        Output.clear();
        // TODO: Why the phrase have to be trimmed before splitting, we will be ignoring empty parts anyways?
        Tokens = Phrase.trimmed().replace(DUAL_SPACE," ").replace(DUAL_SPACE," ").split(" ", QString::SkipEmptyParts);
        foreach(const QString& Token, Tokens){
            Normalized =  Processor->process(Token); // process each token by language based spell corrector.
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

    //Process Phrase, processing all bi-tokens then tri-tokens, so on until there are no more changes
    //Re-process whole phrase until there are no more changes
    do{
        FinalPhrase = Output;
        for (int MaxTokens=2; MaxTokens< Processor->maxAutoCorrectTokens(); ++MaxTokens){
            do{
                Phrase = Output;
                Tokens = Phrase.trimmed().replace(DUAL_SPACE," ").replace(DUAL_SPACE," ").split(" ", QString::SkipEmptyParts);
                if (Tokens.size() < MaxTokens)
                    break;
                Output.clear();

                bool HasRemaining = true;
                for(int FromTokenIndex=0; FromTokenIndex<=Tokens.size() - MaxTokens ; FromTokenIndex++){
                    HasRemaining = true;
                    MultiWordBuffer.clear();
                    for (int i=0; i<MaxTokens; i++)
                        if ((FromTokenIndex + i) < Tokens.size())
                            MultiWordBuffer.append(Tokens.at(FromTokenIndex+i));

                    if (MultiWordBuffer.size() < MaxTokens - 1)
                        continue;

                    Normalized = Processor->process(MultiWordBuffer); // if it is unable to unify multi tokens, returns empty string.
                    if (Normalized.size()){
                        Output += Normalized + " ";
                        QStringList NormalizedTokens = Normalized.split(" ");
                        if (NormalizedTokens.size() >= MaxTokens){
                            int TokensToRemove = MaxTokens;
                            while(TokensToRemove > 0) {
                                Tokens.removeAt(FromTokenIndex);
                                --TokensToRemove;
                            }
                            for(int NormalizedTokenIndex = NormalizedTokens.size() - 1; NormalizedTokenIndex >= 0; --NormalizedTokenIndex)
                                Tokens.insert(FromTokenIndex,NormalizedTokens.at(NormalizedTokenIndex));
                            // TODO: This causes bug as we have already added the normalized version to the output
                            //FromTokenIndex--; // As Token at FromTokenIndex has changed so reprocess it. We decrease one, because we will add one in for loop.
                            FromTokenIndex+=MaxTokens - 1; // increased FromTokenIndex for MaxTokens to pass unified token and decrease one, because we will add one in for loop.
                            HasRemaining = false;
                        }else{
                            FromTokenIndex+=MaxTokens - 1; // increased FromTokenIndex for MaxTokens to pass unified token and decrease one, because we will add one in for loop.
                            if(FromTokenIndex + 1 == Tokens.size())
                                HasRemaining = false;
                        }
                    }else
                        Output += MultiWordBuffer.takeFirst() + " ";
                }
                if (HasRemaining)
                    Output += ((QStringList)Tokens.mid(Tokens.size()-MaxTokens + 1)).join(" ");
            }while(Output.trimmed() != Phrase.trimmed());
        }
    }while(Output.trimmed() != FinalPhrase.trimmed());

    if (Output.trimmed() != _inputStr.trimmed())
        _changed = true;

    return Output;
}

/**************************************************************************************************/
intfSpellCorrector::intfSpellCorrector(const char _code[]) :
    refNormalizerInstance(Normalizer::instance())
{
    this->Active = true;
    SpellCorrector::instance().registerProcessor(_code, this);
}

/**
 * @brief initialize spellCorrector configurations.
 *
 * Every language has some specific configurations.
 * This function can load all configurations of language specific spellCheckers without any need to overload it in derived classes.
 * This function also calculates and sets #MaxAutoCorrectTokens and call postInit function language specific spellCorrectors.
 *
 * @param _baseConfigPath base address of language specific configuration path.
 * @param _settings some other language specific settings.
 * @exception throws exception if it is unable to open a config file.
 * @exception throws exception if a keyVal config type file, dosn't have a valid data line.
 * @return Returns true if initialization process is succeded.
 */

bool intfSpellCorrector::init(const QString& _baseConfigPath, const QVariantHash _settings)
{
    this->Active = _settings.value("Active",true).toBool();
    TargomanInlineInfo(5, "Loading " << this->Lang << " Config file...");

    this->MaxAutoCorrectTokens = 0;
    foreach (const stuConfigType& Config, this->ConfigTypes){ // #ConfigTypes list is already initiallized in constructor of language specific spellCorrectors.
        QString ConfigFilePath = _baseConfigPath + "/" + this->Lang + "/" + Config.Name + ".tbl";
        if (QFile::exists(ConfigFilePath) == false)
            throw exSpellCorrector("File: <"+ConfigFilePath+"> Not found");
        QFile ConfigFile(ConfigFilePath);
        ConfigFile.open(QIODevice::ReadOnly);
        if(!ConfigFile.isReadable ())
            throw exSpellCorrector("Unable to open file: <"+ConfigFilePath+">.");

        QTextStream ConfigStream(&ConfigFile);
        ConfigStream.setCodec("UTF-8");
        QString ConfigLine;
        int CommentIndex = -1;
        int LineNumber = 0;

        while (!ConfigStream.atEnd())
        {
            ConfigLine = ConfigStream.readLine().trimmed();
            LineNumber++;
            if (ConfigLine.startsWith("##") || ConfigLine.trimmed().isEmpty())
                continue;

            if ((CommentIndex = ConfigLine.indexOf("##")) >= 0)
                ConfigLine.truncate(CommentIndex);

            ConfigLine = ConfigLine.trimmed();

            if (ConfigLine.startsWith("NEW "))
                ConfigLine.remove(0, 4);

            if (Config.IsKeyVal){
                QStringList Pair = ConfigLine.split('=');
                if (Pair.size() == 2){
                    QString Key = this->refNormalizerInstance.normalize(Pair[0].trimmed());
                    QString Val = this->refNormalizerInstance.normalize(Pair[1].trimmed());
                    Config.KeyValStorage->insert(Key, Val);
                    this->MaxAutoCorrectTokens = qMax(this->MaxAutoCorrectTokens,
                                                      Key.split(" ", QString::SkipEmptyParts).size()); //finds maximum possible tokens, by checking each data line.
                }
                else {
                    throw exSpellCorrector(QString("Invalid Word Pair at line: %1 ==> %2").arg(LineNumber).arg(ConfigLine));
                }
            }else
                Config.ListStorage->insert(this->refNormalizerInstance.normalize(ConfigLine.trimmed()));

        }
    }

    TargomanFinishInlineInfo(TARGOMAN_COLOR_HAPPY, this->Lang + " Loaded");
    foreach (const stuConfigType& Config, this->ConfigTypes)
        if (Config.IsKeyVal){
            TargomanInfo(5, "\t" + Config.Name + ": " +
                         QString::number(Config.KeyValStorage->size()) + " Entries");
        }else{
            TargomanInfo(5, "\t" + Config.Name + ": " +
                         QString::number(Config.ListStorage->size()) + " Entries");
        }

    this->MaxAutoCorrectTokens = qMax(4, this->MaxAutoCorrectTokens);
    return this->postInit(_settings);
}

}
}
}
}
