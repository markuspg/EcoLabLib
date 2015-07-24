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

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QTimer>
#include <QVector>

class ellSettingsStorage;

class ellReceiptsCreator : public QObject {
    Q_OBJECT
public:
    explicit ellReceiptsCreator( const QString &argDateString, const QString &argPaymentFilePath,
                                 const QString &argPort, const ellSettingsStorage * const argSettingsStorage,
                                 QObject *argParent = 0 );

    const QString dateString;
    const QString port;
    const ellSettingsStorage * const settingsStorage;
signals:

public slots:

private:
    QTimer fileCheckTimer;
    QFile paymentFile;

    void CreateReceiptsFromPaymentFile();
    QVector<QString> *GetParticipantsDataFromPaymentFile();

private slots:
    void PrintReceipts();
};

#endif // RECEIPTSCREATOR_H
