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
 @author Behrooz Vedadian <vedadian@gmail.com>
 */

#include "clsTargetRule.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace PhraseTable{

using namespace Common;

clsTargetRule::clsTargetRule()
{
}

Common::Cost_t clsTargetRule::getPhraseCost()
{
    if (!this->AllCostsComputed){
        for (int i=0; i<this->Costs.size(); ++i){
            //this->Costs.at(i) *

            //TODO multiply each Cost with its corresponding scaling factor. which must be loaded from config file
        }

        AllCostsComputed = true;
    }

    return this->AllCosts
}

}
}
}
}
