/*
 * Copyright 2015-2016 Markus Prasser
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

#include "settingsstorage.h"

ell::SettingsStorage::SettingsStorage( const Builder &argBuilder, QObject *argParent ) :
    QObject{ argParent },
    adminUsers{ argBuilder.adminUsers },
    browserCommand{ argBuilder.browserCommand },
    certFile{ argBuilder.certFile },
    clientConnectionPassword{ argBuilder.clientConnectionPassword },
    defaultReceiptIndex{ argBuilder.defaultReceiptIndex },
    dvipsCommand{ argBuilder.dvipsCommand },
    ecolablibInstallationDirectory{ argBuilder.ecolablibInstallationDirectory },
    fileManager{ argBuilder.fileManager },
    globalListening{ argBuilder.globalListening },
    initialzTreePort{ argBuilder.initialzTreePort },
    installedLaTeXHeaders{ argBuilder.installedLaTeXHeaders },
    installedzTreeVersions{ argBuilder.installedzTreeVersions },
    keyFile{ argBuilder.keyFile },
    killallCommand{ argBuilder.killallCommand },
    latexCommand{ argBuilder.latexCommand },
    lprCommand{ argBuilder.lprCommand },
    networkBroadcastAddress{ argBuilder.networkBroadcastAddress },
    notProperlySetVariables{ argBuilder.notProperlySetVariables },
    orseeURL{ argBuilder.orseeURL },
    postscriptViewer{ argBuilder.postscriptViewer },
    processEnvironment{ argBuilder.processEnvironment },
    ps2pdfCommand{ argBuilder.ps2pdfCommand },
    publicKeyPathRoot{ argBuilder.publicKeyPathRoot },
    publicKeyPathUser{ argBuilder.publicKeyPathUser },
    rmCommand{ argBuilder.rmCommand },
    scpCommand{ argBuilder.scpCommand },
    serverIP{ argBuilder.serverIP },
    serverPort{ argBuilder.serverPort },
    sshCommand{ argBuilder.sshCommand },
    terminalEmulatorCommand{ argBuilder.terminalEmulatorCommand },
    useSSH{ argBuilder.useSSH },
    userNameOnClients{ argBuilder.userNameOnClients },
    vncViewer{ argBuilder.vncViewer },
    wakeonlanCommand{ argBuilder.wakeonlanCommand },
    webcamDisplayCommand{ argBuilder.webcamDisplayCommand },
    webcamNames{ argBuilder.webcamNames },
    webcamURLs{ argBuilder.webcamURLs },
    wineCommand{ argBuilder.wineCommand },
    wmctrlCommand{ argBuilder.wmctrlCommand },
    xsetCommand{ argBuilder.xsetCommand },
    zTreeInstallationDirectory{ argBuilder.zTreeInstallationDirectory }
{
}

ell::SettingsStorage::~SettingsStorage() {
    delete notProperlySetVariables;
}
