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

#include <QDebug>

#include "ecolablib.h"

ell::EcoLabLib::EcoLabLib( const Builder &argBuilder, QObject *argParent ) :
    QObject{ argParent },
    sessionsModel{ new SessionsModel{ this } },
    settingsStorage{ new SettingsStorage{ argBuilder, this } },
    clientManager{ settingsStorage, this },
    userName{ settingsStorage->processEnvironment->value( "USER", "" ) }
{
    qDebug() << "Initializing EcoLabLib";
    CheckIfUserIsAdmin();
    connect( &clientManager, SIGNAL( HelpRequestRetrieved( QStringList* ) ),
             this, SIGNAL( HelpRequestRetrieved( QStringList* ) ) );
}

void ell::EcoLabLib::CheckIfUserIsAdmin() {
    if ( userName == "" ) {
        true;
        userIsAdmin = false;
        qDebug() << "No user name could be queried, admin functionality will be disabled";
        return;
    }

    if ( settingsStorage->adminUsers ) {
        for ( auto s : *settingsStorage->adminUsers ) {
            if ( s == userName ) {
                userIsAdmin = true;
                qDebug() << "User" << userName << "is admin";
                return;
            }
        }
    }

    qDebug() << "User" << userName << "is no admin, admin functionality will be disabled";
    userIsAdmin = false;
}

void ell::EcoLabLib::KillLocalzLeaves() {
    if ( !settingsStorage->killallCommand ) {
        qDebug() << "The 'killall_command' configuration variable"
                    " was not properly set, skipping";
        return;
    }

    QProcess killzLeavesProcess;
    killzLeavesProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
    killzLeavesProcess.startDetached( *settingsStorage->killallCommand,
                                      QStringList{ "zleaf.exe" } );
    qDebug() << *settingsStorage->killallCommand << QStringList{ "zleaf.exe" };
}

bool ell::EcoLabLib::ShowORSEE() {
    if ( settingsStorage->browserCommand && settingsStorage->orseeURL ) {
        QProcess showORSEEProcess;
        showORSEEProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
        return showORSEEProcess.startDetached( *settingsStorage->browserCommand,
                                               QStringList{ *settingsStorage->orseeURL } );
        qDebug() << *settingsStorage->browserCommand << *settingsStorage->orseeURL;
    } else {
        qDebug() << "The 'browser_command' and/or 'orsee_url' configuration variable(s)"
                    " was not properly set, skipping";
    }

    return false;
}

bool ell::EcoLabLib::ShowPreprints() {
    if ( settingsStorage->fileManager && settingsStorage->ecolablibInstallationDirectory ) {
        QProcess showPreprintsProcess;
        showPreprintsProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
        QStringList arguments{ QStringList{} << *settingsStorage->ecolablibInstallationDirectory +  "/preprints" };
        return showPreprintsProcess.startDetached( *settingsStorage->fileManager, arguments );
    }

    return false;
}

bool ell::EcoLabLib::ShowWebcam( const QString &argWebcamURL ) {
    if ( !settingsStorage->webcamDisplayCommand ) {
        return false;
    }

    QProcess showWebcamProcess;
    showWebcamProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
    QStringList arguments{ QStringList{} << argWebcamURL };
    return showWebcamProcess.startDetached( *settingsStorage->webcamDisplayCommand, arguments );
}

void ell::EcoLabLib::StartLocalzLeaf( const QString &argzLeafName, const QString &argzLeafVersion, const int &argzTreePort ) {
    QProcess startLocalzLeafProcess;
    startLocalzLeafProcess.setProcessEnvironment( *settingsStorage->processEnvironment );

    QString program;
    QStringList arguments;

    if ( !settingsStorage->wineCommand || !settingsStorage->zTreeInstallationDirectory ) {
        return;
    }

    program = *settingsStorage->wineCommand;
    arguments.append( *settingsStorage->zTreeInstallationDirectory
                      + "/zTree_" + argzLeafVersion + "/zleaf.exe" );
    arguments << "/server" << "127.0.0.1"
              << "/channel" << QString::number( argzTreePort - 7000 )
              << "/name" << argzLeafName;
    startLocalzLeafProcess.startDetached( program, arguments );
}

void ell::EcoLabLib::StartNewSession( QVector< ell::Client* > * const argAssociatedClients,
                                      const QString &argParticipiantNameReplacement,
                                      const bool &argPrintAnonymousReceipts,
                                      const QString &argReceiptsHeader,
                                      const QString &argzTreeDataTargetPath,
                                      const quint16 &argzTreePort,
                                      const QString &argzTreeVersion ) {
    sessionsModel->push_back( new Session{ argParticipiantNameReplacement, argAssociatedClients, argReceiptsHeader,
                                              argPrintAnonymousReceipts, settingsStorage, argzTreeDataTargetPath, argzTreePort,
                                              argzTreeVersion, this } );
}
