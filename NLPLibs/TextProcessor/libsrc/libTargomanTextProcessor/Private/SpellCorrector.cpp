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

    QString Output;
    QString Phrase = _inputStr;
    for (uint i=1; i< Processor->maxAutoCorrectTokens(); ++i){
        QString Normalized;
        QStringList Tokens = Phrase.trimmed().replace("  "," ").replace("  "," ").split(" ", QString::SkipEmptyParts);
        QStringList MultiWordBuffer;

        Output.clear();

        for (uint i=1; i< Processor->maxAutoCorrectTokens(); ++i){
            Tokens.append("");
        }

        foreach(const QString& Token, Tokens){
            Normalized =  Processor->AutoCorrectTerms.value(Token);
            if (Normalized.size())
                MultiWordBuffer.append(Normalized);
            else{
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
                            Processor->AutoCorrectTerms.insert(Token,Token);
                            QFile File(Processor->autoCorrectFile());
                            File.copy(Processor->autoCorrectFile() + ".back");
                            File.open(QFile::Append);
                            File.write((Token + '\t' + Token).toUtf8());
                            ValidSelection = true;
                            break;
                        }
                        case 2:
                        {
                            std::string Buffer;

                            std::cout<<"Normalize: <"<<Token.toUtf8().constData()<<"> to:"<<std::endl;
                            std::cin >>Buffer;
                            Processor->AutoCorrectTerms.insert(Token,QString::fromUtf8(Buffer.c_str()));
                            QFile File(Processor->autoCorrectFile());
                            File.copy(Processor->autoCorrectFile() + ".back");
                            File.open(QFile::Append);
                            File.write((Token + '\t' + QString::fromUtf8(Buffer.c_str())).toUtf8());
                            ValidSelection = true;
                            break;
                        }
                        default:
                            std::cout<<"Invalid Selection."<<std::endl;
                            break;
                        }
                    }
                }else
                    MultiWordBuffer.append(Token);
            }

            for( int i= 2; i <= MultiWordBuffer.size(); i++){
                QString Normalized = Processor->process(MultiWordBuffer.mid(0, i));
                if (Normalized.size()){
                    MultiWordBuffer[i-1] = Normalized;
                    for (int j = 0; j<i-1 ; j++)
                        MultiWordBuffer.takeFirst();
                }
            }

            if (MultiWordBuffer.size() >= Processor->maxAutoCorrectTokens()){
                Output.append(" ");
                Output.append(MultiWordBuffer.takeFirst());
            }
        }
        Phrase = Output;
    }
    return Output;
}

}
}
}
