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

#ifndef TARGOMAN_COMMON_CLSSAFECOREAPPLICATION_H
#define TARGOMAN_COMMON_CLSSAFECOREAPPLICATION_H

#include <QCoreApplication>

namespace Targoman{
namespace Common {

class clsSafeCoreApplication : public QCoreApplication
{
    Q_OBJECT
public:
    explicit clsSafeCoreApplication(int& _argc, char ** _argv);
    bool notify(QObject * _object, QEvent * _ev);
};

}
}
#endif // TARGOMAN_COMMON_CLSSAFECOREAPPLICATION_H
