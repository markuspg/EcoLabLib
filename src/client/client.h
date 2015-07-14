#ifndef CLIENT_H
#define CLIENT_H

#include "../settingsstorage.h"

#include <QObject>
#include <QTcpSocket>

enum class ellClientState_t : unsigned short int {
    BOOTING,
    CONNECTED,
    DISCONNECTED,
    SHUTTING_DOWN,
    ZLEAF_RUNNING
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

    //! This function gets frequently polled by Labcontrol to update the table view showing the clients
    ellClientState_t GetClientState() const { return state; }

    void Boot();
    void KillzLeaf();
    void SetSocket( QTcpSocket *argSocket );
    void Shutdown();
    void StartzLeaf();

signals:

public slots:
    void Disconnected();

private:
    const ellSettingsStorage * const settingsStorage = nullptr;
    QTcpSocket *socket = nullptr;
    ellClientState_t state = ellClientState_t::DISCONNECTED;

    void SendMessage( const quint16 &argMessageID, QString *argMessage = nullptr );

private slots:
    void ReadMessage();
};

#endif // CLIENT_H
