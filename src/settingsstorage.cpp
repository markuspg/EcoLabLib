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
    latexCommand{ argBuilder.latexCommand },
    lprCommand{ argBuilder.lprCommand },
    netstatCommand{ argBuilder.netstatCommand },
    networkBroadcastAddress{ argBuilder.networkBroadcastAddress },
    orseeURL{ argBuilder.orseeURL },
    pingCommand{ argBuilder.pingCommand },
    postscriptViewer{ argBuilder.postscriptViewer },
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
    webcamNames{ argBuilder.webcamNames },
    webcamURLs{ argBuilder.webcamURLs },
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
    delete latexCommand;
    delete localzLeafName;
    delete lprCommand;
    delete netstatCommand;
    delete networkBroadcastAddress;
    delete orseeURL;
    delete pingCommand;
    delete postscriptViewer;
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
    delete webcamNames;
    delete webcamURLs;
    delete wmctrlCommand;
    delete xsetCommand;
    delete zTreeInstallationDirectory;
}
