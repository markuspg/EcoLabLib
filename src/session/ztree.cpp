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

#include "ztree.h"

ellzTree::ellzTree( const ellSettingsStorage * const argSettingsStorage, const QString &argZTreeDataTargetPath,
                    const int &argZTreePort, const QString &argZTreeVersionPath, QObject *argParent ) :
    QObject{ argParent }
{
    connect( &ztreeInstance, SIGNAL( finished( int, QProcess::ExitStatus ) ),
             this, SIGNAL( zTreeClosed( int, QProcess::ExitStatus ) ) );
    QString program;
    QStringList arguments;
#ifdef Q_OS_UNIX
    if ( !argSettingsStorage->wineCommand || !argSettingsStorage->zTreeInstallationDirectory ) {
        throw std::runtime_error{ "Either the 'wine_command' or the 'ztree_installation_directory' variable were not set."
                                  " Both are mandatory to start z-Tree" };
    }
    program = *argSettingsStorage->wineCommand;
    arguments.append( QString{ *argSettingsStorage->zTreeInstallationDirectory + "/zTree_" + argZTreeVersionPath + "/ztree.exe" } );
    arguments << "/datadir" << QString{ "Z:/" + argZTreeDataTargetPath } << "/privdir" << QString{ "Z:/" + argZTreeDataTargetPath }
              << "/gsfdir" << QString{ "Z:/" + argZTreeDataTargetPath } << "/tempdir" << QString{ "Z:/" + argZTreeDataTargetPath }
              << "/leafdir" << QString{ "Z:/" + argZTreeDataTargetPath };
#else
    program = QString{ *argSettingsStorage->zTreeInstallationDirectory + "/zTree_" + argZTreeVersionPath + "/ztree.exe" };
    arguments << "/datadir" << QString{ argZTreeDataTargetPath } << "/privdir" << QString{ argZTreeDataTargetPath }
              << "/gsfdir" << QString{ argZTreeDataTargetPath } << "/tempdir" << QString{ argZTreeDataTargetPath }
              << "/leafdir" << QString{ argZTreeDataTargetPath };
#endif
    arguments << "/channel" << QString::number( argZTreePort - 7000 );

    ztreeInstance.setProcessEnvironment( *argSettingsStorage->processEnvironment );
    ztreeInstance.setWorkingDirectory( QDir::currentPath() );
    ztreeInstance.start( program, arguments );
}
