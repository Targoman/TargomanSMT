/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of both GPL vrsion 3 and
 * TCRL(Targoman Community Research License)
 * You can find a copy of TCRL license file with distributed source or
 * download them from http://targoman.com/License.txt
 *
 * This file is part of KenLM implementation in Targoman Project.
 *
 *   KenLM is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   KenLM is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with KenLM.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   KenLM code is derived from Kenneth's code.
 *   Find origianl code at: https://kheafield.com/code/kenlm/
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_NLPLIBS_LM_KENLM_PRIVATE_BINARYMANAGER_H
#define TARGOMAN_NLPLIBS_LM_KENLM_PRIVATE_BINARYMANAGER_H

#include <vector>
#include "../BinaryModelTypes.h"
#include "../Configs.h"

namespace Targoman {
namespace NLPLibs {
namespace LM {
namespace Private {


struct FixedWidthParameters {
  unsigned char order;
  float probing_multiplier;
  // What type of model is this?
  enuBinaryModel::Type model_type;
  // Does the end of the file have the actual strings in the vocabulary?
  bool has_vocabulary;
  unsigned int search_version;
};

// Parameters stored in the header of a binary file.
struct Parameters {
  FixedWidthParameters fixed;
  std::vector<quint64> counts;
};

// This is a macro instead of an inline function so constants can be assigned using it.
#define ALIGN8(a) ((std::ptrdiff_t(((a)-1)/8)+1)*8)


class clsBinaryFormat
{
public:
    clsBinaryFormat();
};

}
}
}
}

#endif // TARGOMAN_NLPLIBS_LM_KENLM_PRIVATE_BINARYMANAGER_H
