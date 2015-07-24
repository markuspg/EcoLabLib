#ifndef RECEIPTSPRINTER_H
#define RECEIPTSPRINTER_H

#include "../settingsstorage.h"

#include <QProcess>
#include <QStringList>
#include <QThread>

class ellReceiptsPrinter : public QThread
{
    Q_OBJECT
public:
    explicit ellReceiptsPrinter( const QString * const argDateString, const QString * const argzTreeDataTargetPath,
                                 const ellSettingsStorage * const argSettingsStorage, QObject *argParent = nullptr );

signals:
    void PrintingFinished();

public slots:

private:
    const QString * const dateString = nullptr;             //! The date string of the session to be printed ("YYMMDD_hhmm")
    const int processTimeOut = 15000;                       //! The maximum runtime which will be granted to a started process
    const ellSettingsStorage * const settingsStorage;
    const QString * const zTreeDataTargetPath = nullptr;    //! The path were the data of this zTree instance's session will be saved

    void run() Q_DECL_OVERRIDE;
};

#endif // RECEIPTSPRINTER_H
