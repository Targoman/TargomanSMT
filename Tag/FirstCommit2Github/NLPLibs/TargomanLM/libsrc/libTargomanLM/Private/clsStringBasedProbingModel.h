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

#ifndef TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_CLSSTRINGBASEDPROBINGMODEL_H
#define TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_CLSSTRINGBASEDPROBINGMODEL_H

#include "clsAbstractProbingModel.h"

namespace Targoman {
namespace NLPLibs {
namespace TargomanLM {
namespace Private {

class clsStringBasedProbingModel : public clsAbstractProbingModel
{
public:
    clsStringBasedProbingModel(){;}
    Common::LogP_t lookupNGram(const QStringList &_ngram, quint8 &_foundedGram) const;
    Common::LogP_t lookupNGram(const QList<Common::WordIndex_t> &_ngram, quint8 &_foundedGram) const
    {
        Q_UNUSED(_ngram)
        Q_UNUSED(_foundedGram)
        throw Common::exTargomanNotImplemented("NgramLookup by WordIndex not implemented yet");
    }
    inline QString modelHeaderSuffix() {return QString("-Probing-v1.0-StringBased");}
};

}
}
}
}

#endif // TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_CLSSTRINGBASEDPROBINGMODEL_H
