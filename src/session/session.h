/*
 * Copyright 2015-2016 Markus Prasser
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

#ifndef SESSION_H
#define SESSION_H

#include "../client/client.h"
#include "receiptscreator.h"
#include "../settingsstorage.h"
#include "ztree.h"

#include <QDateTime>
#include <QObject>
#include <QTimer>
#include <QVariant>

namespace ell {

//! Represents a session conducted in the laboratory
/*!
  This class represents a session conducted in the laboratory. It stores all settings relevant for this
  session and consists of a 'ell::zTree' instance and optional receipts printing facilities.
 */
class Session : public QObject
{
    Q_OBJECT
public:
    //! 'ell::Session's constructor which will be called by 'EcoLabLib'
    /*!
       This constructor creates a new session. The target path for z-Tree's data will be created and afterwards
       z-Tree will be started. If all required components are available, also a 'ell::ReceiptsCreator' instance
       for automatic receipts creation and printing will be initialized.
       \param argAnonymousReceiptsPlaceholder Placeholder which shall be inserted for participant names if anonymous printing is required
       \param argAssociatedClients This vector contains all clients belonging to this session
       \param argLatexHeaderName The name of the for this session chosen LaTeX header
       \param argAnonReceipts True, if anonymous receipts are required
       \param argSettingsStorage The 'ell::SettingsStorage' containing all settings
       \param argzTreeDataTargetPath The path were z-Tree shall store its data (e.g. the gamesafe file)
       \param argzTreePort The port this session's z-Tree shall listen on
       \param argzTreeVersionString The version this session's z-Tree shall use
       \param argParent 'ell::Session's parent object
     */
    explicit Session( const QString &argAnonymousReceiptsPlaceholder, QVector< Client* > * const argAssociatedClients,
                      const QString &argLatexHeaderName, const bool &argAnonReceipts,
                      const SettingsStorage * const argSettingsStorage, const QString &argzTreeDataTargetPath,
                      const int argzTreePort, const QString &argzTreeVersionString, QObject *argParent = nullptr );
    ~Session();

    /*! Returns the data item with the given index
     *
     * @param argIndex The index of the desired item
     */
    QVariant GetDataItem( int argIndex );
signals:
    /*!
       This signal gets emitted if this session's z-Tree closed. After this the session requests its own cleanup.
       \param argSession The session passes a pointer to itself, so that 'ell::SessionsModel' deletes the correct session
     */
    void SessionFinished( Session *argSession );

public slots:

private:
    const QString * const anonymousReceiptsPlaceholder = nullptr;   //! Placeholder which shall be inserted for participant names if anonymous printing is required
    std::unique_ptr< QVector< Client* > > associatedClients = nullptr; //! This vector stores all clients belonging to this session
    const QString * const latexHeaderName;              //! The name of the chosen LaTeX header
    const bool printAnonymousReceipts = false;          //! True, if anonymous receipts are required
    ReceiptsCreator *receiptsCreator = nullptr;      //! Is used to create and print the receipts
    const SettingsStorage * const settingsStorage = nullptr; //! Contains all external settings
    QString * const zTreeDataTargetPath = nullptr;      //! The path were the data of this session's zTree instance shall be saved
    zTree *zTreeInstance= nullptr;                   //! The session's zTree instance
    const int zTreePort = 7000;         //! The port this session's z-Tree instance shall listen on
    const QString zTreeVersionString;   //! The string of the version of zTree used by this session's instance

private slots:
    //! Starts the session by creating instances of the relevant classes
    void InitializeClasses();
    //! Changes zTree's window title to contain its port number to make z-Tree windows distinguishable
    void RenameWindow();
    //! This slot is called on finish of z-Tree and emits the 'SessionFinished' signal
    void zTreeClosed();
};

}

#endif // SESSION_H
