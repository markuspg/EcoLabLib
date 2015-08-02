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

ellSession::ellSession( const QString &argAnonymousReceiptsPlaceholder, QVector< ellClient* > * const argAssociatedClients,
                        const QString &argLatexHeaderName, const bool &argAnonReceipts,
                        const ellSettingsStorage * const argSettingsStorage, const QString &argzTreeDataTargetPath,
                        const int argzTreePort, const QString &argzTreeVersionString, QObject *argParent ) :
    QObject{ argParent },
    anonymousReceiptsPlaceholder{ new QString { argAnonymousReceiptsPlaceholder } },
    associatedClients{ argAssociatedClients },
    latexHeaderName{ new QString{ argLatexHeaderName } },
    printAnonymousReceipts{ argAnonReceipts },
    settingsStorage{ argSettingsStorage },
    zTreeDataTargetPath{ new QString{ argzTreeDataTargetPath } },
    zTreePort{ argzTreePort },
    zTreeVersionString{ argzTreeVersionString }
{
    // If in the last three seconds of a minute, wait for the next one to start,
    // so that z-Tree and the receipts printer have surely the same minute of start
    if ( QTime::currentTime().second() > 56 ) {
        QTimer::singleShot( 5000, this, SLOT( InitializeClasses() ) );
    } else {
        InitializeClasses();
    }

#ifdef Q_OS_LINUX
    if ( settingsStorage->wmctrlCommand ) {
        QTimer::singleShot( 5000, this, SLOT( RenameWindow() ) );
    }
#endif
}

ellSession::~ellSession() {
    delete anonymousReceiptsPlaceholder;
    for ( auto s : *associatedClients ) {
        s->SetSessionPort();
        s->SetzLeafVersion();
    }
    delete latexHeaderName;
    delete zTreeDataTargetPath;
}

QVariant ellSession::GetDataItem( int argIndex ) {
    switch ( argIndex ) {
    case 0:
        return QVariant{ zTreeVersionString.split( '_', QString::KeepEmptyParts, Qt::CaseInsensitive )[ 1 ] };
    case 1:
        return QVariant{ zTreePort };
    default:
        return QVariant{};
    }
}

void ellSession::InitializeClasses() {
    // Create the new data directory
    QDir dir{ *zTreeDataTargetPath };
    QString dateString( QDateTime::currentDateTime().toString( "yyMMdd_hhmm" ) );
    zTreeDataTargetPath->append( "/" + dateString + "-" + QString::number( zTreePort ) );
    if ( !dir.mkpath( *zTreeDataTargetPath ) ) {
        true;
    }

    zTreeInstance = new ellzTree{ settingsStorage, *zTreeDataTargetPath, zTreePort, zTreeVersionString, this };
    connect( zTreeInstance, SIGNAL( zTreeClosed( int,QProcess::ExitStatus ) ),
             this, SLOT( zTreeClosed() ) );
    // Only create a 'Receipts_Handler' instance, if all neccessary variables were set
    if ( *latexHeaderName != "None found" && settingsStorage->dvipsCommand && settingsStorage->latexCommand ) {
        receiptsCreator = new ellReceiptsCreator{ anonymousReceiptsPlaceholder, printAnonymousReceipts, dateString, latexHeaderName,
                                                  QString{ *zTreeDataTargetPath + "/" + dateString + ".pay" },
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
