#include "receiptscreator.h"

ellReceiptsCreator::ellReceiptsCreator( const QString &argDateString, const QString &argPaymentFilePath,
                                        const QString &argPort, const ellSettingsStorage * const argSettingsStorage,
                                        QObject *argParent ) :
    QObject{ argParent },
    dateString{ argDateString },
    paymentFile{ argPaymentFilePath },
    port{ argPort },
    settingsStorage{ argSettingsStorage },
    fileCheckTimer{ this }
{
    connect( &fileCheckTimer, &QTimer::timeout, this, &ellReceiptsCreator::PrintReceipts );
    fileCheckTimer.start( 1000 );
}

void ellReceiptsCreator::CreateReceiptsFromPaymentFile() {
    // Get the data needed for receipts creation from the payment file
    QVector< QString > *rawParticipantsData = nullptr;
    rawParticipantsData = GetParticipantsDataFromPaymentFile();
}

void ellReceiptsCreator::GetParticipantsDataFromPaymentFile() {
    // Create the vector to store the single lines of the file
    QVector<QString> *participantsData = new QVector< QString >;

    // Open the payment file for reading and create a QTextStream
    paymentFile.open( QIODevice::ReadOnly | QIODevice::Text );
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

void ellReceiptsCreator::PrintReceipts() {
    // If the payment file exists, print it
    if ( paymentFile.exists() ) {
        CreateReceiptsFromPaymentFile();
    }
}
