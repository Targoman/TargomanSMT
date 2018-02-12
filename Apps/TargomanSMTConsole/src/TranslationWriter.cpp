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

TranslationWriter::TranslationWriter()
{
    this->LastSavedIndex = 0;
    if (gConfigs::OutputFile.value().size())
        QFile::remove(gConfigs::OutputFile.value());
}

void TranslationWriter::writeOutputLines(const QList<QString>& _outputLines)
{
    if (gConfigs::OutputFile.value().size()){
        QFile OutFile(gConfigs::OutputFile.value());
        if (OutFile.open(QFile::Append), OutFile.isWritable() == false)
            throw exTargomanSMTConsole("Unable to open: "+ OutFile.fileName() + " for Writing");
        QTextStream Stream(&OutFile);
        Stream.setCodec("UTF-8");
        foreach(const QString& Line, _outputLines)
            Stream << Line << "\n";

    }else{
        foreach(const QString& Line, _outputLines)
            TargomanHappy(1, Line)
    }
    // TODO: Make this more appropriate
    ++this->LastSavedIndex;
    this->PendingTranslations.remove(this->LastSavedIndex);
}

QString TranslationWriter::getNBestPathString(const QString& _translation,  const SMT::stuTranslationOutput::stuCostElements& _costElements)
{
    const QString SEPARATOR = " ||| ";
    QStringList FeatureCosts;
    foreach(const auto& CostElement, _costElements.Elements) {
        FeatureCosts << CostElement.FeatureName + "= " + QString::number(-CostElement.Cost);
    }
    return _translation + SEPARATOR + FeatureCosts.join(" ") + SEPARATOR + QString::number(-_costElements.Total);
}

void TranslationWriter::finialize()
{
    while(this->PendingTranslations.size())
        this->writeOutputLines(this->PendingTranslations.values(this->PendingTranslations.firstKey()));
}

void TranslationWriter::writeTranslation(quint64 _index, const QString &_translation)
{
    QMutexLocker Locker(&this->OutputListLock);

    if (_index == this->LastSavedIndex + 1){
        this->writeOutputLines(QList<QString>() << _translation);
    }else{
        this->PendingTranslations.insert(_index, _translation);
    }

    while(this->PendingTranslations.size() &&
          this->PendingTranslations.firstKey() == this->LastSavedIndex + 1){
        this->writeOutputLines(this->PendingTranslations.values(this->PendingTranslations.firstKey()));
    }
}

void TranslationWriter::writeNBestPaths(quint64 _index, const SMT::stuTranslationOutput &_translationOutput)
{
    QMutexLocker Locker(&this->OutputListLock);

    if(_translationOutput.Translations.size() != _translationOutput.TranslationsCostElements.size()) {
        TargomanLogError("Translation output error: Number of translations does not match the number of cost element descriptors");
        // TODO: What to do, what not to do?!
        return;
    }

    QList<QString> PathStrings;
    for(int PathIndex = 0; PathIndex < _translationOutput.Translations.size(); ++PathIndex) {
        PathStrings << this->getNBestPathString(_translationOutput.Translations.at(PathIndex),
                                             _translationOutput.TranslationsCostElements.at(PathIndex));
    }
    if (_index == this->LastSavedIndex + 1) {
        this->writeOutputLines(PathStrings);
    }else{
        foreach(const QString& PathString, PathStrings)
        this->PendingTranslations.insert(_index, PathString);
    }

    while(this->PendingTranslations.size() &&
          this->PendingTranslations.firstKey() == this->LastSavedIndex + 1){
        this->writeOutputLines(this->PendingTranslations.values(this->PendingTranslations.firstKey()));
    }

}

}
}
