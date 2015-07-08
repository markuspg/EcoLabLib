#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "client.h"
#include "../settingsstorage.h"

#include <QObject>
#include <QSettings>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include <QVector>

class ellSettingsStorage;

class ellClientManager : public QTcpServer
{
    Q_OBJECT
public:
    explicit ellClientManager( const ellSettingsStorage * const argSettingsStorage, QObject *argParent = nullptr );
    ~ellClientManager();

signals:

public slots:

private:
    QMap< QString, ellClient* > * clientIPsToClientsMap = nullptr;
    QVector< ellClient* > clients;
    int clientQuantity = 0;
    const ellSettingsStorage * const settingsStorage;

private slots:
    void HandleIncomingConnection();
};

#endif // CLIENTMANAGER_H
