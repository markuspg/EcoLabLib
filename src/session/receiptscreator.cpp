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

#include "receiptscreator.h"

ellReceiptsCreator::ellReceiptsCreator( const QString * const argAnonymousReceiptsPlaceholder, const bool &argAnonReceipts,
                                        const QString &argDateString, const QString * const argLaTeXHeaderName,
                                        const QString &argPaymentFilePath, const QString &argPort,
                                        const ellSettingsStorage * const argSettingsStorage,
                                        const QString * const argzTreeDataTargetPath,
                                        QObject *argParent ) :
    QObject{ argParent },
    anonymousReceipts{ argAnonReceipts },
    anonymousReceiptsPlaceholder{ argAnonymousReceiptsPlaceholder },
    dateString{ new QString{ argDateString } },
    fileCheckTimer{ this },
    latexHeaderName{ argLaTeXHeaderName },
    paymentFile{ argPaymentFilePath },
    port{ argPort },
    settingsStorage{ argSettingsStorage },
    zTreeDataTargetPath{ argzTreeDataTargetPath }
{
    expectedPaymentFileName = paymentFile.fileName().split( '/', QString::KeepEmptyParts ).back();

    connect( &fileCheckTimer, &QTimer::timeout, this, &ellReceiptsCreator::PrintReceipts );
    fileCheckTimer.start( 1000 );
}

ellReceiptsCreator::~ellReceiptsCreator() {
    if ( receiptsPrinter ) {
        receiptsPrinter->quit();
        receiptsPrinter->wait();
        delete receiptsPrinter;
        receiptsPrinter = nullptr;
    }
    delete dateString;
}

void ellReceiptsCreator::CreateReceiptsFromPaymentFile() {
    // Get the data needed for receipts creation from the payment file
    QVector< QString > *rawParticipantsData = nullptr;
    rawParticipantsData = GetParticipantsDataFromPaymentFile();

    double overallPayoff = 0.0;
    QVector< ellPaymentEntry_t* > *participants = ExtractParticipantsData( overallPayoff, rawParticipantsData );
    delete rawParticipantsData;
    rawParticipantsData = nullptr;

    /* Make receipts overview anonymous if requested
     * (at this stage just names are removed, so that the overview still contains the client names)
     */
    if ( !anonymousReceiptsPlaceholder->isEmpty() ) {
        MakeReceiptsAnonymous( false, participants );
    }

    // Load the LaTeX header
    QString *latexText = LoadLatexHeader();
    if ( latexText == nullptr ) {
        for ( auto s : *participants ) {
            delete s;
        }
        delete participants;
        participants = nullptr;
        return;
    }

    // Write the comprehension table
    latexText->append( "\n\\COMPREHENSION{\n" );
    unsigned short int line = 0;
    for ( auto s : *participants ) {
        latexText->append( expectedPaymentFileName + " & " + s->computer + " & " + s->name + " & " + QString::number( s->payoff, 'f', 2 ) + " \\EUR\\\\\n" );
        if ( line % 2 == 0 ) {
            latexText->append( "\\rowcolor[gray]{0.9}\n" );
        }
        ++line;
    }

    // MISSING: Appending show up entries to the overview

    /* Make also the clients on the receipts anonymous.
     * This is done as second step, so that the beforehand created overview still contains the clients
     */
    if ( !anonymousReceiptsPlaceholder->isEmpty() ) {
        MakeReceiptsAnonymous( true, participants );
    }

    // Add the LaTeX middle sequence
    latexText->append( "}{" + QString::number( overallPayoff, 'f', 2 ) + "}\n\n%%Einzelquittungen\n" );

    // Write the single receipts
    for ( auto s : *participants ) {
        if ( s->payoff >= 0 ) {
            latexText->append( "\\GAINRECEIPT{" + expectedPaymentFileName + "}{" + s->computer + "}{" + s->name + "}{" + QString::number( s->payoff, 'f', 2 ) + "}\n" );
        }
        else {
            latexText->append( "\\LOSSRECEIPT{" + expectedPaymentFileName + "}{" + s->computer + "}{" + s->name + "}{" + QString::number( s->payoff, 'f', 2 ) + "}\n" );
        }
        delete s;
    }
    delete participants;
    participants = nullptr;

    // Append LaTeX ending
    latexText->append( "\\end{document}" );

    // Create the tex file
    QFile *texFile = new QFile( *zTreeDataTargetPath + "/" + *dateString + ".tex" );
    // Clean up any already existing files
    if ( texFile->exists() ) {
        if ( !texFile->remove() ) {
            true;
            delete latexText;
            return;
        }
    }
    // Create a new file
    if ( !texFile->open( QIODevice::Text | QIODevice::WriteOnly ) ) {
        true;
        delete latexText;
        return;
    }

    // Open a QTextStream to write to the file
    QTextStream out( texFile );

    out << *latexText;
    delete latexText;
    latexText = nullptr;

    receiptsPrinter = new ellReceiptsPrinter{ dateString, zTreeDataTargetPath, settingsStorage, this };
    receiptsPrinter->start();
    connect( receiptsPrinter, &ellReceiptsPrinter::PrintingFinished,
             this, &ellReceiptsCreator::DeleteReceiptsPrinterInstance );
    // connect( receiptsPrinter, &lcReceiptsPrinter::ErrorOccurred, this, &lcReceiptsHandler::DisplayMessageBox );

    // Clean up
    texFile->close();
    delete texFile;
}

void ellReceiptsCreator::DeleteReceiptsPrinterInstance() {
    receiptsPrinter->quit();
    receiptsPrinter->wait();
    delete receiptsPrinter;
    receiptsPrinter = nullptr;

    emit PrintingFinished();
}

QVector< ellPaymentEntry_t* > *ellReceiptsCreator::ExtractParticipantsData( double &argOverallPayoff, QVector<QString> *argRawData ) {
    /* The tab separated fields in the payment file are:
     * SUBJECT  COMPUTER    INTERESTED  NAME    PROFIT  SIGNATURE
     */
    QVector< ellPaymentEntry_t* > *participants = new QVector< ellPaymentEntry_t* >;
    for ( QVector<QString>::const_iterator cit = argRawData->cbegin(); cit != argRawData->cend() - 1; ++cit ) {
        // Split the lines containing the participants' data into their inidivual parts
        QStringList tempParticipantData = cit->split( '\t', QString::KeepEmptyParts );
        // Create a new struct instance for the participant's data and fill it
        ellPaymentEntry_t *participant  = new ellPaymentEntry_t;
        participant->computer = tempParticipantData.at( 1 );
        participant->name = tempParticipantData.at( 3 );
        participant->payoff = tempParticipantData.at( 4 ).toDouble();
        argOverallPayoff += participant->payoff;
        participants->append( participant );
    }

    return participants;
}

QVector<QString> *ellReceiptsCreator::GetParticipantsDataFromPaymentFile() {
    // Create the vector to store the single lines of the file
    QVector<QString> *participantsData = nullptr;

    // Open the payment file for reading and create a QTextStream
    if ( paymentFile.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        participantsData = new QVector< QString >;
    } else {
        return participantsData;
    }
    QTextStream in( &paymentFile );
    in.setCodec( "ISO 8859-1" );

    // Read the file line by line and store them in the vector
    while ( true ) {
        QString line = in.readLine();
        if ( line.isNull() ) {
            break;
        }
        participantsData->append( line );
    }

    // Remove the first line, since it is not needed
    participantsData->erase( participantsData->begin() );

    // Close the file afterwards
    paymentFile.close();

    return participantsData;
}

QString *ellReceiptsCreator::LoadLatexHeader() {
    // Prepare all facilities to read the latex header file
    QFile latexHeaderFile( *settingsStorage->ecolablibInstallationDirectory + "/" + *latexHeaderName + "_header.tex" );
    if ( !latexHeaderFile.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        true;
        return nullptr;
    }
    QTextStream in( &latexHeaderFile );

    QString *header = new QString( in.readAll() );

    latexHeaderFile.close();

    return header;
}

void ellReceiptsCreator::MakeReceiptsAnonymous( bool argAlsoAnonymizeClients, QVector< ellPaymentEntry_t* > *argDataVector ) {
    if ( !argAlsoAnonymizeClients ) {
        for ( QVector< ellPaymentEntry_t* >::iterator it = argDataVector->begin(); it != argDataVector->end(); ++it ) {
            ( *it )->name = QString{ *anonymousReceiptsPlaceholder };
        }
    }
    else {
       for ( QVector< ellPaymentEntry_t* >::iterator it = argDataVector->begin(); it != argDataVector->end(); ++it ) {
           ( *it )->name = QString{ *anonymousReceiptsPlaceholder };
           ( *it )->computer = QString{ "\\hspace{1cm}" };
       }
    }
}

void ellReceiptsCreator::PrintReceipts() {
    // If the payment file exists, print it
    if ( paymentFile.exists() ) {
        fileCheckTimer.stop();
        CreateReceiptsFromPaymentFile();
    }
}
