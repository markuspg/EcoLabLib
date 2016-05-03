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

ellBuilder::ellBuilder( QObject *argParent ) :
    QObject{ argParent },
    processEnvironment{ new QProcessEnvironment{ QProcessEnvironment::systemEnvironment() } },
    settings{ "Economic Laboratory", "EcoLabLib", this }
{
    ReadSettings();

    DetectInstalledZTreeVersionsAndLaTeXHeaders();
}

bool ellBuilder::CheckPath( const QString * const argPath ) {
    if ( !QFile::exists( *argPath ) ) {
        true;
        return false;
    }

    return true;
}

bool *ellBuilder::ConvertToBool( QString *& argValueString ) {
    bool *tempBool = nullptr;
    if ( argValueString ) {
        QVariant temp{ *argValueString };
        tempBool = new bool{ temp.toBool() };
    }
    delete argValueString;
    argValueString = nullptr;
    return tempBool;
}

quint16 *ellBuilder::ConvertToNumber( const QString &argVariableName, QString *& argValueString ) {
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

void ellBuilder::DetectInstalledZTreeVersionsAndLaTeXHeaders() {
    // Detect the installed LaTeX headers
    if ( ecolablibInstallationDirectory ) {
        // Get all files ending with '_header.tex'
        QDir latexDirectory{ *ecolablibInstallationDirectory,
                    "*_header.tex", QDir::Name, QDir::CaseSensitive | QDir::Files | QDir::Readable };
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

void ellBuilder::ReadSettings() {
    QString *tempAdminUsers = ReadSettingsItem( "admin_users", false );
    browserCommand = ReadSettingsItem( "browser_command", true );
    certFile = ReadSettingsItem( "cert_file_path", true );
    clientConnectionPassword = ReadSettingsItem( "client_connection_password", false );
    QString *tempDefaultReceiptIndex = ReadSettingsItem( "default_receipt_index", false );
    dvipsCommand = ReadSettingsItem( "dvips_command", true );
    ecolablibInstallationDirectory = ReadSettingsItem( "ecolablib_installation_directory", true );
    fileManager = ReadSettingsItem( "file_manager", true );
    QString *tempGlobalListening = ReadSettingsItem( "global_listening", false );
    QString *tempInitialzTreePort = ReadSettingsItem( "initial_ztree_port", false );
    keyFile = ReadSettingsItem( "key_file_path", true );
    killallCommand = ReadSettingsItem( "killall_command", true );
    latexCommand = ReadSettingsItem( "latex_command", true );
    lprCommand = ReadSettingsItem( "lpr_command", true );
    networkBroadcastAddress = ReadSettingsItem( "network_broadcast_address", false );
    orseeURL = ReadSettingsItem( "orsee_url", false );
    postscriptViewer = ReadSettingsItem( "postscript_viewer", true );
    ps2pdfCommand = ReadSettingsItem( "ps2pdf_command", true );
    publicKeyPathRoot = ReadSettingsItem( "public_key_path_root", true );
    publicKeyPathUser = ReadSettingsItem( "public_key_path_user", true );
    rmCommand = ReadSettingsItem( "rm_command", true );
    scpCommand = ReadSettingsItem( "scp_command", true );
    serverIP = ReadSettingsItem( "server_ip", false );
    QString *tempServerPort = ReadSettingsItem( "server_port", false );
    sshCommand = ReadSettingsItem( "ssh_command", true );
    terminalEmulatorCommand = ReadSettingsItem( "terminal_emulator_command", true );
    QString *tempUseSSH = ReadSettingsItem( "use_ssh", false );
    userNameOnClients = ReadSettingsItem( "user_name_on_clients", false );
    vncViewer = ReadSettingsItem( "vnc_viewer", true );
    wakeonlanCommand = ReadSettingsItem( "wakeonlan_command", true );
    webcamDisplayCommand = ReadSettingsItem( "webcam_display_command", true );
    QString *tempWebcamNames = ReadSettingsItem( "webcam_names", false );
    QString *tempWebcamURLs = ReadSettingsItem( "webcam_urls", false );
    wineCommand = ReadSettingsItem( "wine_command", true );
    wmctrlCommand = ReadSettingsItem( "wmctrl_command", true );
    xsetCommand = ReadSettingsItem( "xset_command", true );
    zTreeInstallationDirectory = ReadSettingsItem( "ztree_installation_directory", true );

    // Create the QStringLists by splitting
    adminUsers = SplitStringListsToStrings( '|', tempAdminUsers );
    webcamNames = SplitStringListsToStrings( '|', tempWebcamNames );
    webcamURLs = SplitStringListsToStrings( '|', tempWebcamURLs );
    // If the names or URLs are missing or their lengths do not match, delete them
    if ( !webcamNames || !webcamURLs || !( webcamNames->length() == webcamURLs->length() ) ) {
        SaveInvalidSettings( "general webcam settings" );
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

QString *ellBuilder::ReadSettingsItem(const QString &argVariableName, const bool &argIsFile) {
    // If setting variable is not available, return 'nullptr'
    if ( !settings.contains( argVariableName ) ) {
        true;
        SaveInvalidSettings( argVariableName );

        return nullptr;
    } else {
        QString *tempString = new QString{ settings.value( argVariableName ).toString() };
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

void ellBuilder::SaveInvalidSettings( const QString &argVariableName ) {
    if ( !notProperlySetVariables ) {
        notProperlySetVariables = new QStringList{};
    }
    notProperlySetVariables->append( argVariableName );
}

QStringList *ellBuilder::SplitStringListsToStrings( const QChar &argSep, QString *& argListString ) {
    QStringList *tempList = nullptr;
    if ( argListString ) {
        tempList = new QStringList{ argListString->split( argSep, QString::SkipEmptyParts ) };
        delete argListString;
        argListString = nullptr;
    }
    return tempList;
}
