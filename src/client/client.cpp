#include "client.h"

ellClient::ellClient( const QString &argHostName, const QString &argIP, const QString &argMAC,
                      const QString &argWebcamAvailable, const QString &argXPosition, const QString &argYPosition,
                      const ellSettingsStorage * const argSettingsStorage, QObject *argParent ) :
    QObject{ argParent },
    hostName{ argHostName },
    ip{ argIP },
    mac{ argMAC },
    webcamAvailable{ static_cast< bool >( argWebcamAvailable.toUInt() ) },
    xPosition{ argXPosition.toUInt() },
    yPosition{ argYPosition.toUInt() },
    settingsStorage{ argSettingsStorage }
{
}
