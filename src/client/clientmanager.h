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

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "client.h"
#include "../settingsstorage.h"

#include <QObject>
#include <QSettings>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include <QVector>

class ellSettingsStorage;

class ellClientManager : public QTcpServer
{
    Q_OBJECT
public:
    explicit ellClientManager( const ellSettingsStorage * const argSettingsStorage, QObject *argParent = nullptr );
    ~ellClientManager();

    QVector< ellClient* > *GetClients() const { return clients; }

signals:

public slots:

private:
    QMap< QString, ellClient* > * clientIPsToClientsMap = nullptr;
    QVector< ellClient* > *clients = nullptr;
    int clientQuantity = 0;
    const ellSettingsStorage * const settingsStorage;

private slots:
    void HandleIncomingConnection();
};

#endif // CLIENTMANAGER_H
