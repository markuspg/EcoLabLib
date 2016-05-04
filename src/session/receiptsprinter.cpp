/*
 * Copyright 2015-2016 Markus Prasser
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

#include "receiptsprinter.h"

ell::ReceiptsPrinter::ReceiptsPrinter( const QString * const argDateString, const QString * const argzTreeDataTargetPath,
                                        const SettingsStorage * const argSettingsStorage, QObject *argParent ) :
    QThread{ argParent },
    dateString{ argDateString },
    settingsStorage{ argSettingsStorage },
    zTreeDataTargetPath{ argzTreeDataTargetPath }
{
}

void ell::ReceiptsPrinter::run() {
    if ( !settingsStorage->latexCommand ) {
        return;
    }
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

    if ( !settingsStorage->dvipsCommand ) {
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
