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

enum class ellClientState_t : unsigned short int {
    BOOTING,
    CONNECTED,
    DISCONNECTED,
    SHUTTING_DOWN,
    ZLEAF_RUNNING
};

class ellClient : public QObject
{
    Q_OBJECT
public:
    explicit ellClient( const QString &argHostName, const QString &argIP, const QString &argMAC,
                        const QString &argWebcamAvailable, const QString &argXPosition, const QString &argYPosition,
                        const ellSettingsStorage * const argSettingsStorage, QObject *argParent = nullptr );
    ~ellClient();

    const QString hostName;
    const QString ip;
    const QString mac;
    const bool webcamAvailable = 0;
    const unsigned int xPosition = 1;
    const unsigned int yPosition = 1;

    //! This function gets frequently polled by Labcontrol to update the table view showing the clients
    ellClientState_t GetClientState() const { return state; }

    void Boot();
    QString *GetSessionPort() const { return sessionPort; }
    void KillzLeaf();
    void SetSessionPort( QString * const argSessionPort );
    void SetzLeafVersion( QString * const argzLeafVersion );
    void SetSocket( QTcpSocket *argSocket );
    void ShowDesktop();
    void Shutdown();
    void StartzLeaf();

signals:

public slots:
    void Disconnected();
    void ReadMessage();

private:
    quint16 blockSize = 0;
    QString *sessionPort = nullptr;     //! The port the z-Leaf on this client uses (for the 'TVClients')
    const ellSettingsStorage * const settingsStorage = nullptr;
    QTcpSocket *socket = nullptr;
    ellClientState_t state = ellClientState_t::DISCONNECTED;
    QString *zleafVersion = nullptr;    //! The z-Leaf version this client shall use

    void SendMessage( const quint16 &argMessageID, QString *argMessage = nullptr );

private slots:
};

#endif // CLIENT_H
