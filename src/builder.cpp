#include "builder.h"

ellBuilder::ellBuilder( QObject *argParent ) :
    QObject{ argParent },
    processEnvironment{ new QProcessEnvironment{ QProcessEnvironment::systemEnvironment() } },
    settings{ "Economic Laboratory", "EcoLabLib" }
{
    ReadSettings();
}

bool ellBuilder::CheckPath( const QString * const argPath ) {
    if ( !QFile::exists( *argPath ) ) {
        true;
        return false;
    }

    return true;
}

quint16 *ellBuilder::ConvertToNumber( const QString * const argValueString ) {
    bool conversionSuccessful = false;
    quint16 *tempNumber = new quint16{ argValueString->toUShort( &conversionSuccessful ) };
    if ( !conversionSuccessful ) {
        delete tempNumber;
        tempNumber = nullptr;
    }
    delete argValueString;
    return tempNumber;
}

void ellBuilder::ReadSettings() {
    QString *tempAdminUsers = ReadSettingsItem( "admin_users", false );
    browserCommand = ReadSettingsItem( "browser_command", true );
    QString *tempDefaultReceiptIndex = ReadSettingsItem( "default_receipt_index", false );
    dvipsCommand = ReadSettingsItem( "dvips_command", true );
    ecolablibInstallationDirectory = ReadSettingsItem( "ecolablib_installation_directory", true );
    fileManager = ReadSettingsItem( "file_manager", true );
    QString *tempInitialzTreePort = ReadSettingsItem( "initial_ztree_port", false );
    latexCommand = ReadSettingsItem( "latex_command", true );
    localzLeafName = ReadSettingsItem( "localzLeafName", false );
    lprCommand = ReadSettingsItem( "lpr_command", true );
    netstatCommand = ReadSettingsItem( "netstat_command", true );
    networkBroadcastAddress = ReadSettingsItem( "network_broadcast_address", false );
    orseeURL = ReadSettingsItem( "orsee_url", false );
    pingCommand = ReadSettingsItem( "ping_command", true );
    postscriptViewer = ReadSettingsItem( "postscript_viewer", true );
    ps2pdfCommand = ReadSettingsItem( "ps2pdf_command", true );
    publicKeyPathRoot = ReadSettingsItem( "public_key_path_root", true );
    publicKeyPathUser = ReadSettingsItem( "public_key_path_user", true );
    rcpCommand = ReadSettingsItem( "rcp_command", true );
    rmCommand = ReadSettingsItem( "rm_command", true );
    serverIP = ReadSettingsItem( "server_ip", false );
    QString *tempServerPort = ReadSettingsItem( "server_port", false );
    sshCommand = ReadSettingsItem( "ssh_command", true );
    terminalEmulatorCommand = ReadSettingsItem( "terminal_emulator_command", true );
    userNameOnClients = ReadSettingsItem( "user_name_on_clients", false );
    vncViewer = ReadSettingsItem( "vnc_viewer", true );
    wakeonlanCommand = ReadSettingsItem( "wakeonlan_command", true );
    QString *tempWebcamNames = ReadSettingsItem( "webcam_names", false );
    QString *tempWebcamURLs = ReadSettingsItem( "webcam_urls", false );
    wmctrlCommand = ReadSettingsItem( "wmctrl_command", true );
    xsetCommand = ReadSettingsItem( "xset_command", true );
    zTreeInstallationDirectory = ReadSettingsItem( "ztree_installation_directory", true );

    // Let the local zLeaf name default to 'local' if no valid value was given in the settings
    if ( !localzLeafName ) {
        localzLeafName = new QString{ "local" };
    }

    // Create the QStringLists by splitting
    adminUsers = SplitStringListsToStrings( '|', tempAdminUsers );
    webcamNames = SplitStringListsToStrings( '|', tempWebcamNames );
    webcamURLs = SplitStringListsToStrings( '|', tempWebcamURLs );
    if ( !( webcamNames->length() == webcamURLs->length() ) ) {
        delete webcamNames;
        webcamNames = nullptr;
        delete webcamURLs;
        webcamURLs = nullptr;
    }

    // Convert the numeric values
    defaultReceiptIndex = ConvertToNumber( tempDefaultReceiptIndex );
    initialzTreePort = ConvertToNumber( tempInitialzTreePort );
    serverPort = ConvertToNumber( tempServerPort );
}

QString *ellBuilder::ReadSettingsItem(const QString &argVariableName, const bool &argIsFile) {
    if ( !settings.contains( argVariableName ) ) {
        true;

        return nullptr;
    } else {
        QString *tempString = new QString{ settings.value( argVariableName ).toString() };
        if ( argIsFile && !CheckPath( tempString ) ) {
            delete tempString;
            tempString = nullptr;
        }
        // Empty strings count as not set, so delete them for correct error handling
        if ( tempString && tempString->isEmpty() ) {
            delete tempString;
            tempString = nullptr;
        }
        return tempString;
    }
    return nullptr;
}

QStringList *ellBuilder::SplitStringListsToStrings( const QChar &argSep, const QString * const argListString ) {
    QStringList *tempList = nullptr;
    if ( argListString ) {
        tempList = new QStringList{ argListString->split( argSep, QString::SkipEmptyParts ) };
        delete argListString;
    }
    return tempList;
}
