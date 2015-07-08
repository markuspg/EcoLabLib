#include "clientmanager.h"

ellClientManager::ellClientManager( const ellSettingsStorage * const argSettingsStorage, QObject *argParent ) :
    QTcpServer{ argParent },
    clientIPsToClientsMap{ new QMap< QString, ellClient* > },
    settingsStorage{ argSettingsStorage }
{
    if ( !listen( QHostAddress{ *settingsStorage->serverIP }, *settingsStorage->serverPort ) ) {
        throw 20;
    }

    connect( this, &ellClientManager::newConnection, this, &ellClientManager::HandleIncomingConnection );

    QSettings clientData{ "Economic Laboratory", "EcoLabLib" };

    // Get the client quantity to check the value lists for clients creation for correct length
    if ( !clientData.contains( "client_quantity" ) ) {
        throw 20;
    }
    bool conversionSuccess = false;
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

    for ( int i = 0; i < clientQuantity; i++ ) {
        clients.append( new ellClient{ clientHostNames[ i ], clientIPs[ i ], clientMACs[ i ], clientXPositions[ i ],
                                       clientYPositions[ i ], clientWebcams[ i ], settingsStorage, this } );

        // Add an corresponding entry to the 'client_ips_to_clients_map' std::map<QString, Client*>
        ( *clientIPsToClientsMap )[ clients.last()->ip ] = clients.last();
    }
}

ellClientManager::~ellClientManager() {
    delete clientIPsToClientsMap;
}

void ellClientManager::HandleIncomingConnection() {
    while ( this->hasPendingConnections() ) {
        QTcpSocket *incConnection = this->nextPendingConnection();
        QString peerAddress = incConnection->peerAddress().toString();
        if ( clientIPsToClientsMap->contains( peerAddress ) ) {
            ellClient *connectingClient = ( *clientIPsToClientsMap )[ peerAddress ];
            connectingClient->SetSocket( incConnection );
            connect( incConnection, &QTcpSocket::disconnected,
                     connectingClient, &ellClient::Disconnected );
            connect( incConnection, &QTcpSocket::disconnected,
                     incConnection, &QTcpSocket::deleteLater );
        } else {
            incConnection->abort();
            delete incConnection;
        }
    }
}
