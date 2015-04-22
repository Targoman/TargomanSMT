/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 * Originally based on Qt Torrent example.
 * http://doc.qt.digia.com/qt-maemo/network-torrent.html
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_COMMON_NETWORK_INTFTCPSERVERSECURITY_HPP
#define TARGOMAN_COMMON_NETWORK_INTFTCPSERVERSECURITY_HPP

#include <QString>

namespace Targoman{
namespace Common{
namespace Network{
namespace Private{

class intfTCPServerSecurity
{
public:
    virtual bool isConnectionAllowed(const QString& _ip, qint32 _port) = 0;
    virtual ~intfTCPServerSecurity(){}
};

}
}
}
}
#endif // TARGOMAN_COMMON_NETWORK_INTFTCPSERVERSECURITY_HPP
