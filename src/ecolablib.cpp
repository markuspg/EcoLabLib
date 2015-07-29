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
    clientManager{ settingsStorage, this }
{
}

EcoLabLib::~EcoLabLib() {
    delete settingsStorage;
}

void EcoLabLib::KillLocalzLeaves() {
    QProcess killzLeavesProcess;
    killzLeavesProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
#ifdef Q_OS_UNIX
    killzLeavesProcess.startDetached( *settingsStorage->killallCommand, QStringList{ "zleaf.exe" } );
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

void EcoLabLib::StartNewSession( QVector< ellClient* > * const argAssociatedClients, const QString &argParticipiantNameReplacement,
                                 const bool &argPrintAnonymousReceipts, const QString &argReceiptsHeader,
                                 const QString &argzTreeDataTargetPath, const quint16 &argzTreePort, const QString &argzTreeVersion ) {
    sessionsModel->push_back( new ellSession{ argParticipiantNameReplacement, argAssociatedClients, argReceiptsHeader,
                                              argPrintAnonymousReceipts, settingsStorage, argzTreeDataTargetPath, argzTreePort,
                                              argzTreeVersion, this } );
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
#else
    program = QString{ *settingsStorage->zTreeInstallationDirectory
                       + "/zTree_" + argzLeafVersion + "/zleaf.exe" };
#endif
    arguments << "/server" << "127.0.0.1"
              << "/channel" << QString::number( argzTreePort - 7000 )
              << "/name" << argzLeafName;
    startLocalzLeafProcess.startDetached( program, arguments );
}
