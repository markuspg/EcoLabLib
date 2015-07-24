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
    QString program;
    QStringList arguments;
#ifdef Q_OS_UNIX
    program = *argSettingsStorage->wineCommand;
    arguments.append( QString{ *argSettingsStorage->zTreeInstallationDirectory + "/zTree_" + argZTreeVersionPath + "/ztree.exe" } );
#else
    program = QString{ *argSettingsStorage->zTreeInstallationDirectory + "/zTree_" + argZTreeVersionPath + "/ztree.exe" };
#endif
    arguments << "/datadir" << QString{ "Z:/" + argZTreeDataTargetPath } << "/privdir" << QString{ "Z:/" + argZTreeDataTargetPath }
              << "/gsfdir" << QString{ "Z:/" + argZTreeDataTargetPath } << "/tempdir" << QString{ "Z:/" + argZTreeDataTargetPath }
              << "/leafdir" << QString{ "Z:/" + argZTreeDataTargetPath } << "/channel" << QString::number( argZTreePort - 7000 );

    ztreeInstance.setProcessEnvironment( *argSettingsStorage->processEnvironment );
    ztreeInstance.startDetached( program, arguments, QDir::currentPath() );
}
