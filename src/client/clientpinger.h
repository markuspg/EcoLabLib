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

#ifndef CLIENTPINGER_H
#define CLIENTPINGER_H

#include <QProcess>
#include <QThread>

#include "clientenums.h"

namespace ell {

class ClientPinger final : public QThread {
    Q_OBJECT
public:
    explicit ClientPinger( const QString * const argIP, const QString * const argPingCommand,
                           QObject *argParent = nullptr );

signals:
    void stateChanged( ClientState_t newState );

public slots:

protected:
    virtual void run();

private:
    const QStringList pingArguments;
    const QString * const pingCommand = nullptr;
    QProcess * const pingProcess = nullptr;
    ClientState_t state = ClientState_t::DISCONNECTED;
};

}

#endif // CLIENTPINGER_H
