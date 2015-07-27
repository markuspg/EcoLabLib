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

#include "session.h"

ellSession::ellSession( const QString &argAnonymousReceiptsPlaceholder, const QString &argLatexHeaderName, const bool &argAnonReceipts,
                        const ellSettingsStorage * const argSettingsStorage, const QString &argzTreeDataTargetPath,
                        const int argzTreePort, const QString &argzTreeVersionPath, QObject *argParent ) :
    QObject{ argParent },
    anonymousReceiptsPlaceholder{ new QString { argAnonymousReceiptsPlaceholder } },
    latexHeaderName{ new QString{ argLatexHeaderName } },
    printAnonymousReceipts{ argAnonReceipts },
    settingsStorage{ argSettingsStorage },
    zTreeDataTargetPath{ new QString{ argzTreeDataTargetPath } },
    zTreePort{ argzTreePort },
    zTreeVersionPath{ argzTreeVersionPath }
{
    // This part ensures, that both class instances are created in the same minute, so that the payment file name can be guessed correctly
    QDateTime current_time;
    current_time  = QDateTime::currentDateTime();

    // If in the last three seconds of a minute, wait for the next one to start
    if ( QTime::currentTime().second() > 56 ) {
        QTimer::singleShot( 5000, this, SLOT( InitializeClasses() ) );
    } else {
        InitializeClasses();
    }

    if ( settingsStorage->wmctrlCommand ) {
        QTimer::singleShot( 5000, this, SLOT( RenameWindow() ) );
    }
}

ellSession::~ellSession() {
    delete anonymousReceiptsPlaceholder;
    delete latexHeaderName;
    delete zTreeDataTargetPath;
}

QVariant ellSession::GetDataItem( int argIndex ) {
    switch ( argIndex ) {
    case 0:
        return QVariant{ zTreeVersionPath.split( '_', QString::KeepEmptyParts, Qt::CaseInsensitive )[ 1 ] };
    case 1:
        return QVariant{ zTreePort };
    default:
        return QVariant{};
    }
}

void ellSession::InitializeClasses() {
    // Create the new data directory
    QDir dir{ *zTreeDataTargetPath };
    QString date_string( QDateTime::currentDateTime().toString( "yyMMdd_hhmm" ) );
    if ( !dir.mkpath( *zTreeDataTargetPath + "/" + date_string + "-" + QString::number( zTreePort ) ) ) {
        true;
    }
    zTreeDataTargetPath->append( "/" + date_string + "-" + QString::number( zTreePort ) );

    zTreeInstance = new ellzTree{ settingsStorage, *zTreeDataTargetPath, zTreePort, zTreeVersionPath, this };
    connect( zTreeInstance, SIGNAL( zTreeClosed( int,QProcess::ExitStatus ) ),
             this, SLOT( zTreeClosed() ) );
    // Only create a 'Receipts_Handler' instance, if all neccessary variables were set
    if ( *latexHeaderName != "None found" && settingsStorage->dvipsCommand && settingsStorage->latexCommand ) {
        receiptsCreator = new ellReceiptsCreator{ anonymousReceiptsPlaceholder, date_string, latexHeaderName,
                                                  QString{ *zTreeDataTargetPath + "/" + date_string + ".pay" },
                                                  QString::number( zTreePort ), settingsStorage,
                                                  zTreeDataTargetPath, this };
        true;
    } else {
        true;
    }
}

void ellSession::RenameWindow() {
    // Example: wmctrl -r <window name> -T <new name>

    QStringList arguments;
    arguments << "-r" << "zTree - Untitled Treatment 1" << "-T" << QString{ "zTree on port " + QString::number( zTreePort ) };

    // Start the process
    QProcess renameZTreeWindowProcess;
    renameZTreeWindowProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
    renameZTreeWindowProcess.startDetached( *settingsStorage->wmctrlCommand, arguments );
}

void ellSession::zTreeClosed() {
    emit SessionFinished( this );
}
