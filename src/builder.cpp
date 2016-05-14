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

#include "builder.h"

ell::Builder::Builder( QObject *argParent ) :
    QObject{ argParent },
    processEnvironment{ new QProcessEnvironment{ QProcessEnvironment::systemEnvironment() } },
    settings{ "Economic Laboratory", "EcoLabLib", this },
    toolSettings{ "Economic Laboratory", "EcoLabLibDependencies", this }
{
    ReadSettings();

    DetectInstalledZTreeVersionsAndLaTeXHeaders();
}

bool ell::Builder::CheckPath( const QString * const argPath ) {
    if ( !QFile::exists( *argPath ) ) {
        true;
        return false;
    }

    return true;
}

bool *ell::Builder::ConvertToBool( QString *& argValueString ) {
    bool *tempBool = nullptr;
    if ( argValueString ) {
        QVariant temp{ *argValueString };
        tempBool = new bool{ temp.toBool() };
    }
    delete argValueString;
    argValueString = nullptr;
    return tempBool;
}

quint16 *ell::Builder::ConvertToNumber( const QString &argVariableName, QString *& argValueString ) {
    quint16 *tempNumber = nullptr;
    if ( argValueString ) {
        bool conversionSuccessful = false;
        tempNumber = new quint16{ argValueString->toUShort( &conversionSuccessful ) };
        if ( !conversionSuccessful ) {
            SaveInvalidSettings( argVariableName );
            delete tempNumber;
            tempNumber = nullptr;
        }
    }
    delete argValueString;
    argValueString = nullptr;
    return tempNumber;
}

void ell::Builder::DetectInstalledZTreeVersionsAndLaTeXHeaders() {
    // Detect the installed LaTeX headers
    if ( ecolablibInstallDir ) {
        // Get all files ending with '_header.tex'
        QDir latexDirectory{ *ecolablibInstallDir, "*_header.tex",
                             QDir::Name, QDir::CaseSensitive | QDir::Files | QDir::Readable };
        if ( !latexDirectory.exists() || latexDirectory.entryList().isEmpty() ) {
            true;
            installedLaTeXHeaders = new QStringList{ "None found" };
        } else {
            installedLaTeXHeaders = new QStringList{ latexDirectory.entryList() };
            installedLaTeXHeaders->replaceInStrings( "_header.tex", "" );
        }
    }

    // Detect the installed zTree versions
    if ( zTreeInstallationDirectory ) {
        // Get all directories beginning with 'zTree_'
        QDir zTreeDirectory{ *zTreeInstallationDirectory, "zTree_*", QDir::Name,
                    QDir::NoDotAndDotDot | QDir::Dirs | QDir::Readable | QDir::CaseSensitive };
        if ( zTreeDirectory.entryList().isEmpty() ) {
            true;
        }
        else {
            installedzTreeVersions = new QStringList{ zTreeDirectory.entryList() };
            installedzTreeVersions->replaceInStrings( "zTree_", "" );
        }
    }
}

void ell::Builder::ReadSettings() {
    QString *tempAdminUsers = ReadSettingsItem( settings, "admin_users", false );
    browserCommand = ReadSettingsItem( toolSettings, "browser_command", true, true );
    certFile = ReadSettingsItem( settings, "cert_file_path", true );
    clientConnectionPassword = ReadSettingsItem( settings, "client_connection_password", false );
    QString *tempDefaultReceiptIndex = ReadSettingsItem( settings, "default_receipt_index",
                                                         false, true );
    dvipsCommand = ReadSettingsItem( toolSettings, "dvips_command", true );
    ecolablibInstallDir = ReadSettingsItem( settings, "ecolablib_installation_directory", true );
    fileManager = ReadSettingsItem( toolSettings, "file_manager", true, true );
    QString *tempGlobalListening = ReadSettingsItem( settings, "global_listening", false );
    QString *tempInitialzTreePort = ReadSettingsItem( settings, "initial_ztree_port",
                                                      false, true );
    keyFile = ReadSettingsItem( settings, "key_file_path", true );
    killallCommand = ReadSettingsItem( toolSettings, "killall_command", true );
    latexCommand = ReadSettingsItem( toolSettings, "latex_command", true );
    lprCommand = ReadSettingsItem( toolSettings, "lpr_command", true );
    networkBroadcastAddress = ReadSettingsItem( settings, "network_broadcast_address", false );
    orseeURL = ReadSettingsItem( settings, "orsee_url", false, true );
    pingCommand = ReadSettingsItem( toolSettings, "ping_command", true );
    postscriptViewer = ReadSettingsItem( toolSettings, "postscript_viewer", true );
    ps2pdfCommand = ReadSettingsItem( toolSettings, "ps2pdf_command", true );
    publicKeyPathRoot = ReadSettingsItem( settings, "public_key_path_root", true, true );
    publicKeyPathUser = ReadSettingsItem( settings, "public_key_path_user", true, true );
    rmCommand = ReadSettingsItem( toolSettings, "rm_command", true );
    scpCommand = ReadSettingsItem( toolSettings, "scp_command", true, true );
    serverIP = ReadSettingsItem( settings, "server_ip", false );
    QString *tempServerPort = ReadSettingsItem( settings, "server_port", false );
    sshCommand = ReadSettingsItem( toolSettings, "ssh_command", true, true );
    terminalEmulatorCommand = ReadSettingsItem( toolSettings, "terminal_emulator_command",
                                                true, true );
    QString *tempUseSSH = ReadSettingsItem( settings, "use_ssh", false, true );
    userNameOnClients = ReadSettingsItem( settings, "user_name_on_clients", false );
    vncViewer = ReadSettingsItem( toolSettings, "vnc_viewer", true, true );
    wakeonlanCommand = ReadSettingsItem( toolSettings, "wakeonlan_command", true );
    webcamDisplayCommand = ReadSettingsItem( toolSettings, "webcam_display_command",
                                             true, true );
    QString *tempWebcamNames = ReadSettingsItem( settings, "webcam_names", false, true );
    QString *tempWebcamURLs = ReadSettingsItem( settings, "webcam_urls", false, true );
    wineCommand = ReadSettingsItem( toolSettings, "wine_command", true );
    wmctrlCommand = ReadSettingsItem( toolSettings, "wmctrl_command", true, true );
    zTreeInstallationDirectory = ReadSettingsItem( settings, "ztree_installation_directory", true );

    // Create the QStringLists by splitting
    adminUsers = SplitStringListsToStrings( '|', tempAdminUsers );
    webcamNames = SplitStringListsToStrings( '|', tempWebcamNames );
    webcamURLs = SplitStringListsToStrings( '|', tempWebcamURLs );
    // If the names or URLs are missing or their lengths do not match, delete them
    if ( !webcamNames || !webcamURLs || !( webcamNames->length() == webcamURLs->length() ) ) {
        delete webcamNames;
        webcamNames = nullptr;
        delete webcamURLs;
        webcamURLs = nullptr;
    }

    // Convert the boolean values
    globalListening = ConvertToBool( tempGlobalListening );
    useSSH = ConvertToBool( tempUseSSH );

    // Convert the numeric values
    defaultReceiptIndex = ConvertToNumber( "default_receipt_index", tempDefaultReceiptIndex );
    initialzTreePort = ConvertToNumber( "initial_ztree_port", tempInitialzTreePort );
    serverPort = ConvertToNumber( "server_port", tempServerPort );
}

QString *ell::Builder::ReadSettingsItem( const QSettings &argSettingsStorage,
                                         const QString &argVariableName,
                                         const bool argIsFile, const bool argUnimportant ) {
    // If setting variable is not available, return 'nullptr'
    if ( !argSettingsStorage.contains( argVariableName ) && !argUnimportant ) {
        SaveInvalidSettings( argVariableName );

        return nullptr;
    } else {
        QString *tempString = new QString{ argSettingsStorage.value( argVariableName ).toString() };
        // If the variable is a file, check for existance and set to 'nullptr' on failure
        if ( argIsFile && !CheckPath( tempString ) ) {
            SaveInvalidSettings( argVariableName );
            delete tempString;
            tempString = nullptr;
        }
        // Empty strings count as not set, so delete them for correct error handling
        if ( tempString && tempString->isEmpty() ) {
            SaveInvalidSettings( argVariableName );
            delete tempString;
            tempString = nullptr;
        }
        return tempString;
    }
    return nullptr;
}

void ell::Builder::SaveInvalidSettings( const QString &argVariableName ) {
    if ( !notProperlySetVariables ) {
        notProperlySetVariables = new QStringList{};
    }
    notProperlySetVariables->append( argVariableName );
}

QStringList *ell::Builder::SplitStringListsToStrings( const QChar &argSep, QString *& argListString ) {
    QStringList *tempList = nullptr;
    if ( argListString ) {
        tempList = new QStringList{ argListString->split( argSep, QString::SkipEmptyParts ) };
        delete argListString;
        argListString = nullptr;
    }
    return tempList;
}
