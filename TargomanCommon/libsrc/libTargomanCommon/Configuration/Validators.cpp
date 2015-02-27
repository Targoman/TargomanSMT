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

#include "Validators.h"
#include "intfConfigurable.hpp"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Validators{

/***************************************************************************************/
clsPathValidator::clsPathValidator(PathAccess::Options _requiredAccess):
    RequiredAccess(_requiredAccess)
{}

bool clsPathValidator::validate(const intfConfigurable &_item, QString &_errorMessage)
{
    QString Path = _item.toVariant().toString();
    QFileInfo PathInfo(Path);

    if (this->RequiredAccess.testFlag(PathAccess::Dir) && PathInfo.isDir() == false){
        _errorMessage = _item.configPath() + ": <"+Path+"> must be a directory";
        return false;
    }else if (this->RequiredAccess.testFlag(PathAccess::File) && PathInfo.isFile() == false){
        _errorMessage = _item.configPath() + ": <"+Path+"> must be a file";
        return false;
    }
    if (this->RequiredAccess.testFlag(PathAccess::Executable) && PathInfo.isExecutable() == false){
        _errorMessage = _item.configPath() + ": <"+Path+"> must be executable";
        return false;
    }

    if (this->RequiredAccess.testFlag(PathAccess::Readable) && PathInfo.isReadable() == false){
        _errorMessage = _item.configPath() + ": Unable to open <"+Path+"> for READING";
        return false;
    }
    if (this->RequiredAccess.testFlag(PathAccess::Writeatble) && PathInfo.isWritable() == false){
        _errorMessage = _item.configPath() + ": Unable to open <"+Path+"> for WRITING";
        return false;
    }
    _errorMessage.clear();
    return true;
}
///////////////////////////////////////////////////////////////////////
clsIntValidator::clsIntValidator(qint64 _min, qint64 _max):
    Max(_max),Min(_min)
{}

bool clsIntValidator::validate(const intfConfigurable &_item, QString &_errorMessage)
{
    qint64 Number = _item.toVariant().toLongLong();
    if (Number < this->Min || Number > this->Max){
        _errorMessage = _item.configPath() + QString(": must be between %1 - %2").arg(this->Min).arg(this->Max);
        return false;
    }
    _errorMessage.clear();
    return true;
}

///////////////////////////////////////////////////////////////////////
clsUIntValidator::clsUIntValidator(quint64 _min, quint64 _max):
    Max(_max),Min(_min)
{}

bool clsUIntValidator::validate(const intfConfigurable &_item, QString &_errorMessage)
{
    quint64 Number = _item.toVariant().toULongLong();
    if (Number < this->Min || Number > this->Max){
        _errorMessage = _item.configPath() + QString(": must be between %1 - %2").arg(this->Min).arg(this->Max);
        return false;
    }
    _errorMessage.clear();
    return true;
}

///////////////////////////////////////////////////////////////////////
clsDoubleValidator::clsDoubleValidator(double _min, double _max):
    Max(_max),Min(_min)
{}

bool clsDoubleValidator::validate(const intfConfigurable &_item, QString &_errorMessage)
{
    double Number = _item.toVariant().toDouble();
    if (Number < this->Min || Number > this->Max){
        _errorMessage = _item.configPath() + QString(": must be between %1 - %2").arg(this->Min).arg(this->Max);
        return false;
    }
    _errorMessage.clear();
    return true;
}

}
}
}
}
