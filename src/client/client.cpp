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

#include "client.h"
#include "clientpinger.h"

ell::Client::Client( const QString &argHostName, const QString &argIP, const QString &argMAC,
                      const QString &argWebcamAvailable, const QString &argXPosition, const QString &argYPosition,
                      const SettingsStorage * const argSettingsStorage, QObject *argParent ) :
    QObject{ argParent },
    hostName{ argHostName },
    ip{ argIP },
    mac{ argMAC },
    webcamAvailable{ static_cast< bool >( argWebcamAvailable.toUInt() ) },
    xPosition{ argXPosition.toUInt() },
    yPosition{ argYPosition.toUInt() },
    clientPinger{ new ClientPinger{ &ip, argSettingsStorage->pingCommand, this } },
    settingsStorage{ argSettingsStorage }
{
}

ell::Client::~Client() {
    clientPinger->requestInterruption();
    clientPinger->wait( 4096 );
    delete clientPinger;
}

void ell::Client::BeamFile( const QString &argDirectoryToBeam ) const {
    // Break, if the client is neither connected nor running a z-Leaf
    if ( state != ClientState_t::CONNECTED && state != ClientState_t::ZLEAF_RUNNING ) {
        return;
    }

    QStringList arguments;
    arguments << "-2" << "-B" << "-i" << *settingsStorage->publicKeyPathUser << "-l" << "65536"
              << "-r" << argDirectoryToBeam
              << QString{ *settingsStorage->userNameOnClients + "@" + ip + ":media4ztree" };

    // Start the process
    QProcess beamFileProcess;
    beamFileProcess.setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
    beamFileProcess.startDetached( *settingsStorage->scpCommand, arguments );
}

void ell::Client::Boot() {
    if ( !settingsStorage->networkBroadcastAddress || !settingsStorage->serverIP
         || !settingsStorage->wakeonlanCommand ) {
        return;
    }
    if ( state == ClientState_t::CONNECTED || state == ClientState_t::SHUTTING_DOWN ) {
        return;
    }
    QStringList arguments;
    // For Debian based UNIXes shipping the 'wakeonlan' program
    if ( settingsStorage->wakeonlanCommand->contains( "wakeonlan" ) ) {
        arguments << "-i" << *settingsStorage->networkBroadcastAddress << mac;
    }
    // For Fedora based UNIXes shipping the 'wol' program
    if ( settingsStorage->wakeonlanCommand->contains( "wol" ) ) {
        arguments << "-i" << *settingsStorage->serverIP << mac;
    }

    // Start the process
    QProcess wakeonlanProcess;
    wakeonlanProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
    wakeonlanProcess.startDetached( *settingsStorage->wakeonlanCommand, arguments );

    state = ClientState_t::BOOTING;
}

void ell::Client::KillzLeaf() {
    SendMessage( "KillzLeaf" );
}

void ell::Client::OpenFileSystem( const bool &argAsRoot ) const {
    if ( !settingsStorage->fileManager ) {
        return;
    }

    // Do nothing, if the client is not in an applicable state
    if ( !( state == ClientState_t::CONNECTED || state == ClientState_t::ZLEAF_RUNNING ) ) {
        return;
    }

    QString userToBeUsed;
    if ( argAsRoot ) {
        userToBeUsed = "root";
    } else {
        if ( settingsStorage->userNameOnClients ) {
            userToBeUsed = *settingsStorage->userNameOnClients;
        } else {
            return;
        }
    }

    QStringList arguments = QStringList{} << QString{ "sftp://" + userToBeUsed + "@" + ip };

    QProcess openFilesystemProcess;
    openFilesystemProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
    openFilesystemProcess.startDetached( *settingsStorage->fileManager, arguments );
}

void ell::Client::OpenTerminal( const QString &argCommand, const bool &argOpenAsRoot ) {
    if ( settingsStorage->terminalEmulatorCommand && settingsStorage->sshCommand ) {
        // Do nothing, if the client is not in an applicable state
        if ( !( state == ClientState_t::CONNECTED || state == ClientState_t::ZLEAF_RUNNING ) ) {
            return;
        }

        QStringList arguments;
        if ( !argOpenAsRoot ) {
            if ( !settingsStorage->publicKeyPathUser || !settingsStorage->userNameOnClients ) {
                return;
            }
            arguments << "--title" << hostName << "-e"
                      << QString{ *settingsStorage->sshCommand + " -i " + *settingsStorage->publicKeyPathUser
                                  + " " + *settingsStorage->userNameOnClients + "@" + ip };
        } else {
            if ( !settingsStorage->publicKeyPathRoot ) {
                return;
            }
            arguments << "--title" << hostName << "-e"
                      << QString{ *settingsStorage->sshCommand + " -i " + *settingsStorage->publicKeyPathRoot + " " + "root@" + ip };
        }

        if ( settingsStorage->terminalEmulatorCommand->contains( "konsole" ) ) {
            arguments.prepend( "--new-tab" );
            arguments.prepend( "--show-tabbar" );
        } else {
            if ( settingsStorage->terminalEmulatorCommand->contains( "gnome-terminal" ) ) {
                arguments.prepend( "--tab" );
            }
        }

        if ( !argCommand.isEmpty() ) {
            arguments.append( " '" + argCommand + "'" );
        }

        QProcess openTerminalProcess;
        openTerminalProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
        openTerminalProcess.startDetached( *settingsStorage->terminalEmulatorCommand, arguments );
    }
}

void ell::Client::PasswordReceived( QString argMessage ) {
    if ( argMessage != *settingsStorage->clientConnectionPassword ) {
        webSocket->close( QWebSocketProtocol::CloseCodePolicyViolated, "Wrong password given" );
    } else {
        disconnect( webSocket, SIGNAL( textMessageReceived( QString ) ),
                    this, SLOT( PasswordReceived( QString ) ) );
        connect( webSocket, SIGNAL( textMessageReceived( QString ) ),
                 this, SLOT( TextMessageReceived( QString ) ) );
    }
}

void ell::Client::SendMessage( const QString &argMessage ) {
    if ( webSocket ) {
        webSocket->sendTextMessage( argMessage );
    }
}

void ell::Client::SetSessionPort( QString * const argSessionPort ) {
    sessionPort.reset( argSessionPort );
}

void ell::Client::SetWebSocket( QWebSocket *argWebSocket ) {
    if ( webSocket ) {
        webSocket->abort();
        webSocket->deleteLater();
    }
    webSocket = argWebSocket;
    webSocket->setParent( this );
    if ( webSocket->isValid() ) {
        state = ClientState_t::CONNECTED;
    }
}

void ell::Client::SetzLeafVersion( QString * const argzLeafVersion ) {
    zleafVersion.reset( argzLeafVersion );
}

void ell::Client::ShowDesktop() {
    if ( !settingsStorage->vncViewer ) {
        return;
    }

    QStringList arguments{ ip };

    QProcess showDesktopProcess;
    showDesktopProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
    showDesktopProcess.startDetached( *settingsStorage->vncViewer, arguments );
}

void ell::Client::Shutdown() {
    SendMessage( "Shutdown" );
}

void ell::Client::StartzLeaf( const QString * const fakeName ) {
    // Don't crash if the port or version for z-Leaf are not set yet
    if ( !sessionPort || !settingsStorage->serverIP || !zleafVersion ) {
        return;
        if ( fakeName ) {
            delete fakeName;
        }
    }

    if ( !fakeName ) {
        SendMessage( QString{ "StartzLeaf|" + *zleafVersion + "|"
                              + *settingsStorage->serverIP + "|" + *sessionPort } );
    } else {
        SendMessage( QString{ "StartzLeaf|" + *zleafVersion + "|"
                              + *settingsStorage->serverIP
                              + "|" + *sessionPort + "|" + *fakeName } );
        delete fakeName;
    }
}

void ell::Client::TextMessageReceived( QString argMessage ) {
    QStringList tempMessageSplit = argMessage.split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );

    bool conversionSucceeded = false;
    int messageID = tempMessageSplit[ 0 ].toInt( &conversionSucceeded );
    if ( !conversionSucceeded ) {
        throw std::runtime_error{ "Conversion of sent message index to int failed" };
    }

    switch ( messageID ) {
    case 0:
        state = ClientState_t::ZLEAF_RUNNING;
        break;
    case 1:
        state = ClientState_t::CONNECTED;
        break;
    default:
        true;
    }
}

void ell::Client::WebSocketDisconnected() {
    webSocket->deleteLater();
    webSocket = nullptr;
    state = ClientState_t::DISCONNECTED;
}
