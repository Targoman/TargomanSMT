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
 * @author Behrooz Vedadian <vedadian@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#ifndef TARGOMAN_NLPLIBS_LANGUAGEMODEL_CLSLANGUAGEMODEL_H
#define TARGOMAN_NLPLIBS_LANGUAGEMODEL_CLSLANGUAGEMODEL_H

#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanCommon/Macros.h"
#include "libTargomanLM/Definitions.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman {
namespace NLPLibs {
namespace TargomanLM {

namespace Private {
    class clsLanguageModelPrivate;
}
/**
 * @brief This class manages loaded language model.
 */
class clsLanguageModel
{
public:
    clsLanguageModel();
    ~clsLanguageModel();

    quint8 init(bool _justVocab = false);

    quint8 init(const QString& _filePath, const stuLMConfigs& _configs = stuLMConfigs(), bool _justVocab = false);

    void convertBinary(const QString& _binFilePath);

    quint8  order() const;
    Common::WordIndex_t getID(const QString& _word) const;
    QString getWordByID(Common::WordIndex_t _wordIndex) const;

    Common::LogP_t lookupNGram(const QStringList & _ngram, quint8& _foundedGram) const;
    Common::LogP_t lookupNGram(const QList<Common::WordIndex_t> &_ngram, quint8& _foundedGram) const ;


private:
    QScopedPointer<Private::clsLanguageModelPrivate> pPrivate;

    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t> FilePath;
    static Targoman::Common::Configuration::tmplConfigurable<double>  DeafultUnknownProb;
    static Targoman::Common::Configuration::tmplConfigurable<double>  DeafultUnknownBackoff;
    static Targoman::Common::Configuration::tmplConfigurable<bool>    UseIndexBasedModel;
    static Targoman::Common::Configuration::tmplConfigurable<bool>    VerifyBinaryChecksum;
};


}
}
}

#endif // TARGOMAN_NLPLIBS_LANGUAGEMODEL_CLSLANGUAGEMODEL_H
