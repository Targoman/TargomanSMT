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

#ifndef TARGOMAN_APPS_TRANSLATIONWRITER_H
#define TARGOMAN_APPS_TRANSLATIONWRITER_H

#include <QMutex>
#include <QMap>
#include <libTargomanSMT/Types.h>

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
    void writeNBestPaths(quint64 _index, const SMT::stuTranslationOutput& _translationOutput);
    void finialize();

private:
    TranslationWriter();
    void writeOutputLines(const QList<QString>& _outputLines);
    QString getNBestPathString(const QString &_translation, const SMT::stuTranslationOutput::stuCostElements &_translationOutput);

private:
    QMutex                  OutputListLock;
    QMultiMap<quint64, QString>  PendingTranslations;
    quint64                 LastSavedIndex = 0;
};

}
}

#endif // TARGOMAN_APPS_TRANSLATIONWRITER_H
