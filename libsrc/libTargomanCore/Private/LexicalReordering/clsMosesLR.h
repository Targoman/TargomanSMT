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

#ifndef TARGOMAN_CORE_PRIVATE_LEXICALREORDERING_CLSLEXICALREORDERING_H
#define TARGOMAN_CORE_PRIVATE_LEXICALREORDERING_CLSLEXICALREORDERING_H

#include "intfLexicalReordering.h"

namespace Targoman {
namespace Core {
namespace Private {
namespace LexicalReordering {

class clsMosesLR
{
public:
    clsMosesLR();

    static void registerConfigs(const QString& _configPath = ""){
        //intfLexicalReordering::registerConfigs(_configPath + "/MosesLR")
    }

private:
    QString FilePath;
    size_t  TableLimit;
};

}
}
}
}

#endif // TARGOMAN_CORE_PRIVATE_LEXICALREORDERING_CLSLEXICALREORDERING_H
