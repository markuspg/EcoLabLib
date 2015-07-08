#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

class ellSettingsStorage;

class ellClient : public QObject
{
    Q_OBJECT
public:
    explicit ellClient( const QString &argHostName, const QString &argIP, const QString &argMAC,
                        const QString &argWebcamAvailable, const QString &argXPosition, const QString &argYPosition,
                        const ellSettingsStorage * const argSettingsStorage, QObject *argParent = nullptr );

    const QString hostName;
    const QString ip;
    const QString mac;
    const bool webcamAvailable = 0;
    const unsigned int xPosition = 1;
    const unsigned int yPosition = 1;

signals:

public slots:

private:
    const ellSettingsStorage * const settingsStorage = nullptr;
};

#endif // CLIENT_H
