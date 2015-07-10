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

ellClient::~ellClient() {
    delete socket;
}

void ellClient::Boot() {
    if ( state == ellClientState_t::CONNECTED || state == ellClientState_t::SHUTTING_DOWN ) {
        return;
    }
    QStringList arguments;
    arguments << "-i" << *settingsStorage->networkBroadcastAddress << mac;

    // Start the process
    QProcess wakeonlanProcess;
    wakeonlanProcess.setProcessEnvironment( *settingsStorage->processEnvironment );
    wakeonlanProcess.startDetached( *settingsStorage->wakeonlanCommand, arguments );

    state = ellClientState_t::BOOTING;
}

void ellClient::Disconnected() {
    delete socket;
    socket = nullptr;
    state = ellClientState_t::DISCONNECTED;
}

void ellClient::SetSocket( QTcpSocket *argSocket ) {
    if ( socket ) {
        socket->abort();
        delete socket;
    }
    socket = argSocket;
    if ( socket->isValid() ) {
        state = ellClientState_t::CONNECTED;
    }
}
