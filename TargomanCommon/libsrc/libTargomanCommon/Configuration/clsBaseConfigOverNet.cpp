/*******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework                *
 *                                                                             *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                           *
 *                                                                             *
 * This file is part of Targoman.                                              *
 *                                                                             *
 * Targoman is free software: you can redistribute it and/or modify            *
 * it under the terms of the GNU Lesser General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * Targoman is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                             *
 *******************************************************************************/
/**
 @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 @author Behrooz Vedadian <vedadian@targoman.com>
 */

#include "Private/clsBaseConfigOverNet.h"

namespace Targoman {
namespace Common {
namespace Configuration {
namespace Private {

clsBaseConfigOverNet::clsBaseConfigOverNet(clsConfigManagerPrivate &_configManager) :
    ConfigManagerPrivate(_configManager),
    ActorUUID(_configManager.ActorUUID)
{
}

QVariantList clsBaseConfigOverNet::walk(bool _showDetails)
{
    if (this->AllowedToView == false)
        throw exNoLogin("Invalid Request. Please login first");

    QVariantList TableRows;
    QVariantList Table;

    if (_showDetails)
        TableRows.insert(
                    TableRows.end(),
                    QVariantList()<<"Path"<<"Value"<<"Type"<<"Updatable"<<"Options");
    else
        TableRows.insert(
                    TableRows.end(),
                    QVariantList()<<"Path"<<"Value");

    Table.append(TableRows);

    foreach(const intfConfigurable* Item, this->ConfigManagerPrivate.Configs.values()){
        TableRows.clear();
        if (Item->remoteView() == false)
            continue;

        if (_showDetails)
            TableRows.insert(TableRows.end(),
                             QVariantList()<<
                             Item->configPath()<<
                             Item->toVariant().toString()<<
                             Item->typeString()<<
                             (Item->canBeConfigured(enuConfigSource::Net) &&
                              this->AllowedToChange)<<
                             Item->validValues()
                             );
        else
            TableRows.insert(TableRows.end(),
                             QVariantList()<<
                             Item->configPath()<<
                             Item->toVariant().toString()
                             );

        Table.append(TableRows);
    }
    return Table;
}

QVariant clsBaseConfigOverNet::query(const QString& _path)
{
    if (this->AllowedToView == false)
        throw exNoLogin("Invalid Request. Please login first");

    intfConfigurable* ConfigItem = this->ConfigManagerPrivate.Configs.value(_path, NULL);
    if (ConfigItem)
        return ConfigItem->toVariant();
    else
        throw exObjectNotFound(_path);
}

QVariantList clsBaseConfigOverNet::bulkQuery(const QString& _parentPath,
                                            bool _isRegex,
                                            bool _showDetails,
                                            bool _justUpdatable,
                                            const QString& _justType,
                                            const QString& _stripString)
{
    if (this->AllowedToView == false)
        throw exNoLogin("Invalid Request. Please login first");

    QString ParenPath = _parentPath;

    if (ParenPath.endsWith('/') == false)
        ParenPath += _isRegex ? "(\\/|$)" : "/";

    ParenPath.replace(QRegExp("//*"),"/");

    QList<intfConfigurable*> ConfigItems =
            this->ConfigManagerPrivate.configItems(ParenPath, _isRegex, false);

    if (ConfigItems.isEmpty())
        throw exObjectNotFound(ParenPath);

    QVariantList TableRows;
    QVariantList Table;

    if (_showDetails)
        TableRows.insert(
                    TableRows.end(),
                    QVariantList()<<"Path"<<"Value"<<"Type"<<"Updatable"<<"Options");
    else
        TableRows.insert(
                    TableRows.end(),
                    QVariantList()<<"Path"<<"Value");

    Table.append(TableRows);

    foreach(intfConfigurable* Item, ConfigItems) {
        if (Item->remoteView() == false
                || (_justUpdatable != -2
                    && (Item->canBeConfigured(enuConfigSource::Net) &&
                        this->AllowedToChange) != _justUpdatable)
                || (_justType.size() &&
                    _justType != Item->typeString()))
            continue;

        TableRows.clear();

        QString Path = Item->configPath();
        if (Path.startsWith(_stripString))
            Path = Path.mid(_stripString.size());

        if (_showDetails)
            TableRows.insert(TableRows.end(),
                             QVariantList()<<
                             Item->configPath()<<
                             Item->toVariant().toString()<<
                             Item->typeString()<<
                             (Item->canBeConfigured(enuConfigSource::Net) &&
                              this->AllowedToChange)<<
                             Item->validValues()
                             );
        else
            TableRows.insert(TableRows.end(),
                             QVariantList()<<
                             Item->configPath()<<
                             Item->toVariant().toString()
                             );
        Table.append(TableRows);
    }
    return Table;
}

QVariant clsBaseConfigOverNet::set(const QString& _path, const QVariant& _newValue)
{
    if (this->AllowedToView == false)
        throw exNoLogin("Invalid Request. Please login first");

    if (this->AllowedToChange == false)
        throw exInvalidAction("Not Allowed to set");

    if (_path.isEmpty())
        throw exObjectNotFound(_path);

    if (_newValue.isNull() == false) {
        intfConfigurable* ConfigItem =
                this->ConfigManagerPrivate.Configs.value(_path, NULL);
        if (ConfigItem){
            if (ConfigItem->remoteView() == false ||
                    ConfigItem->canBeConfigured(enuConfigSource::Net) == false)
                throw exInvalidUpdateSource(_path);

            QString ErrorMessage;
            QVariant OldValue = ConfigItem->toVariant();
            try{
                ConfigItem->setFromVariant(_newValue);
                if (ConfigItem->crossValidate(ErrorMessage) == false){
                    ConfigItem->setFromVariant(OldValue);
                    throw exInvalidData(ErrorMessage + " On: " + _path);
                }else{
                    TargomanLogInfo(5, QString("User: %1 Changed value of %2 to %4").arg(
                                        this->ActorName).arg(
                                        _path).arg(
                                        _newValue.toString()));
                    return _newValue;
                }
            }catch(exConfiguration &e){
                throw exInvalidData(e.what().split(">;ex").first() + " On: " + _path);
            }
        }else
            throw exObjectNotFound(_path);
    } else
        throw exInvalidAction("Unimplemented Set to NULL Command on: " + _path);
}

stuPong clsBaseConfigOverNet::ssidPing(const QString& _ssid)
{
    stuPong Pong;
    emit this->ConfigManagerPrivate.Parent.sigPing(_ssid, Pong);
    return Pong;
}

}
}
}
}
