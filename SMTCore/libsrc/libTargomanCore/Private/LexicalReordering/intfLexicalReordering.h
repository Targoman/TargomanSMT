/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 * 
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_CORE_PRIVATE_LEXICALREORDERING_INTFLEXICALREORDERING_H
#define TARGOMAN_CORE_PRIVATE_LEXICALREORDERING_INTFLEXICALREORDERING_H

#include "GlobalConfigs.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace LexicalReordering {


TARGOMAN_ADD_EXCEPTION_HANDLER(exPhraseTable, exTargomanCore);

class intfLexicalReordering
{
    virtual void init(const QString& _configPath){
        //this->FilePath   = Targoman::Core::gConfigs.getConfig(_configPath + "/FilePath").toString();
    }

    virtual void registerConfigs(const QString& _configPath){
        //Targoman::Core::gConfigs.addConfig(_configPath + "/FilePath"  , QVariant::String, "");
    }

private:
    QString ConfigPath;
};
}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_LEXICALREORDERING_INTFLEXICALREORDERING_H
