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
 * @author Behrooz Vedadian <vedadian@targoman.com>
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
