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

#ifndef ECOLABLIB_H
#define ECOLABLIB_H

#include "client/clientmanager.h"
#include "ecolablib_global.h"
#include "settingsstorage.h"
#include "session/sessionsmodel.h"

#include <QObject>

class ellBuilder;

//! Merges all facilities together and is the primary interface to 'Labcontrol'
/*!
  This class is the heart of 'EcoLabLib'. It contains and merges all functionality
  and exposes it via its interfaces to 'Labcontrol'.
 */
class ECOLABLIBSHARED_EXPORT EcoLabLib : public QObject {
    Q_OBJECT

public:
    //! 'EcoLabLib's constructor which will be called by 'Labcontrol'
    /*!
       This constructor initializes 'EcoLabLib's three central components,
       the 'ellSessionsModel' storing the running sessions, the 'ellSettingsStorage'
       doing the same for the settings and also the 'ellClientManager' who
       manages the connetions to the clients and controls them.
       \param argBuilder    The builder used to construct 'ellSettingsStorage'
       \param argParent     'EcoLabLib's parent object
     */
    explicit EcoLabLib( const ellBuilder &argBuilder, QObject *argParent = nullptr );

    //! Returns the clients managed by 'ellClientManager'
    /*!
       This function returns the clients managed by 'ellClientManager' and is used
       to initialize the table views displaying the clients.
       \return A QVector containing all managed (=existing) clients
     */
    const QVector< ellClient* > *GetClients() const { return clientManager.GetClients(); }
    /*!
       This function returns true, if the user running 'Labcontrol' has
       administrative rights, otherwise it returns false.
       \return True, if the user has administrative rights, otherwise false
     */
    bool GetIfUserIsAdmin() const { return userIsAdmin; }
    //! Returns the 'ellSettingsStorage'
    /*!
       This function returns the 'ellSettingsStorage' of 'EcoLabLib' which is also
       plentiful used by 'Labcontrol'.
       \return 'EcoLabLib's 'ellSettingsStorage' instance
     */
    const ellSettingsStorage *GetSettingsStorage() { return settingsStorage; }
    //! This function kills all z-Leaves running locally on the server
    void KillLocalzLeaves();
    /*!
       \brief This function shows ORSEE in a locally opened browser window
       \return True on successfull start, otherwise false
     */
    bool ShowORSEE();
    /*!
       \brief This function shows the preprints folder in a locally opened file manager
       \return True on successfull start, otherwise false
     */
    bool ShowPreprints();
    /*!
       \brief This function shows the specified URL in an external webcam viewer
       \param argWebcamURL The URL to be displayed
       \return True on successfull start, otherwise false
     */
    bool ShowWebcam( const QString &argWebcamURL );
    //! Start a local z-Leaf on the server
    /*!
       This function starts a local z-Leaf on the server.
       \param argzLeafName The name with which the started z-Leaf shall connect to z-Tree
       \param argzLeafVersion The version the started z-Leaf shall have
       \param argzTreePort The port on which the started z-Leaf shall conntect to z-Tree
     */
    void StartLocalzLeaf( const QString &argzLeafName, const QString &argzLeafVersion, const int &argzTreePort );

public slots:
    //! Starts a new session with the specified parameters
    /*!
       This slots starts a new session with the specified parameters. The session will afterwards be added to the 'ellSessionsModel' and be terminated and cleaned up on the finish of z-Tree
       \param argAssociatedClients The clients which belong to this session
       \param argParticipiantNameReplacement A replacement for the participant names, if anonymous receipts are required
       \param argPrintAnonymousReceipts True, if anonymous receipts are required
       \param argReceiptsHeader The header which shall be used for receipts creation
       \param argzTreeDataTargetPath The path, where z-Tree shall store its data (e.g. gamesafe file)
       \param argzTreePort The port on which the z-Tree of this session shall listen
       \param argzTreeVersion The version this session's z-Tree shall use
     */
    void StartNewSession( QVector< ellClient* > * const argAssociatedClients, const QString &argParticipiantNameReplacement,
                          const bool &argPrintAnonymousReceipts, const QString &argReceiptsHeader,
                          const QString &argzTreeDataTargetPath, const quint16 &argzTreePort, const QString &argzTreeVersion );

private:
    ellSessionsModel *sessionsModel = nullptr;  //! Stores all started sessions
    const ellSettingsStorage * const settingsStorage = nullptr; //! Contains all external settings
    ellClientManager clientManager;             //! Stores and manages all clients
    bool userIsAdmin = false;       //! This is true, if the user has administrative rights, otherwise false

    //! Checks if the user running 'Labcontrol' has administrative rights and sets 'userIsAdmin' accordingly
    void CheckIfUserIsAdmin();

public:
    const QString userName = "";    //! The name of the user running 'Labcontrol'
};

#endif // ECOLABLIB_H
