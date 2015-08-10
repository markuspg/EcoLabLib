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

#include "clientmanager.h"

ellClientManager::ellClientManager( const ellSettingsStorage * const argSettingsStorage, QObject *argParent ) :
    QObject{ argParent },
    clientIPsToClientsMap{ new QMap< QString, ellClient* > },
    settingsStorage{ argSettingsStorage },
    websocketServer{ new QWebSocketServer{ QStringLiteral( "ellClientManager" ),
                     QWebSocketServer::SecureMode, this } }
{
    QSslConfiguration sslConfiguration;
    QFile certFile{ *settingsStorage->certFile };
    QFile keyFile{ *settingsStorage->keyFile };
    certFile.open( QIODevice::ReadOnly );
    keyFile.open( QIODevice::ReadOnly );
    QSslCertificate certificate{ &certFile, QSsl::Pem };
    QSslKey sslKey{ &keyFile, QSsl::Rsa, QSsl::Pem };
    certFile.close();
    keyFile.close();
    sslConfiguration.setPeerVerifyMode( QSslSocket::VerifyNone );
    sslConfiguration.setLocalCertificate( certificate );
    sslConfiguration.setPrivateKey( sslKey );
    sslConfiguration.setProtocol( QSsl::TlsV1SslV3 );
    websocketServer->setSslConfiguration( sslConfiguration );

    bool successfullyStarted = false;
    // Listening is only possible, if the server port was set correctly
    if ( settingsStorage->serverPort ) {
        // Listen on every available network device
        if ( *settingsStorage->globalListening ) {
            if ( !websocketServer->listen( QHostAddress::Any, *settingsStorage->serverPort ) ) {
                throw "Listening failed";
            } else {
                successfullyStarted = true;
            }
        // Just listen on the network device specified by its IP
        } else {
            if ( settingsStorage->serverIP ) {
                if ( !websocketServer->listen( QHostAddress{ *settingsStorage->serverIP }, *settingsStorage->serverPort ) ) {
                    throw "Listening failed";
                } else {
                    successfullyStarted = true;
                }
            } else {
                throw "The mandatory server ip was not set";
            }
        }
    } else {
        throw "The mandatory server port was not set";
    }
    if ( successfullyStarted ) {
        connect( websocketServer, &QWebSocketServer::newConnection,
                 this, &ellClientManager::HandleIncomingWebSocketConnection );
    }

    QSettings clientData{ "Economic Laboratory", "EcoLabLib" };

    // Get the client quantity to check the value lists for clients creation for correct length
    if ( !clientData.contains( "client_quantity" ) ) {
        throw 20;
    }
    bool conversionSuccess = false;
    int clientQuantity = 0;
    clientQuantity = clientData.value( "client_quantity", "0" ).toInt( &conversionSuccess );
    if ( !conversionSuccess ) {
        throw 20;
    }

    // Create all the clients in the lab
    QStringList clientHostNames = clientData.value( "client_hostnames" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientHostNames.length() != clientQuantity ) {
        throw 20;
    }
    QStringList clientIPs = clientData.value( "client_ips" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientIPs.length() != clientQuantity ) {
        throw 20;
    }
    QStringList clientMACs = clientData.value( "client_macs" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientMACs.length() != clientQuantity ) {
        throw 20;
    }
    QStringList clientWebcams = clientData.value( "client_webcams" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientWebcams.length() != clientQuantity ) {
        throw 20;
    }
    QStringList clientXPositions = clientData.value( "client_xpos" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientXPositions.length() != clientQuantity ) {
        throw 20;
    }
    QStringList clientYPositions = clientData.value( "client_ypos" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientYPositions.length() != clientQuantity ) {
        throw 20;
    }

    // If all preparations where successful, create the 'clients' QVector and add the ellClient instances to it
    clients = new QVector< ellClient* >;
    for ( int i = 0; i < clientQuantity; i++ ) {
        clients->append( new ellClient{ clientHostNames[ i ], clientIPs[ i ], clientMACs[ i ], clientWebcams[ i ], clientXPositions[ i ],
                                        clientYPositions[ i ], settingsStorage, this } );

        // Add an corresponding entry to the 'client_ips_to_clients_map' std::map<QString, Client*>
        ( *clientIPsToClientsMap )[ clients->last()->ip ] = clients->last();
    }
    clients->squeeze();
}

ellClientManager::~ellClientManager() {
    websocketServer->close();
    delete clients;
}

void ellClientManager::HandleIncomingWebSocketConnection() {
    while ( websocketServer->hasPendingConnections() ) {
        QWebSocket *incConnection = websocketServer->nextPendingConnection();
        QString peerAddress = incConnection->peerAddress().toString();
        if ( clientIPsToClientsMap->contains( peerAddress ) ) {
            ellClient *connectingClient = ( *clientIPsToClientsMap )[ peerAddress ];
            connectingClient->SetWebSocket( incConnection );
            connect( incConnection, SIGNAL( textMessageReceived( QString ) ),
                     connectingClient, SLOT( PasswordReceived( QString ) ) );
            connect( incConnection, &QWebSocket::disconnected,
                     connectingClient, &ellClient::WebSocketDisconnected );
            // connect( incConnection, &QTcpSocket::readyRead,
            //          connectingClient, &ellClient::ReadMessage );
            connect( incConnection, &QWebSocket::disconnected,
                     incConnection, &QWebSocket::deleteLater );
        } else {
            incConnection->abort();
            delete incConnection;
        }
    }
}
