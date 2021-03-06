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

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <stdexcept>

#include "../ecolablib_global.h"
#include "client.h"
#include "../settingsstorage.h"

#include <QDataStream>
#include <QObject>
#include <QSettings>
#include <QSslError>
#include <QSslKey>
#include <QTcpServer>
#include <QTextStream>
#include <QVector>
#include <QWebSocketServer>

class ellSettingsStorage;

//! Contains and manages all known clients
/*!
  This class does the client management part of 'EcoLabLib'. It receives connection attempts
  of clients, accepts valid ones and updates the 'ellClient's statuses and sockets.
 */
class ECOLABLIBSHARED_EXPORT ellClientManager : public QObject
{
    Q_OBJECT
public:
    //! 'ellClientManager's constructor which will be called by 'EcoLabLib'
    /*!
       The constructor creates the QMap linking client IPs to the actual 'ellClient' instances,
       initializes and starts the listening of the TCP server and creates all 'ellClient' instances.
       \param argSettingsStorage The 'ellSettingsStorage' containing all settings
       \param argParent 'ellClientManager's parent object
     */
    explicit ECOLABLIBSHARED_EXPORT ellClientManager( const ellSettingsStorage * const argSettingsStorage, QObject *argParent = nullptr );
    //! This destructor cleans up all data on the heap
    ~ellClientManager();

    //! Returns the managed clients
    /*!
       This function returns the managed clients which are used by
       'Labcontrol' to initialize the table views displaying the clients.
       \return A QVector containing all managed (=existing) clients
     */
    QVector< ellClient* > *GetClients() const { return clients; }

signals:
    void HelpRequestRetrieved( QStringList *argHelpRequestMessage );

public slots:

private:
    std::unique_ptr< QMap< QString, ellClient* > > clientIPsToClientsMap = nullptr; //! This QMap is used to find the 'ellClient' instances corresponding to IP addresses. This is used to treat client connection attempts correctly
    QVector< ellClient* > *clients = nullptr;       //! This QVector stores all 'ellClient' instances
    QTcpServer *helpMessageServer = nullptr;       //! A TCP server to retrieve clients' help requests
    const ellSettingsStorage * const settingsStorage = nullptr; //! Contains all external settings
    QWebSocketServer *websocketServer = nullptr;    //! A WebSocket server to handle the clients' ClientClient connections

private slots:
    //! Handles incoming connections
    /*!
       This slot handles incoming connections. The address of the peer is searched for in the 'clientIPsToClientsMap'. If it is found, the old connection of the corresponding client will be replaced with the new one. If the peer's IP is not found in the map, the connection will be aborted.
     */
    void HandleIncomingWebSocketConnection();
    void OpenHelpRequestServer();
    void SendHelpRequestReply();
};

#endif // CLIENTMANAGER_H
