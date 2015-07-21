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


#ifndef TARGOMAN_APPS_CLSTRANSLATIONJOB_H
#define TARGOMAN_APPS_CLSTRANSLATIONJOB_H

#include <QVariantList>
#include "libTargomanSMT/Translator.h"

namespace Targoman {
namespace Apps {

class clsTranslationJob
{
public:
    clsTranslationJob(bool _brief, bool _keepAsSource);
    QVariantList doJob(const QString& _inputStr);

private:
    SMT::stuTranslationOutput mapLineTranslation(const QString& _line);
    SMT::stuTranslationOutput mapSentenceTranslation(const QString& _ixml);
    void reduceLineTranslation(QVariantList &_result, const SMT::stuTranslationOutput& _intermediate);
    void reduceSentenceTranslation(SMT::stuTranslationOutput& _result, const SMT::stuTranslationOutput& _intermediate);
private:
    bool Brief;
    bool KeepAsSource;

public:
    static QString SourceLanguage; // Just for speed optimization
};

}
}

#endif // TARGOMAN_APPS_CLSTRANSLATIONJOB_H
