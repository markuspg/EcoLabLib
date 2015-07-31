/*
 * Copyright 2015 Markus Prasser
 *
 * This file is part of EcoLabLib.
 *
 *  EcoLabLib is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  EcoLabLib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with EcoLabLib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SETTINGSSTORAGE_H
#define SETTINGSSTORAGE_H

#include "builder.h"

#include <QObject>

#include <memory>

class ellSettingsStorage : public QObject
{
    Q_OBJECT
public:
    explicit ellSettingsStorage( const ellBuilder &argBuilder, QObject *argParent = nullptr );
    ellSettingsStorage( const ellSettingsStorage &argSettingsStorage ) = delete;
    ~ellSettingsStorage();

    const std::unique_ptr< const QStringList > adminUsers = nullptr;
    const std::unique_ptr< const QString > browserCommand = nullptr;
    const std::unique_ptr< const quint16 > defaultReceiptIndex = nullptr;
    const std::unique_ptr< const QString > dvipsCommand = nullptr;
    const std::unique_ptr< const QString > ecolablibInstallationDirectory = nullptr;
    const std::unique_ptr< const QString > fileManager = nullptr;
    const std::unique_ptr< const quint16 > initialzTreePort = nullptr;
    const std::unique_ptr< const QStringList > installedLaTeXHeaders = nullptr;
    const std::unique_ptr< const QStringList > installedzTreeVersions = nullptr;
    const std::unique_ptr< const QString > killallCommand = nullptr;
    const std::unique_ptr< const QString > latexCommand = nullptr;
    const std::unique_ptr< const QString > lprCommand = nullptr;
    const std::unique_ptr< const QString > networkBroadcastAddress = nullptr;
    const std::unique_ptr< const QString > orseeURL = nullptr;
    const std::unique_ptr< const QString > postscriptViewer = nullptr;
    const std::unique_ptr< const QProcessEnvironment > processEnvironment = nullptr;
    const std::unique_ptr< const QString > ps2pdfCommand = nullptr;
    const std::unique_ptr< const QString > publicKeyPathRoot = nullptr;
    const std::unique_ptr< const QString > publicKeyPathUser = nullptr;
    const std::unique_ptr< const QString > rcpCommand = nullptr;
    const std::unique_ptr< const QString > rmCommand = nullptr;
    const std::unique_ptr< const QString > serverIP = nullptr;
    const std::unique_ptr< const quint16 > serverPort = nullptr;
    const std::unique_ptr< const QString > sshCommand = nullptr;
    const std::unique_ptr< const QString > terminalEmulatorCommand = nullptr;
    const std::unique_ptr< const QString > userNameOnClients = nullptr;
    const std::unique_ptr< const QString > vncViewer = nullptr;
    const std::unique_ptr< const QString > wakeonlanCommand = nullptr;
    const std::unique_ptr< const QString > webcamDisplayCommand = nullptr;
    const std::unique_ptr< const QStringList > webcamNames = nullptr;
    const std::unique_ptr< const QStringList > webcamURLs = nullptr;
    const std::unique_ptr< const QString > wineCommand = nullptr;
    const std::unique_ptr< const QString > wmctrlCommand = nullptr;
    const std::unique_ptr< const QString > xsetCommand = nullptr;
    const std::unique_ptr< const QString > zTreeInstallationDirectory = nullptr;

    QString *GetLocalzLeafName() const { return localzLeafName; }
    void SetLocalzLeafName( QString *argLocalzLeafName ) { localzLeafName = argLocalzLeafName; }

signals:

public slots:

private:
    QString *localzLeafName = nullptr;
};

#endif // SETTINGSSTORAGE_H
