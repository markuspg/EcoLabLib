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

#include "clientpinger.h"

ell::ClientPinger::ClientPinger( const QString * const argIP,
                                 const QString * const argPingCommand,
                                 QObject *argParent ) :
    QThread{ argParent },
    pingArguments{ QStringList{} << "-c" << "1" << "-w" << "1" << "-q" << *argIP },
    pingCommand{ argPingCommand },
    pingProcess{ new QProcess{ this } }
{
    pingProcess->setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
}

void ell::ClientPinger::run() {
    ClientState_t newState = ClientState_t::UNINITIALIZED;

    while ( true ) {
        if ( isInterruptionRequested() ) {
            return;
        } else {
            pingProcess->start( *pingCommand, pingArguments );
            if ( !pingProcess->waitForFinished( 4096 ) || pingProcess->exitCode() != 0 ) {
                newState = ClientState_t::DISCONNECTED;
            } else {
                newState = ClientState_t::CONNECTED;
            }
        }

        if ( newState != state ) {
            state = newState;
            emit stateChanged( state );
        }

        QThread::sleep( 1 );
    }
}