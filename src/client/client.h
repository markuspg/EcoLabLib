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

#ifndef CLIENT_H
#define CLIENT_H

#include <stdexcept>

#include "clientenums.h"
#include "../ecolablib_global.h"
#include "../settingsstorage.h"

#include <QObject>
#include <QWebSocket>

namespace ell {

class ClientPinger;

//! Represents a client in the laboratory
/*!
  This class represents a client in the laboratory. Each client has to be specified in 'EcoLabLib's settings file.
  A client consists of its fixed attributes, its changing state and some executable functions
 */
class ECOLABLIBSHARED_EXPORT Client : public QObject
{
    Q_OBJECT
public:
    //! 'ell::Client's constructor which will be called by 'ell::ClientManager'
    /*!
       The constructor initializes the 'ell::Client's const attributes. The default state is 'disconnected'.
       \param argHostName The hostname the client has
       \param argIP Its IP address
       \param argMAC Ist MAC address
       \param argWebcamAvailable If a webcam is attached to the client
       \param argXPosition The client's x coordinate in the laboratory
       \param argYPosition The client's y coordinate in the laboratory
       \param argSettingsStorage The 'ell::SettingsStorage' containing all settings
       \param argParent 'ell::Client's parent object
     */
    explicit Client( const QString &argHostName, const QString &argIP, const QString &argMAC,
                     const QString &argXPosition, const QString &argYPosition,
                     const SettingsStorage * const argSettingsStorage, QObject *argParent = nullptr );
    Client( const Client &argClient ) = delete;
    ~Client();

    const QString hostName;             //! The hostname of the client
    const QString ip;                   //! Its IP address
    const QString mac;                  //! Its MAC address
    const unsigned int xPosition = 1;   //! The client's x coordinate in the laboratory
    const unsigned int yPosition = 1;   //! The client's y coordinate in the laboratory

    //! This functions beams the specified directory to the folder 'media4ztree' in the client experimenter user's home directory
    /*!
     * \param argDirectoryToBeam    The directory which (including all its contents and even links) shall be beamed
     */
    void BeamFile( const QString &argDirectoryToBeam ) const;
    //! Tries to boot the client using wakeonlan
    void Boot();
    /*!
       \brief This function gets frequently polled by Labcontrol to update the table view showing the clients
       \return The client's current state
     */
    ClientState_t GetClientState() const { return state; }
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
     */
    void OpenFileSystem( const bool &argAsRoot ) const;
    //! This command opens a SSH session connected to the client in a terminal emulator on the server
    /*!
     * \param argCommand An optional command to be executed (pass an empty string, if no command shall be run)
     * \param argOpenAsRoot True, if the command shall be run as root, otherwise false)
     */
    void OpenTerminal( const QString &argCommand, const bool &argOpenAsRoot );
    /*!
       \brief This function updates the stored session port and is used by 'lcSessionStarter' or 'ell::Session's destructor
       \param argSessionPort The new 'ell::Session' port or nothing, if the client shall be freed
     */
    void SetSessionPort( QString * const argSessionPort = nullptr );
    /*!
       \brief This function updates the stored z-Leaf version and is used by 'lcMainWindow' and 'lcSessionStarter' or 'ell::Session's destructor
       \param argzLeafVersion The new 'ell::Session' z-Leaf version or nothing, if the client shall be freed
     */
    void SetzLeafVersion( QString * const argzLeafVersion = nullptr );
    /*!
       \brief This function gets called by 'ell::ClientManager' to update the socket of this client on new connection attempts
       \param argSocket The new socket
     */
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
    void HandleStateChange( unsigned int argState );
    //! This slot is called the first time a new client sends a text message. This message should contain the correct password, otherwise the connection will be closed.
    void PasswordReceived( QString argMessage );
    //! This slot gets called if the client's socket disconnected and cleans it up, also changing the client's status
    void WebSocketDisconnected();

private:
    ClientPinger * const clientPinger = nullptr;
    std::unique_ptr < const QString > sessionPort = nullptr;    //! The port the z-Leaf on this client uses (for the 'TVClients')
    const SettingsStorage * const settingsStorage = nullptr; //! Contains all external settings
    ClientState_t state = ClientState_t::DISCONNECTED;    //! The client's state
    QWebSocket *webSocket = nullptr;    //! The websocket this client is currently connected on
    std::unique_ptr < const QString > zleafVersion = nullptr;   //! The z-Leaf version this client shall use

    void SendMessage( const QString &argMessage );

private slots:
    //! This slot is called on new text messages arriving at the websocket. It processes those and undertakes needed actions.
    void TextMessageReceived( QString argMessage );
};

}

#endif // CLIENT_H
