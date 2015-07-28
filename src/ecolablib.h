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

#ifndef ECOLABLIB_H
#define ECOLABLIB_H

#include "client/clientmanager.h"
#include "ecolablib_global.h"
#include "settingsstorage.h"
#include "session/sessionsmodel.h"

#include <QObject>

class ellBuilder;

class ECOLABLIBSHARED_EXPORT EcoLabLib : public QObject {
    Q_OBJECT

public:
    explicit EcoLabLib( const ellBuilder &argBuilder, QObject *argParent = nullptr );
    ~EcoLabLib();

    QVector< ellClient* > *GetClients() const { return clientManager.GetClients(); }
    ellSessionsModel *GetSessionsModel() const { return sessionsModel; }
    const ellSettingsStorage *GetSettingsStorage() { return settingsStorage; }
    bool ShowORSEE();
    bool ShowPreprints();

public slots:
    void StartNewSession( QVector< ellClient* > * const argAssociatedClients, const QString &argParticipiantNameReplacement,
                          const bool &argPrintAnonymousReceipts, const QString &argReceiptsHeader,
                          const QString &argzTreeDataTargetPath, const quint16 &argzTreePort, const QString &argzTreeVersion );

private:
    ellSessionsModel *sessionsModel = nullptr;
    const ellSettingsStorage * const settingsStorage = nullptr;
    ellClientManager clientManager;
};

#endif // ECOLABLIB_H
