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
 */

#ifndef TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_ARPAMANAGER_H
#define TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_ARPAMANAGER_H

#include "../Definitions.h"
#include "intfBaseModel.hpp"

namespace Targoman {
namespace NLPLibs {
namespace TargomanLM {
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exARPAManager, exLanguageModel);

/**
 * @brief Main functionality of this class is for loading language model ARPA files.
 */
class ARPAManager
{
public:
    static inline ARPAManager& instance(){return *(Q_LIKELY(Instance) ? Instance : (Instance = new ARPAManager));}

    quint8 load(const QString&  _file, intfBaseModel &_model, bool _justVocab = false, bool _skipVocab = false);

private:
    ARPAManager();
    Q_DISABLE_COPY(ARPAManager)

private:
    static ARPAManager* Instance; /**< A static pointer member of this class.*/
};

}
}
}
}

#endif // TARGOMAN_NLPLIBS_LANGUAGEMODEL_PRIVATE_ARPAMANAGER_H
