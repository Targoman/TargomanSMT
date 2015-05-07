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

#ifndef TARGOMAN_APPS_CLSTRANSLATIONJOB_H
#define TARGOMAN_APPS_CLSTRANSLATIONJOB_H

#include <QRunnable>
#include <QString>

namespace Targoman {
namespace Apps {

class clsTranslationJob : public QRunnable
{
public:
    clsTranslationJob(quint64 _index, const QString &_sourceString);
    void run();

private:
    quint64 Index;
    QString SourceString;
};

}
}

#endif // TARGOMAN_APPS_CLSTRANSLATIONJOB_H
