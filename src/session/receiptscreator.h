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

//! Creates the receipts from the payment file created by z-Tree
/*!
  This class frequently checks the z-Tree data target path for the appearance of the payment file. After
  this a LaTeX file for the receipts printing process will be created and the 'ellReceiptsPrinter'
  will be instanciated.
 */
class ellReceiptsCreator : public QObject {
    Q_OBJECT
public:
    //! 'ellReceiptsCreator's constructor which will be called by 'ellSession'
    /*!
       This constructor stores all needed settings, especially the expected payment file name
       and starts a timer which frequently checks for the appearance of the payment file.
       \param argAnonymousReceiptsPlaceholder Placeholder which shall be inserted for participant names if anonymous printing is desired
       \param argAnonReceipts If anymous receipts shall be printed (true) or not (false)
       \param argDateString The date string of the session to be printed ("yyMMdd_hhmm")
       \param argLaTeXHeaderName The name of the LaTeX header file to be used for receipts creation
       \param argPaymentFilePath The path where the payment file is expected to appear
       \param argSettingsStorage The 'ellSettingsStorage' containing all settings
       \param argzTreeDataTargetPath The path were z-Tree shall store its data (e.g. the gamesafe file)
       \param argParent 'ellReceiptsCreator's parent object
     */
    explicit ellReceiptsCreator( const QString * const argAnonymousReceiptsPlaceholder, const bool &argAnonReceipts,
                                 const QString &argDateString, const QString * const argLaTeXHeaderName,
                                 const QString &argPaymentFilePath, const ellSettingsStorage * const argSettingsStorage,
                                 const QString * const argzTreeDataTargetPath, QObject *argParent = 0 );
    //! Cleans up the 'ellReceiptsCreator'
    ~ellReceiptsCreator();

signals:
    //! This signal becomes emitted, if the printing process finished
    void PrintingFinished();

public slots:

private:
    const bool anonymousReceipts = true;                            //! If anymous receipts shall be printed (true) or not (false)
    const QString * const anonymousReceiptsPlaceholder = nullptr;   //! Placeholder which shall be inserted for participant names if anonymous printing is desired
    const QString * const dateString = nullptr;                     //! The date string of the session to be printed ("YYMMDD_hhmm")
    ellReceiptsPrinter *receiptsPrinter = nullptr;                  //! Prints the created receipts in another thread
    QString expectedPaymentFileName;                                //! The name of the payment file whose appearence will be regularily queried
    QTimer fileCheckTimer;                                          //! The timer initiating the checks for the payment file existance
    const QString * const latexHeaderName = nullptr;                //! The name of the LaTeX header file to be used for receipts creation
    QFile paymentFile;                                              //! A QFile object representing the expected payment file
    const ellSettingsStorage * const settingsStorage;               //! Contains all external settings
    const QString * const zTreeDataTargetPath = nullptr;            //! The path were the data of this zTree instance's session will be saved

    //! Extracts the data of the participants whose receipts shall be printed
    /*!
       This function parses the payment file and extracts and returns all data relevant for the receipts.
       \param argOverallPayoff In this variable the overall payoff of this session will be stored
       \param argRawData A QVector containing the raw payoff data as a QStrings, will be deleted and nulled in the process
       \return A QVector containing the payoff data stored in a nice structured way
     */
    QVector< ellPaymentEntry_t* > *ExtractParticipantsData( double &argOverallPayoff,
                                                            QList< QString > *& argRawData );
    //! This function extracts all relevant lines from the payment file
    QList< QString > *GetParticipantsDataFromPaymentFile();
    //! This function loads the LaTeX header from its file in the 'EcoLabLib' installation directory
    QString *LoadLatexHeader();
    //! This function anonymizes the receipts data used in the process
    /*!
     * This function anonymizes the data for the receipts whilst the receipts creation process.This happens in two
     * steps, so that the client numbers are on the overview for the experimenter, but not on the participant receipts.
     * \param argAlsoAnonymizeClients This should be true for the second run, when also the client number shall be anonymized
     * \param argDataVector The vector containing the already structured payoff data
     */
    void MakeReceiptsAnonymous( bool argAlsoAnonymizeClients, QVector< ellPaymentEntry_t* > *argDataVector );

private slots:
    //! This function creates a LaTeX receipts file from the appeared payment file
    void CreateReceiptsFromPaymentFile();
    //! Deletes the ellReceiptsPrinter instance after successful printing
    void DeleteReceiptsPrinterInstance();
    //! Prints the receipts, if the payment file exists
    void PrintReceipts();
};

#endif // RECEIPTSCREATOR_H
