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

    QProcess *process = nullptr;
    process = new QProcess{};
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    process->setProcessEnvironment( env );
    process->setWorkingDirectory( *zTreeDataTargetPath );
    process->start( *settingsStorage->latexCommand, arguments );
    if( !process->waitForFinished( processTimeOut ) ) {
        true;
        delete process;
        process = nullptr;
        return;
    }
    delete process;
    process = nullptr;

    // Convert the dvi file to postscript
    arguments.clear();
    arguments << "-q*" << "-o" << QString{ *dateString + ".ps" } << QString{ *dateString + ".dvi" };

    process = new QProcess{};
    process->setProcessEnvironment( env );
    process->setWorkingDirectory( *zTreeDataTargetPath );
    process->start( *settingsStorage->dvipsCommand, arguments );
    if( !process->waitForFinished( processTimeOut ) ) {
        true;
        delete process;
        process = nullptr;
        return;
    }
    delete process;
    process = nullptr;

    // Print the postscript file
    if ( settingsStorage->lprCommand ) {
        arguments.clear();
        arguments << QString{ *zTreeDataTargetPath + "/" + *dateString + ".ps" };

        process = new QProcess{};
        process->setProcessEnvironment( env );
        process->setWorkingDirectory( *zTreeDataTargetPath );
        process->start( *settingsStorage->lprCommand, arguments );
        if( !process->waitForFinished( processTimeOut ) ) {
            true;
        }
        delete process;
        process = nullptr;
    }

    // Convert the postscript file to pdf
    if ( settingsStorage->ps2pdfCommand ) {
        arguments.clear();
        arguments << QString{ *zTreeDataTargetPath + "/" + *dateString + ".ps" }
                  << QString{ *zTreeDataTargetPath + "/" + *dateString + ".pdf" };

        process = new QProcess{};
        process->setProcessEnvironment( env );
        process->setWorkingDirectory( *zTreeDataTargetPath );
        process->start( *settingsStorage->ps2pdfCommand, arguments );
        if( !process->waitForFinished( processTimeOut ) ) {
            true;
        }
        delete process;
        process = nullptr;

        // Show the postscript file if the conversion succeeded
        if ( settingsStorage->postscriptViewer ) {
            arguments.clear();
            arguments << QString{ *zTreeDataTargetPath + "/" + *dateString + ".ps" };

            process = new QProcess{};
            process->setProcessEnvironment( env );
            process->setWorkingDirectory( *zTreeDataTargetPath );
            process->startDetached( *settingsStorage->postscriptViewer, arguments );
            delete process;
            process = nullptr;
        }
    }

    // Clean up the zTree working path
    if ( settingsStorage->rmCommand ) {
        arguments.clear();
        arguments << QString{ *zTreeDataTargetPath + "/" + *dateString + ".aux" }
                  << QString{ *zTreeDataTargetPath + "/" + *dateString + ".dvi" }
                  << QString{ *zTreeDataTargetPath + "/" + *dateString + ".log" }
                  << QString{ *zTreeDataTargetPath + "/" + *dateString + ".tex" };

        process = new QProcess{};
        process->setProcessEnvironment( env );
        process->setWorkingDirectory( *zTreeDataTargetPath );
        process->start( *settingsStorage->rmCommand, arguments);
        if( !process->waitForFinished( processTimeOut ) ) {
            true;
        }
        delete process;
        process = nullptr;
      }

    emit PrintingFinished();
}
