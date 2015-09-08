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

namespace Targoman {
namespace Apps {

TranslationWriter* TranslationWriter::Instance = NULL;

TranslationWriter::TranslationWriter()
{
    this->LastInsertedIndex = 0;
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
    ++this->LastInsertedIndex;
}

void TranslationWriter::writeTranslation(quint64 _index, const QString &_translation)
{
    QMutexLocker Locker(&this->OutputListLock);

    if (_index == this->LastInsertedIndex + 1){
        this->writeTranslation(_translation);
        while(this->PendingTranslations.size() &&
              this->PendingTranslations.begin().key() == this->LastInsertedIndex)
            this->writeTranslation(this->PendingTranslations.begin().value());
    }else{
        this->PendingTranslations.insert(_index,_translation);
    }
}

}
}
