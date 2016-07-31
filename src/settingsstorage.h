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

#ifndef SETTINGSSTORAGE_H
#define SETTINGSSTORAGE_H

#include "builder.h"

#include <QObject>

#include <memory>

namespace ell {

//! Stores all settings of 'EcoLabLib'
/*!
  This class stores all settings needed by any part of 'EcoLabLib' or 'Labcontrol'. Those are
  owned by this, const and can be publicly accessed for a low overhead.
 */
class SettingsStorage : public QObject
{
    Q_OBJECT
public:
    //! 'ell::SettingsStorage's constructor which will be called by 'EcoLabLib'
    /*!
       This constructor initializes all settings using the pointers passed through 'argBuilder'.
       Any settings who failed to load properly will be set to 'nullptr'.
       \param argBuilder    The builder used to get the const variables values
       \param argParent     'ell::SettingsStorage's parent object
     */
    explicit SettingsStorage( const Builder &argBuilder, QObject *argParent = nullptr );
    SettingsStorage( const SettingsStorage &argSettingsStorage ) = delete;
    ~SettingsStorage();

    const std::unique_ptr< const QStringList > adminUsers = nullptr;    //! Stores all users having administrative rights
    const std::unique_ptr< const QString > browserCommand = nullptr;    //! Stores the path to open a browser
    const std::unique_ptr< const QString > certFile = nullptr;  //! The path to the certificate for the websockets server in 'ell::ClientManager'
    const std::unique_ptr< const QString > clientConnectionPassword = nullptr;    //! Stores the path to open a browser
    const std::unique_ptr< const quint16 > defaultReceiptIndex = nullptr;   //! Stores the index (counting from 0) of the default receipt to use
    const std::unique_ptr< const QString > dvipsCommand = nullptr;  //! The path to convert a dvi file to postscript
    const std::unique_ptr< const QString > ecolablibInstallationDirectory = nullptr;    //! The directory where 'EcolabLib's data will be stored (e.g. receipt headers, preprints etc.)
    const std::unique_ptr< const QString > fileManager = nullptr;   //! The path to open a file manager
    const std::unique_ptr< const quint16 > initialzTreePort = nullptr;  //! The default port which shall be set for new z-Tree instances
    const std::unique_ptr< const QStringList > installedLaTeXHeaders = nullptr; //! Stores all detected LaTeX headers
    const std::unique_ptr< const QStringList > installedzTreeVersions = nullptr;    //! Stores all detected z-Tree versions
    const std::unique_ptr< const QString > keyFile = nullptr;  //! The path to the private key for the websockets server in 'ell::ClientManager'
    const std::unique_ptr< const QString > killallCommand = nullptr;    //! The path used to kill running z-Leaf instances
    const std::unique_ptr< const QString > latexCommand = nullptr;  //! The path to compile a LaTeX file
    const std::unique_ptr< const QString > lprCommand = nullptr;    //! The path to automatically print a postscript file
    const std::unique_ptr< const QString > networkBroadcastAddress = nullptr;   //! The network broadcast address of the laboratory (used for wake-on-lan)
    const QStringList * const notProperlySetVariables = nullptr;    //! This list stores all variables whose loading or initialization failed
    const std::unique_ptr< const QString > orseeURL = nullptr;  //! The URL to the laboratory's ORSEE page
    const QString * const pingCommand = nullptr;    //! The path to the ping command used to query the clients' statuses
    const std::unique_ptr< const QString > postscriptViewer = nullptr;  //! The path to a postscript viewer
    const std::unique_ptr< const QProcessEnvironment > processEnvironment = nullptr;    //! Stores a process environment for newly created processes
    const std::unique_ptr< const QString > ps2pdfCommand = nullptr; //! The path to convert postscript files to pdf
    const std::unique_ptr< const QString > publicKeyPathRoot = nullptr; //! The path to the public key file used for root access to the clients
    const std::unique_ptr< const QString > publicKeyPathUser = nullptr;//! The path to the public key file used for user access to the clients
    const std::unique_ptr< const QString > rmCommand = nullptr; //! The path to rm to remove surplus files after receipts creation
    const std::unique_ptr< const QString > scpCommand = nullptr;    //! The path to scp to copy files to the clients
    const std::unique_ptr< const QString > serverIP = nullptr;  //! The IP z-Tree and 'EcoLabLib' shall listen on
    const std::unique_ptr< const quint16 > serverPort = nullptr;    //! The port 'EcoLabLib' shall listen on for client connections
    const std::unique_ptr< const QString > sshCommand = nullptr;    //! The path to ssh
    const std::unique_ptr< const QString > terminalEmulatorCommand = nullptr;   //! The path to a terminal emulator
    const std::unique_ptr< const bool > useSSH = nullptr;  //! True, if the server shall use SSH to send commands to the clients, otherwise false
    const std::unique_ptr< const QString > userNameOnClients = nullptr; //! The name of the user used for experiments on the clients
    const std::unique_ptr< const QString > vncViewer = nullptr; //! The path to a vnc viewer
    const std::unique_ptr< const QString > wakeonlanCommand = nullptr;  //! The path to a wakeonlan command
    const std::unique_ptr< const QString > webcamDisplayCommand = nullptr;  //! The path to a program to show webcams by URLs to a jpeg export
    const std::unique_ptr< const QStringList > webcamNames = nullptr;   //! An array of the names of the webcams
    const std::unique_ptr< const QStringList > webcamURLs = nullptr;    //! An array of the corresponding URLs (should be in the same order)
    const std::unique_ptr< const QString > wineCommand = nullptr;   //! The path of wine (only relevant for UNIX systems)
    const std::unique_ptr< const QString > wmctrlCommand = nullptr; //! The path of the wmctrl command to change window names on UNIX systems
    const std::unique_ptr< const QString > zTreeInstallationDirectory = nullptr; //! The directory where all z-Tree executables are stored (in subfolders 'zTree_X.Y.Z' or 'zTree_X.Y.ZZ')

signals:

public slots:

private:
};

}

#endif // SETTINGSSTORAGE_H
