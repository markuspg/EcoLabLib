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

#include "builder.h"

ellBuilder::ellBuilder( QObject *argParent ) :
    QObject{ argParent },
    processEnvironment{ new QProcessEnvironment{ QProcessEnvironment::systemEnvironment() } },
    settings{ "Economic Laboratory", "EcoLabLib" }
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

quint16 *ellBuilder::ConvertToNumber( const QString * const argValueString ) {
    quint16 *tempNumber = nullptr;
    if ( argValueString ) {
        bool conversionSuccessful = false;
        tempNumber = new quint16{ argValueString->toUShort( &conversionSuccessful ) };
        if ( !conversionSuccessful ) {
            delete tempNumber;
            tempNumber = nullptr;
        }
    }
    delete argValueString;
    return tempNumber;
}

void ellBuilder::DetectInstalledZTreeVersionsAndLaTeXHeaders() {
    // Detect the installed LaTeX headers
    if ( ecolablibInstallationDirectory ) {
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
    QString *tempDefaultReceiptIndex = ReadSettingsItem( "default_receipt_index", false );
    dvipsCommand = ReadSettingsItem( "dvips_command", true );
    ecolablibInstallationDirectory = ReadSettingsItem( "ecolablib_installation_directory", true );
    fileManager = ReadSettingsItem( "file_manager", true );
    QString *tempInitialzTreePort = ReadSettingsItem( "initial_ztree_port", false );
    killallCommand = ReadSettingsItem( "killall_command", true );
    latexCommand = ReadSettingsItem( "latex_command", true );
    localzLeafName = ReadSettingsItem( "localzLeafName", false );
    lprCommand = ReadSettingsItem( "lpr_command", true );
    networkBroadcastAddress = ReadSettingsItem( "network_broadcast_address", false );
    orseeURL = ReadSettingsItem( "orsee_url", false );
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
    webcamDisplayCommand = ReadSettingsItem( "webcam_display_command", true );
    QString *tempWebcamNames = ReadSettingsItem( "webcam_names", false );
    QString *tempWebcamURLs = ReadSettingsItem( "webcam_urls", false );
    wineCommand = ReadSettingsItem( "wine_command", true );
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
    if ( webcamNames && webcamURLs && !( webcamNames->length() == webcamURLs->length() ) ) {
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
