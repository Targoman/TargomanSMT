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
 */

#ifndef TARGOMAN_APPS_TRANSLATIONWRITER_H
#define TARGOMAN_APPS_TRANSLATIONWRITER_H

#include <QMutex>
#include <QMap>


namespace Targoman {
namespace Apps {

class TranslationWriter
{
public:
    static TranslationWriter& instance(){
        return Q_LIKELY(TranslationWriter::Instance) ?
                    * TranslationWriter::Instance :
                    *(TranslationWriter::Instance = new TranslationWriter);
    }
    void writeTranslation(quint64 _index, const QString& _translation);
    bool isFinished(){ return this->PendingTranslations.isEmpty(); }

private:
    TranslationWriter();
    void writeTranslation(const QString& _translation);

private:
    QMutex                  OutputListLock;
    QMap<quint64, QString>  PendingTranslations;
    quint64                 LastInsertedIndex = 0;
    static TranslationWriter*      Instance;
};

}
}

#endif // TARGOMAN_APPS_TRANSLATIONWRITER_H
