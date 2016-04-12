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

#ifndef RECEIPTSPRINTER_H
#define RECEIPTSPRINTER_H

#include "../settingsstorage.h"

#include <QProcess>
#include <QStringList>
#include <QThread>

//! Prints the created receipts LaTeX file in another thread
/*!
  This class is used to print the receipts LaTeX file created by 'ellReceiptsCreator'
  in another thread to prevent the user interface from freezing.
 */
class ellReceiptsPrinter : public QThread
{
    Q_OBJECT
public:
    //! 'ellReceiptsPrinter's constructor which will be called by 'ellReceiptsCreator'
    /*!
       This constructor does nothing else than initializing this class' attributes with the passed ones.
       \param argDateString The date string of the session to be printed ("yyMMdd_hhmm")
       \param argzTreeDataTargetPath The path were the data of this zTree instance's session will be saved
       \param argSettingsStorage The 'ellSettingsStorage' containing all settings
       \param argParent 'ellReceiptsPrinter's parent object
     */
    explicit ellReceiptsPrinter( const QString * const argDateString, const QString * const argzTreeDataTargetPath,
                                 const ellSettingsStorage * const argSettingsStorage, QObject *argParent = nullptr );

signals:
    //! This signal becomes emitted when the printing has finished
    void PrintingFinished();

public slots:

private:
    const QString * const dateString = nullptr;             //! The date string of the session to be printed ("YYMMDD_hhmm")
    const int processTimeOut = 15000;                       //! The maximum runtime which will be granted to a started process
    const ellSettingsStorage * const settingsStorage;       //! Contains all external settings
    const QString * const zTreeDataTargetPath = nullptr;    //! The path were the data of this zTree instance's session will be saved

    //! This function will print the receipts and will be executed in a separate thread
    void run() Q_DECL_OVERRIDE;
};

#endif // RECEIPTSPRINTER_H
