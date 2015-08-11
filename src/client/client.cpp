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

#include "client.h"

ellClient::ellClient( const QString &argHostName, const QString &argIP, const QString &argMAC,
                      const QString &argWebcamAvailable, const QString &argXPosition, const QString &argYPosition,
                      const ellSettingsStorage * const argSettingsStorage, QObject *argParent ) :
    QObject{ argParent },
    hostName{ argHostName },
    ip{ argIP },
    mac{ argMAC },
    webcamAvailable{ static_cast< bool >( argWebcamAvailable.toUInt() ) },
    xPosition{ argXPosition.toUInt() },
    yPosition{ argYPosition.toUInt() },
    settingsStorage{ argSettingsStorage }
{
}

void ellClient::Boot() {
    if ( state == ellClientState_t::CONNECTED || state == ellClientState_t::SHUTTING_DOWN ) {
        return;
    }
    QStringList arguments;
#ifdef Q_OS_UNIX
    // For Debian based UNIXes shipping the 'wakeonlan' program
    if ( settingsStorage->wakeonlanCommand->contains( "wakeonlan" ) ) {
        arguments << "-i" << *settingsStorage->networkBroadcastAddress << mac;
    }
    // For Fedora based UNIXes shipping the 'wol' program
    if ( settingsStorage->wakeonlanCommand->contains( "wol" ) ) {
        arguments << "-i" << *settingsStorage->serverIP << mac;
    }
#endif

    // Start the process
    QProcess wakeonlanProcess;
    wakeonlanProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
    wakeonlanProcess.startDetached( *settingsStorage->wakeonlanCommand, arguments );

    state = ellClientState_t::BOOTING;
}

void ellClient::KillzLeaf() {
    SendMessage( 2 );
}

void ellClient::OpenFileSystem( const bool &argAsRoot ) const {
    // Do nothing, if the client is not in an applicable state
    if ( !( state == ellClientState_t::CONNECTED || state == ellClientState_t::ZLEAF_RUNNING ) ) {
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

void ellClient::OpenTerminal( const QString &argCommand, const bool &argOpenAsRoot ) {
    if ( settingsStorage->terminalEmulatorCommand ) {
        // Do nothing, if the client is not in an applicable state
        if ( !( state == ellClientState_t::CONNECTED || state == ellClientState_t::ZLEAF_RUNNING ) ) {
            return;
        }

        QStringList arguments;
        if ( !argOpenAsRoot ) {
            arguments << "--title" << hostName << "-e"
                      << QString{ *settingsStorage->sshCommand + " -i " + *settingsStorage->publicKeyPathUser
                                  + " " + *settingsStorage->userNameOnClients + "@" + ip };
        } else {
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

void ellClient::PasswordReceived( QString argMessage ) {
    if ( argMessage != *settingsStorage->clientConnectionPassword ) {
        webSocket->close( QWebSocketProtocol::CloseCodePolicyViolated, "Wrong password given" );
    } else {
        disconnect( webSocket, SIGNAL( textMessageReceived( QString ) ),
                    this, SLOT( PasswordReceived( QString ) ) );
        connect( webSocket, SIGNAL( textMessageReceived( QString ) ),
                 this, SLOT( TextMessageReceived( QString ) ) );
    }
}

void ellClient::SendMessage( const quint16 &argMessageID, QString *argMessage ) {
    if ( argMessage ) {
        webSocket->sendTextMessage( QString::number( argMessageID ) + "|"
                                    + *argMessage );
        delete argMessage;
    } else {
        webSocket->sendTextMessage( QString::number( argMessageID ) );
    }
}

void ellClient::SetSessionPort( QString * const argSessionPort ) {
    sessionPort.reset( argSessionPort );
}

void ellClient::SetWebSocket( QWebSocket *argWebSocket ) {
    if ( webSocket ) {
        webSocket->abort();
        delete webSocket;
    }
    webSocket = argWebSocket;
    webSocket->setParent( this );
    if ( webSocket->isValid() ) {
        state = ellClientState_t::CONNECTED;
    }
}

void ellClient::SetzLeafVersion( QString * const argzLeafVersion ) {
    zleafVersion.reset( argzLeafVersion );
}

void ellClient::ShowDesktop() {
    QStringList arguments{ ip };

    QProcess showDesktopProcess;
    showDesktopProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
    showDesktopProcess.startDetached( *settingsStorage->vncViewer, arguments );
}

void ellClient::Shutdown() {
    SendMessage( 0 );
}

void ellClient::StartzLeaf( const QString * const fakeName ) {
    // Don't crash if the port or version for z-Leaf are not set yet
    if ( !sessionPort || !zleafVersion ) {
        return;
        if ( fakeName ) {
            delete fakeName;
        }
    }

    if ( !fakeName ) {
        SendMessage( 1, new QString{ *zleafVersion + "|" + *settingsStorage->serverIP + "|" + *sessionPort } );
    } else {
        SendMessage( 1, new QString{ *zleafVersion + "|" + *settingsStorage->serverIP
                                     + "|" + *sessionPort + "|" + *fakeName } );
        delete fakeName;
    }
}

void ellClient::TextMessageReceived( QString argMessage ) {
    QStringList tempMessageSplit = argMessage.split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );

    bool conversionSucceeded = false;
    int messageID = tempMessageSplit[ 0 ].toInt( &conversionSucceeded );
    if ( !conversionSucceeded ) {
        throw "Conversion to int failed";
    }

    switch ( messageID ) {
    case 0:
        state = ellClientState_t::ZLEAF_RUNNING;
        break;
    case 1:
        state = ellClientState_t::CONNECTED;
        break;
    default:
        true;
    }
}

void ellClient::WebSocketDisconnected() {
    webSocket->deleteLater();
    webSocket = nullptr;
    state = ellClientState_t::DISCONNECTED;
}
