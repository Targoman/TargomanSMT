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

#ifndef TARGOMAN_APPS_XMLREADER_H
#define TARGOMAN_APPS_XMLREADER_H

#include <QStringList>
#include <libxml/xmlreader.h>
#include "Configs.h"

namespace Targoman {
namespace Apps {

TARGOMAN_ADD_EXCEPTION_HANDLER(exInvalidXML, exAppE4SMT);

class XMLReader
{
public:
  static bool isValid(const QString& _inFile);
  static QStringList getContext(const QString& _inFile, bool _keepTitles);

private:
  static void xmlErrorHandler(void* _userData, xmlErrorPtr _error);
};

}
}

#endif // TARGOMAN_APPS_XMLREADER_H
