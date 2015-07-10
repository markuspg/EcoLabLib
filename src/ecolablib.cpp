/*
 * Copyright 2015 Markus Prasser
 *
 * This file is part of Labcontrol.
 *
 *  Labcontrol is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Labcontrol is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Labcontrol.  If not, see <http://www.gnu.org/licenses/>.
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
