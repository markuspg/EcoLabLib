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

#ifndef RECEIPTSCREATOR_H
#define RECEIPTSCREATOR_H

#include "receiptsprinter.h"
#include "../settingsstorage.h"

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QTimer>
#include <QVector>

//! A struct representing one payoff entry.
/*!
  This class represents a single payoff entry which will be used in the receipts creation process. Multiple instances of this will be used to represent the individual participants' outcomes.
*/
struct ellPaymentEntry_t { QString computer; QString name; double payoff; };

class ellReceiptsCreator : public QObject {
    Q_OBJECT
public:
    explicit ellReceiptsCreator( const QString * const argAnonymousReceiptsPlaceholder, const bool &argAnonReceipts,
                                 const QString &argDateString, const QString * const argLaTeXHeaderName,
                                 const QString &argPaymentFilePath, const QString &argPort,
                                 const ellSettingsStorage * const argSettingsStorage,
                                 const QString * const argzTreeDataTargetPath,
                                 QObject *argParent = 0 );
    ~ellReceiptsCreator();

signals:
    void PrintingFinished();

public slots:

private:
    const bool anonymousReceipts = true;                            //! If anymouse receipts shall be printed (true) or not (false)
    const QString * const anonymousReceiptsPlaceholder = nullptr;   //! Placeholder which shall be inserted for participant names if anonymous printing is desired (QString != "")
    const QString * const dateString = nullptr;                     //! The date string of the session to be printed ("YYMMDD_hhmm")
    ellReceiptsPrinter *receiptsPrinter = nullptr;                  //! Prints the created receipts in another thread
    QString expectedPaymentFileName;                                //! The name of the payment file whose appearence will be regularily queried
    QTimer fileCheckTimer;                                          //! The timer initiating the checks for the payment file existance
    const QString * const latexHeaderName = nullptr;                //! The name of the LaTeX header file to be used for receipts creation
    QFile paymentFile;
    const QString port;                                             //! The port the containing session is running on
    const ellSettingsStorage * const settingsStorage;
    const QString * const zTreeDataTargetPath = nullptr;            //! The path were the data of this zTree instance's session will be saved

    void CreateReceiptsFromPaymentFile();
    //! Extracts the data of the participants whose receipts shall be printed
    QVector< ellPaymentEntry_t* > *ExtractParticipantsData( double &argOverallPayoff, QVector<QString> *argRawData );
    QVector<QString> *GetParticipantsDataFromPaymentFile();
    QString *LoadLatexHeader();
    void MakeReceiptsAnonymous( bool argAlsoAnonymizeClients, QVector< ellPaymentEntry_t* > *argDataVector );

private slots:
    //! Deletes the ellReceiptsPrinter instance after successful printing
    void DeleteReceiptsPrinterInstance();
    //! Prints the receipts, if the payment file exists
    void PrintReceipts();
};

#endif // RECEIPTSCREATOR_H
