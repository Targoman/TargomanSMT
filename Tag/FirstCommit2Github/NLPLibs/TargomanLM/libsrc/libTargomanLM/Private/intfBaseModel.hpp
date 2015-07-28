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

#ifndef TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_INTFBASEMODEL_HPP
#define TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_INTFBASEMODEL_HPP

#include "../Definitions.h"

namespace Targoman {
namespace NLPLibs {
namespace TargomanLM {
namespace Private {

class intfBaseModel
{
public:
    intfBaseModel(enuMemoryModel::Type _type) : Type(_type){}
    virtual ~intfBaseModel(){}

    virtual void init(quint32 _maxNGramCount) = 0;
    virtual void setUnknownWordDefaults(Targoman::Common::LogP_t _prob, Targoman::Common::LogP_t _backoff)=0;
    virtual void insert(const char* _ngram, quint8 _order, float _prob, float _backoff) = 0;
    virtual Targoman::Common::LogP_t lookupNGram(const QStringList &_ngram, quint8& _foundedGram) const = 0;
    virtual Targoman::Common::LogP_t lookupNGram(const QList<Common::WordIndex_t> &_ngram, quint8& _foundedGram) const = 0;
    virtual QString getStatsStr() const = 0;
    virtual quint64 getID(const char* _word) const = 0;
    virtual QString getWordByID(Common::WordIndex_t _wordIndex) const = 0;
    virtual void    saveBinFile(const QString& _binFilePath, quint8 _order) = 0;
    virtual quint8  loadBinFile(const QString& _binFilePath, bool _computeChecksum = true) = 0;

protected:
    enuMemoryModel::Type  Type;
};

}
}
}
}

#endif // TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_INTFBASEMODEL_HPP
