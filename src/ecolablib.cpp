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

#include "ecolablib.h"


EcoLabLib::EcoLabLib( const ellBuilder &argBuilder, QObject *argParent ) :
    QObject{ argParent },
    sessionsModel{ new ellSessionsModel{ this } },
    settingsStorage{ new ellSettingsStorage{ argBuilder, this } },
    clientManager{ settingsStorage, this },
  #ifdef Q_OS_UNIX
      userName{ settingsStorage->processEnvironment->value( "USER", "" ) }
  #endif
  #ifdef Q_OS_WIN
      userName{ settingsStorage->processEnvironment->value( "USERNAME", "" ) }
  #endif
{
    CheckIfUserIsAdmin();
}

void EcoLabLib::CheckIfUserIsAdmin() {
    if ( userName == "" ) {
        true;
        userIsAdmin = false;
        return;
    }

    if ( !( settingsStorage->adminUsers == nullptr ) ) {
        for ( auto s : *settingsStorage->adminUsers ) {
            if ( s == userName ) {
                userIsAdmin = true;
                return;
            }
        }
    }

    userIsAdmin = false;
}

void EcoLabLib::KillLocalzLeaves() {
    QProcess killzLeavesProcess;
    killzLeavesProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
#ifdef Q_OS_UNIX
    killzLeavesProcess.startDetached( *settingsStorage->killallCommand, QStringList{ "zleaf.exe" } );
#endif
#ifdef Q_OS_WIN
    killzLeafProcess.startDetached( "taskkill",
                                    QStringList{} << "/IM" << "zleaf.exe" );
#endif
}

bool EcoLabLib::ShowORSEE() {
    QProcess showORSEEProcess;
    showORSEEProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
    return showORSEEProcess.startDetached( *settingsStorage->browserCommand,
                                           QStringList{ *settingsStorage->orseeURL } );
}

bool EcoLabLib::ShowPreprints() {
    QProcess showPreprintsProcess;
    showPreprintsProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
    QStringList arguments{ QStringList{} << *settingsStorage->ecolablibInstallationDirectory +  "/preprints" };
    return showPreprintsProcess.startDetached( *settingsStorage->fileManager, arguments );
}

bool EcoLabLib::ShowWebcam( const QString &argWebcamURL ) {
    QProcess showWebcamProcess;
    showWebcamProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
    QStringList arguments{ QStringList{} << argWebcamURL };
    return showWebcamProcess.startDetached( *settingsStorage->webcamDisplayCommand, arguments );
}

void EcoLabLib::StartLocalzLeaf( const QString &argzLeafName, const QString &argzLeafVersion, const int &argzTreePort ) {
    QProcess startLocalzLeafProcess;
    startLocalzLeafProcess.setProcessEnvironment( *settingsStorage->processEnvironment );

    QString program;
    QStringList arguments;
#ifdef Q_OS_UNIX
    program = *settingsStorage->wineCommand;
    arguments.append( *settingsStorage->zTreeInstallationDirectory
                      + "/zTree_" + argzLeafVersion + "/zleaf.exe" );
#endif
#ifdef Q_OS_WIN
    program = QString{ *settingsStorage->zTreeInstallationDirectory
                       + "/zTree_" + argzLeafVersion + "/zleaf.exe" };
#endif
    arguments << "/server" << "127.0.0.1"
              << "/channel" << QString::number( argzTreePort - 7000 )
              << "/name" << argzLeafName;
    startLocalzLeafProcess.startDetached( program, arguments );
}

void EcoLabLib::StartNewSession( QVector< ellClient* > * const argAssociatedClients, const QString &argParticipiantNameReplacement,
                                 const bool &argPrintAnonymousReceipts, const QString &argReceiptsHeader,
                                 const QString &argzTreeDataTargetPath, const quint16 &argzTreePort, const QString &argzTreeVersion ) {
    sessionsModel->push_back( new ellSession{ argParticipiantNameReplacement, argAssociatedClients, argReceiptsHeader,
                                              argPrintAnonymousReceipts, settingsStorage, argzTreeDataTargetPath, argzTreePort,
                                              argzTreeVersion, this } );
}
