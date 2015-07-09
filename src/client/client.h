#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class ellSettingsStorage;

enum class ellClientState : unsigned short int {
    disconnected,
    connected
};

class ellClient : public QObject
{
    Q_OBJECT
public:
    explicit ellClient( const QString &argHostName, const QString &argIP, const QString &argMAC,
                        const QString &argWebcamAvailable, const QString &argXPosition, const QString &argYPosition,
                        const ellSettingsStorage * const argSettingsStorage, QObject *argParent = nullptr );
    ~ellClient();

    const QString hostName;
    const QString ip;
    const QString mac;
    const bool webcamAvailable = 0;
    const unsigned int xPosition = 1;
    const unsigned int yPosition = 1;

    void SetSocket( QTcpSocket *argSocket );

signals:

public slots:
    void Disconnected();

private:
    const ellSettingsStorage * const settingsStorage = nullptr;
    QTcpSocket *socket = nullptr;
    ellClientState state = ellClientState::disconnected;
};

#endif // CLIENT_H
