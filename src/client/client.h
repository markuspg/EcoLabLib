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

#ifndef CLIENT_H
#define CLIENT_H

#include "../settingsstorage.h"

#include <QObject>
#include <QTcpSocket>
#include <QWebSocket>

enum class ellClientState_t : unsigned short int {
    BOOTING,
    CONNECTED,
    DISCONNECTED,
    SHUTTING_DOWN,
    ZLEAF_RUNNING
};

//! Represents a client in the laboratory
/*!
  This class represents a client in the laboratory. Each client has to be specified in 'EcoLabLib's settings file.
  A client consists of its fixed attributes, its changing state and some executable functions
 */
class ellClient : public QObject
{
    Q_OBJECT
public:
    //! 'ellClient's constructor which will be called by 'ellClientManager'
    /*!
       The constructor initializes the 'ellClient's const attributes. The default state is 'disconnected'.
       \param argHostName The hostname the client has
       \param argIP Its IP address
       \param argMAC Ist MAC address
       \param argWebcamAvailable If a webcam is attached to the client
       \param argXPosition The client's x coordinate in the laboratory
       \param argYPosition The client's y coordinate in the laboratory
       \param argSettingsStorage The 'ellSettingsStorage' containing all settings
       \param argParent 'ellClient's parent object
     */
    explicit ellClient( const QString &argHostName, const QString &argIP, const QString &argMAC,
                        const QString &argWebcamAvailable, const QString &argXPosition, const QString &argYPosition,
                        const ellSettingsStorage * const argSettingsStorage, QObject *argParent = nullptr );
    ellClient( const ellClient &argClient ) = delete;

    const QString hostName;             //! The hostname of the client
    const QString ip;                   //! Its IP address
    const QString mac;                  //! Its MAC address
    const bool webcamAvailable = 0;     //! If a webcam is attached to the client
    const unsigned int xPosition = 1;   //! The client's x coordinate in the laboratory
    const unsigned int yPosition = 1;   //! The client's y coordinate in the laboratory

    //! Tries to boot the client using wakeonlan
    void Boot();
    /*!
       \brief This function gets frequently polled by Labcontrol to update the table view showing the clients
       \return The client's current state
     */
    ellClientState_t GetClientState() const { return state; }
    /*!
       \brief This function is used by 'Labcontrol' to add port information to the table views
       \return A pointer to a QString containing the port of the session the client belongs to ('nullptr' if the client belongs to no session)
     */
    const QString *GetSessionPort() const { return sessionPort.get(); }
    //! Kills any z-Leaves on the clients
    void KillzLeaf();
    //! This command opens the client's file system in a file manager
    /*!
     * \param argAsRoot True, if the file system shall be opened with root rights; false, otherwise
     * \param argRunLocally True, if the file manager shall be opened locally; false, otherwise
     */
    void OpenFileSystem( const bool &argAsRoot, const bool &argRunLocally ) const;
    /*!
       \brief This function updates the stored session port and is used by 'lcSessionStarter' or 'ellSession's destructor
       \param argSessionPort The new 'ellSession' port or nothing, if the client shall be freed
     */
    void SetSessionPort( QString * const argSessionPort = nullptr );
    /*!
       \brief This function updates the stored z-Leaf version and is used by 'lcMainWindow' and 'lcSessionStarter' or 'ellSession's destructor
       \param argzLeafVersion The new 'ellSession' z-Leaf version or nothing, if the client shall be freed
     */
    void SetzLeafVersion( QString * const argzLeafVersion = nullptr );
    /*!
       \brief This function gets called by 'ellClientManager' to update the socket of this client on new connection attempts
       \param argSocket The new socket
     */
    void SetSocket( QTcpSocket *argSocket );
    void SetWebSocket( QWebSocket *argWebSocket );
    //! Shows locally the desktop of the client using a vnc viewer
    void ShowDesktop();
    //! Shuts down the client
    void Shutdown();
    /*!
     * \brief This function starts a z-Leaf on the client
     * \param fakeName An optional fake name, if the client shall not connect to z-Tree using its hostname
     */
    void StartzLeaf( const QString * const fakeName = nullptr );

signals:

public slots:
    //! This slot gets called if the client's socket disconnected and cleans it up, also changing the client's status
    void Disconnected();
    //! This slot gets called if new messages where received, reads them and starts necessary actions
    void ReadMessage();
    void WebSocketDisconnected();

private:
    std::unique_ptr < const QString > sessionPort = nullptr;    //! The port the z-Leaf on this client uses (for the 'TVClients')
    const ellSettingsStorage * const settingsStorage = nullptr; //! Contains all external settings
    QTcpSocket *socket = nullptr;   //! The socket this client is currently connected on
    ellClientState_t state = ellClientState_t::DISCONNECTED;    //! The client's state
    QWebSocket *webSocket = nullptr;    //! The websocket this client is currently connected on
    std::unique_ptr < const QString > zleafVersion = nullptr;   //! The z-Leaf version this client shall use

    void SendMessage( const quint16 &argMessageID, QString *argMessage = nullptr );

private slots:
};

#endif // CLIENT_H
