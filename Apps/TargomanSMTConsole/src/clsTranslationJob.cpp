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

#include "clsTranslationJob.h"
#include "libTargomanSMT/Translator.h"
#include "TranslationWriter.h"
#include "Configs.h"

namespace Targoman {
namespace Apps {

using namespace SMT;

clsTranslationJob::clsTranslationJob(quint64 _index, const QString& _sourceString)
{
    this->SourceString = _sourceString;
    this->Index = _index;
}

void clsTranslationJob::run()
{
    TranslationWriter::instance().writeTranslation(
                this->Index,
                Translator::translate(this->SourceString,true, gConfigs::NBestFile.value().size() > 0));
//    TranslationWriter::instance().writeTranslation(
//                this->Index,
//                Translator::translate(this->SourceString,true).Translation);
//    Translator::printNBestPath(this->SourceString, this->Index);
}

}
}
