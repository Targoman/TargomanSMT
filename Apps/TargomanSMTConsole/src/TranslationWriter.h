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
        static TranslationWriter*      Instance = NULL;

        return Q_LIKELY(Instance) ?
                    * Instance :
                    *(Instance = new TranslationWriter);
    }
    void writeTranslation(quint64 _index, const QString& _translation);
    void finialize();

private:
    TranslationWriter();
    void writeTranslation(const QString& _translation);

private:
    QMutex                  OutputListLock;
    QMap<quint64, QString>  PendingTranslations;
    quint64                 LastSavedIndex = 0;
};

}
}

#endif // TARGOMAN_APPS_TRANSLATIONWRITER_H
