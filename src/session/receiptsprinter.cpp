#include "receiptsprinter.h"

ellReceiptsPrinter::ellReceiptsPrinter( const QString * const argDateString, const QString * const argzTreeDataTargetPath,
                                        const ellSettingsStorage * const argSettingsStorage, QObject *argParent ) :
    QThread{ argParent },
    dateString{ argDateString },
    settingsStorage{ argSettingsStorage },
    zTreeDataTargetPath{ argzTreeDataTargetPath }
{
}

void ellReceiptsPrinter::run() {
    // Compile the TeX file to dvi
    QStringList arguments;
    arguments << "-interaction" << "batchmode" << QString{ *dateString + ".tex" };

    std::unique_ptr< QProcess > process{ new QProcess{} };
    process->setProcessEnvironment( *settingsStorage->processEnvironment );
    process->setWorkingDirectory( *zTreeDataTargetPath );
    process->start( *settingsStorage->latexCommand, arguments );
    if( !process->waitForFinished( processTimeOut ) ) {
        true;
        return;
    }

    process.reset( new QProcess{} );
    // Convert the dvi file to postscript
    arguments.clear();
    arguments << "-q*" << "-o" << QString{ *dateString + ".ps" } << QString{ *dateString + ".dvi" };

    process->setProcessEnvironment( *settingsStorage->processEnvironment );
    process->setWorkingDirectory( *zTreeDataTargetPath );
    process->start( *settingsStorage->dvipsCommand, arguments );
    if( !process->waitForFinished( processTimeOut ) ) {
        true;
        return;
    }

    // Print the postscript file
    if ( settingsStorage->lprCommand ) {
        arguments.clear();
        arguments << QString{ *zTreeDataTargetPath + "/" + *dateString + ".ps" };

        process.reset( new QProcess{} );
        process->setProcessEnvironment( *settingsStorage->processEnvironment );
        process->setWorkingDirectory( *zTreeDataTargetPath );
        process->start( *settingsStorage->lprCommand, arguments );
        if( !process->waitForFinished( processTimeOut ) ) {
            true;
        }
    }

    // Convert the postscript file to pdf
    if ( settingsStorage->ps2pdfCommand ) {
        arguments.clear();
        arguments << QString{ *zTreeDataTargetPath + "/" + *dateString + ".ps" }
                  << QString{ *zTreeDataTargetPath + "/" + *dateString + ".pdf" };

        process.reset( new QProcess{} );
        process->setProcessEnvironment( *settingsStorage->processEnvironment );
        process->setWorkingDirectory( *zTreeDataTargetPath );
        process->start( *settingsStorage->ps2pdfCommand, arguments );
        if( !process->waitForFinished( processTimeOut ) ) {
            true;
        }
    }

    // Show the postscript file if the conversion succeeded
    if ( settingsStorage->postscriptViewer ) {
        arguments.clear();
        arguments << QString{ *zTreeDataTargetPath + "/" + *dateString + ".ps" };

        process.reset( new QProcess{} );
        process->setProcessEnvironment( *settingsStorage->processEnvironment );
        process->setWorkingDirectory( *zTreeDataTargetPath );
        process->startDetached( *settingsStorage->postscriptViewer, arguments );
    }

    // Clean up the zTree working path
    if ( settingsStorage->rmCommand ) {
        arguments.clear();
        arguments << QString{ *zTreeDataTargetPath + "/" + *dateString + ".aux" }
                  << QString{ *zTreeDataTargetPath + "/" + *dateString + ".dvi" }
                  << QString{ *zTreeDataTargetPath + "/" + *dateString + ".log" }
                  << QString{ *zTreeDataTargetPath + "/" + *dateString + ".tex" };

        process.reset( new QProcess{} );
        process->setProcessEnvironment( *settingsStorage->processEnvironment );
        process->setWorkingDirectory( *zTreeDataTargetPath );
        process->start( *settingsStorage->rmCommand, arguments);
        if( !process->waitForFinished( processTimeOut ) ) {
            true;
        }
      }

    emit PrintingFinished();
}
