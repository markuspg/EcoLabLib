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
