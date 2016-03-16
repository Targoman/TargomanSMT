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
 */


#include <QMutexLocker>
#include "TranslationWriter.h"
#include "Configs.h"
#include "libTargomanCommon/Types.h"

namespace Targoman {
namespace Apps {

TranslationWriter::TranslationWriter()
{
    this->LastSavedIndex = 0;
    if (gConfigs::OutputFile.value().size())
        QFile::remove(gConfigs::OutputFile.value());
}

void TranslationWriter::writeTranslation(const QString &_translation)
{
    if (gConfigs::OutputFile.value().size()){
        QFile OutFile(gConfigs::OutputFile.value());
        if (OutFile.open(QFile::Append), OutFile.isWritable() == false)
            throw exTargomanSMTConsole("Unable to open: "+ OutFile.fileName() + " for Writing");
        QTextStream Stream(&OutFile);
        Stream.setCodec("UTF-8");
        Stream<<_translation<<"\n";

    }else{
        TargomanHappy(1,_translation)
    }
    ++this->LastSavedIndex;
    this->PendingTranslations.remove(this->LastSavedIndex);
}

void TranslationWriter::finialize()
{
    while(this->PendingTranslations.size())
        this->writeTranslation(this->PendingTranslations.begin().key(), this->PendingTranslations.begin().value());
}

//void TranslationWriter::writeTranslation(quint64 _index, const QString &_translation)
//{
//    QMutexLocker Locker(&this->OutputListLock);

//    if (_index == this->LastSavedIndex + 1){
//        this->writeTranslation(_translation);
//    }else{
//        this->PendingTranslations.insert(_index, _translation);
//    }

//    while(this->PendingTranslations.size() &&
//          this->PendingTranslations.firstKey() == this->LastSavedIndex + 1){
//        this->writeTranslation(this->PendingTranslations.begin().value());
//    }
//}

void TranslationWriter::writeTranslation(quint64 _index, const stuTranslationOutput &_output)
{
    QMutexLocker Locker(&this->OutputListLock);

    if (_index == this->LastSavedIndex + 1){
        this->writeNBest(_index, _output.NBestTranslations);
        this->writeTranslation(_output.Translation);
    }else{
        this->PendingTranslations.insert(_index, _output);
    }

    while(this->PendingTranslations.size() &&
          this->PendingTranslations.firstKey() == this->LastSavedIndex + 1){
        this->writeNBest(this->PendingTranslations.begin().key(), this->PendingTranslations.begin().value().NBestTranslations);
        this->writeTranslation(this->PendingTranslations.begin().value().Translation);

    }
}

void TranslationWriter::writeNBest(quint64 _index, const QList<QString> &_nBestTranslations)
{

    if (gConfigs::NBestFile.value().size()){
        QFile OutFile(gConfigs::NBestFile.value());
        if (OutFile.open(QFile::Append), OutFile.isWritable() == false)
            throw exTargomanSMTConsole("Unable to open: "+ OutFile.fileName() + " for Writing");
        QTextStream Stream(&OutFile);
        Stream.setCodec("UTF-8");
        for(int i = 0; i < _nBestTranslations.size(); i++){
            Stream<< QString::number(_index - 1) << " ||| " << _nBestTranslations[i] <<"\n";
        }

    }
}

}
}
