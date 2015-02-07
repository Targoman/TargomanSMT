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

#ifndef TARGOMAN_NLPLIBS_PRIVATE_ARPAMANAGER_H
#define TARGOMAN_NLPLIBS_PRIVATE_ARPAMANAGER_H

#include "../Definitions.h"
#include "intfBaseModel.hpp"

namespace Targoman {
namespace NLPLibs {
namespace Private {

TARGOMAN_ADD_EXCEPTION_HANDLER(exARPAManager, exLanguageModel);

/**
 * @brief Main functionality of this class is for loading language model ARPA files.
 */
class ARPAManager
{
public:
    static inline ARPAManager& instance(){return *(Q_LIKELY(Instance) ? Instance : (Instance = new ARPAManager));}

    quint8 load(const QString&  _file, intfBaseModel* _model);

private:
    ARPAManager();
    Q_DISABLE_COPY(ARPAManager)

private:
    static ARPAManager* Instance; /**< A static pointer member of this class.*/
};

}
}
}

#endif // TARGOMAN_NLPLIBS_PRIVATE_ARPAMANAGER_H
