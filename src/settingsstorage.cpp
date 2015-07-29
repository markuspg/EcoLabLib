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

#include "settingsstorage.h"

ellSettingsStorage::ellSettingsStorage( const ellBuilder &argBuilder, QObject *argParent ) :
    QObject{ argParent },
    adminUsers{ argBuilder.adminUsers },
    browserCommand{ argBuilder.browserCommand },
    defaultReceiptIndex{ argBuilder.defaultReceiptIndex },
    dvipsCommand{ argBuilder.dvipsCommand },
    ecolablibInstallationDirectory{ argBuilder.ecolablibInstallationDirectory },
    fileManager{ argBuilder.fileManager },
    initialzTreePort{ argBuilder.initialzTreePort },
    installedLaTeXHeaders{ argBuilder.installedLaTeXHeaders },
    installedzTreeVersions{ argBuilder.installedzTreeVersions },
    killallCommand{ argBuilder.killallCommand },
    latexCommand{ argBuilder.latexCommand },
    lprCommand{ argBuilder.lprCommand },
    netstatCommand{ argBuilder.netstatCommand },
    networkBroadcastAddress{ argBuilder.networkBroadcastAddress },
    orseeURL{ argBuilder.orseeURL },
    pingCommand{ argBuilder.pingCommand },
    postscriptViewer{ argBuilder.postscriptViewer },
    processEnvironment{ argBuilder.processEnvironment },
    ps2pdfCommand{ argBuilder.ps2pdfCommand },
    publicKeyPathRoot{ argBuilder.publicKeyPathRoot },
    publicKeyPathUser{ argBuilder.publicKeyPathUser },
    rcpCommand{ argBuilder.rcpCommand },
    rmCommand{ argBuilder.rmCommand },
    serverIP{ argBuilder.serverIP },
    serverPort{ argBuilder.serverPort },
    sshCommand{ argBuilder.sshCommand },
    terminalEmulatorCommand{ argBuilder.terminalEmulatorCommand },
    userNameOnClients{ argBuilder.userNameOnClients },
    vncViewer{ argBuilder.vncViewer },
    wakeonlanCommand{ argBuilder.wakeonlanCommand },
    webcamDisplayCommand{ argBuilder.webcamDisplayCommand },
    webcamNames{ argBuilder.webcamNames },
    webcamURLs{ argBuilder.webcamURLs },
    wineCommand{ argBuilder.wineCommand },
    wmctrlCommand{ argBuilder.wmctrlCommand },
    xsetCommand{ argBuilder.xsetCommand },
    zTreeInstallationDirectory{ argBuilder.zTreeInstallationDirectory },
    localzLeafName{ argBuilder.localzLeafName }
{
}

ellSettingsStorage::~ellSettingsStorage() {
    delete adminUsers;
    delete browserCommand;
    delete defaultReceiptIndex;
    delete dvipsCommand;
    delete ecolablibInstallationDirectory;
    delete fileManager;
    delete initialzTreePort;
    delete installedLaTeXHeaders;
    delete installedzTreeVersions;
    delete killallCommand;
    delete latexCommand;
    delete localzLeafName;
    delete lprCommand;
    delete netstatCommand;
    delete networkBroadcastAddress;
    delete orseeURL;
    delete pingCommand;
    delete postscriptViewer;
    delete processEnvironment;
    delete ps2pdfCommand;
    delete publicKeyPathRoot;
    delete publicKeyPathUser;
    delete rcpCommand;
    delete rmCommand;
    delete serverIP;
    delete serverPort;
    delete sshCommand;
    delete terminalEmulatorCommand;
    delete userNameOnClients;
    delete vncViewer;
    delete wakeonlanCommand;
    delete webcamDisplayCommand;
    delete webcamNames;
    delete wineCommand;
    delete webcamURLs;
    delete wmctrlCommand;
    delete xsetCommand;
    delete zTreeInstallationDirectory;
}
