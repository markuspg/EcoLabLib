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
    settingsStorage{ argBuilder, this }
{
    DetectInstalledZTreeVersionsAndLaTeXHeaders();
}

EcoLabLib::~EcoLabLib() {
    delete installedLaTeXHeaders;
    delete installedzTreeVersions;
}

void EcoLabLib::DetectInstalledZTreeVersionsAndLaTeXHeaders() {
    // Detect the installed LaTeX headers
    if ( settingsStorage.ecolablibInstallationDirectory ) {
        QDir latexDirectory{ *settingsStorage.ecolablibInstallationDirectory,
                    "*_header.tex", QDir::Name, QDir::CaseSensitive | QDir::Files | QDir::Readable };
        if ( !latexDirectory.exists() || latexDirectory.entryList().isEmpty() ) {
            true;
            installedLaTeXHeaders = new QStringList{ "None found" };
        } else {
            installedLaTeXHeaders = new QStringList{ latexDirectory.entryList() };
            installedLaTeXHeaders->replaceInStrings( "_header.tex", "" );
        }
    }

    // Detect the installed zTree versions
    if ( settingsStorage.zTreeInstallationDirectory ) {
        QDir zTreeDirectory{ *settingsStorage.zTreeInstallationDirectory, "zTree_*", QDir::Name,
                    QDir::NoDotAndDotDot | QDir::Dirs | QDir::Readable | QDir::CaseSensitive };
        if ( zTreeDirectory.entryList().isEmpty() ) {
            true;
        }
        else {
            installedzTreeVersions = new QStringList{ zTreeDirectory.entryList() };
            installedzTreeVersions->replaceInStrings( "zTree_", "" );
        }
    }
}

bool EcoLabLib::ShowORSEE() {
    QProcess showORSEEProcess;
    showORSEEProcess.setProcessEnvironment( *settingsStorage.processEnvironment );
    return showORSEEProcess.startDetached( *settingsStorage.browserCommand,
                                           QStringList{ *settingsStorage.orseeURL } );
}

bool EcoLabLib::ShowPreprints() {
    QProcess showPreprintsProcess;
    showPreprintsProcess.setProcessEnvironment( *settingsStorage.processEnvironment );
    QStringList arguments{ QStringList{} << *settingsStorage.ecolablibInstallationDirectory +  "/preprints" };
    return showPreprintsProcess.startDetached( *settingsStorage.fileManager, arguments );
}
