#ifndef SETTINGSSTORAGE_H
#define SETTINGSSTORAGE_H

#include "builder.h"

#include <QObject>

class ellSettingsStorage : public QObject
{
    Q_OBJECT
public:
    explicit ellSettingsStorage( const ellBuilder &argBuilder, QObject *argParent = nullptr );
    ~ellSettingsStorage();

    const QStringList * const adminUsers = nullptr;
    const QString * const browserCommand = nullptr;
    const quint16 * const defaultReceiptIndex = nullptr;
    const QString * const dvipsCommand = nullptr;
    const QString * const ecolablibInstallationDirectory = nullptr;
    const QString * const fileManager = nullptr;
    const quint16 * const initialzTreePort = nullptr;
    const QString * const latexCommand = nullptr;
    const QString * const lprCommand = nullptr;
    const QString * const netstatCommand = nullptr;
    const QString * const networkBroadcastAddress = nullptr;
    const QString * const orseeURL = nullptr;
    const QString * const pingCommand = nullptr;
    const QString * const postscriptViewer = nullptr;
    const QProcessEnvironment * const processEnvironment = nullptr;
    const QString * const ps2pdfCommand = nullptr;
    const QString * const publicKeyPathRoot = nullptr;
    const QString * const publicKeyPathUser = nullptr;
    const QString * const rcpCommand = nullptr;
    const QString * const rmCommand = nullptr;
    const QString * const serverIP = nullptr;
    const quint16 * const serverPort = nullptr;
    const QString * const sshCommand = nullptr;
    const QString * const terminalEmulatorCommand = nullptr;
    const QString * const userNameOnClients = nullptr;
    const QString * const vncViewer = nullptr;
    const QString * const wakeonlanCommand = nullptr;
    const QStringList * const webcamNames = nullptr;
    const QStringList * const webcamURLs = nullptr;
    const QString * const wmctrlCommand = nullptr;
    const QString * const xsetCommand = nullptr;
    const QString * const zTreeInstallationDirectory = nullptr;

    QString *GetLocalzLeafName() const { return localzLeafName; }
    void SetLocalzLeafName( QString *argLocalzLeafName ) { localzLeafName = argLocalzLeafName; }

signals:

public slots:

private:
    QString *localzLeafName = nullptr;
};

#endif // SETTINGSSTORAGE_H
