/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 @author Behrooz Vedadian <vedadian@gmail.com>
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
