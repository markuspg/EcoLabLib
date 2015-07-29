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

ellClient::~ellClient() {
    delete sessionPort;
    delete socket;
    delete zleafVersion;
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

void ellClient::Disconnected() {
    delete socket;
    socket = nullptr;
    state = ellClientState_t::DISCONNECTED;
}

void ellClient::KillzLeaf() {
    SendMessage( 2 );
}

void ellClient::ReadMessage() {
    QDataStream in( socket );
    in.setVersion( QDataStream::Qt_5_2 );

    quint16 messageID = 0;
    blockSize = 0;
    if ( blockSize == 0 ) {
        if ( socket->bytesAvailable() < ( int )sizeof( quint16 ) ) {
            return;
        }
        in >> blockSize;
        in >> messageID;
    }

    if ( socket->bytesAvailable() < blockSize ) {
        return;
    }

    qDebug() << QString::number( blockSize );
    qDebug() << QString::number( messageID );

    QString serverAnswer;
    in >> serverAnswer;

    qDebug() << serverAnswer;

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

void ellClient::SendMessage( const quint16 &argMessageID, QString *argMessage ) {
    QByteArray block;
    QDataStream out{ &block, QIODevice::WriteOnly };
    out.setVersion( QDataStream::Qt_5_2 );
    out << ( quint16 )0;
    out << ( quint16 )argMessageID;
    if ( argMessage ) {
        out << *argMessage;
        delete argMessage;
    }
    out.device()->seek( 0 );
    out << ( quint16 )( block.size() - sizeof( quint16 ) * 2 );

    socket->write( block );
}

void ellClient::SetSessionPort( QString * const argSessionPort ) {
    delete sessionPort;
    sessionPort = argSessionPort;
}

void ellClient::SetSocket( QTcpSocket *argSocket ) {
    if ( socket ) {
        socket->abort();
        delete socket;
    }
    socket = argSocket;
    if ( socket->isValid() ) {
        state = ellClientState_t::CONNECTED;
    }
}

void ellClient::SetzLeafVersion( QString * const argzLeafVersion ) {
    delete zleafVersion;
    zleafVersion = argzLeafVersion;
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

void ellClient::StartzLeaf() {
    SendMessage( 1, new QString{ *zleafVersion + "|" + *settingsStorage->serverIP + "|" + *sessionPort } );
}
