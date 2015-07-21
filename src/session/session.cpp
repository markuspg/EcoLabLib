#include "session.h"

ellSession::ellSession( const QString &argAnonymousReceiptsPlaceholder, const QString &argLatexHeaderName, const bool &argAnonReceipts,
                        const ellSettingsStorage * const argSettingsStorage, const QString &argzTreeDataTargetPath,
                        const int argzTreePort, const QString &argzTreeVersionPath, QObject *argParent ) :
    QObject{ argParent },
    anonymousReceiptsPlaceholder{ argAnonymousReceiptsPlaceholder },
    latexHeaderName{ argLatexHeaderName },
    printAnonymousReceipts{ argAnonReceipts },
    settingsStorage{ argSettingsStorage },
    zTreeDataTargetPath{ argzTreeDataTargetPath },
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
    QDir dir{ zTreeDataTargetPath };
    QString date_string( QDateTime::currentDateTime().toString( "yyMMdd_hhmm" ) );
    if ( !dir.mkpath( zTreeDataTargetPath + "/" + date_string + "-" + QString::number( zTreePort ) ) ) {
        true;
    }
    zTreeDataTargetPath.append( "/" + date_string + "-" + QString::number( zTreePort ) );

    zTreeInstance = new ellzTree{ settingsStorage, zTreeDataTargetPath, zTreePort, zTreeVersionPath, this };
    // Only create a 'Receipts_Handler' instance, if all neccessary variables were set
    if ( latexHeaderName != "None found" && settingsStorage->dvipsCommand && settingsStorage->latexCommand ) {
        // receiptsHandler = new lcReceiptsHandler{ debugMessagesTextEdit, zTreeDataTargetPath, printReceiptsForLocalClients, anonymousReceiptsPlaceholder, latexHeaderName, settingsItems };
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
