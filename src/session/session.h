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

#ifndef ELLSESSION_H
#define ELLSESSION_H

#include "../client/client.h"
#include "receiptscreator.h"
#include "../settingsstorage.h"
#include "ztree.h"

#include <QDateTime>
#include <QObject>
#include <QTimer>
#include <QVariant>


class ellSession : public QObject
{
    Q_OBJECT
public:
    explicit ellSession( const QString &argAnonymousReceiptsPlaceholder, QVector< ellClient* > * const argAssociatedClients,
                         const QString &argLatexHeaderName, const bool &argAnonReceipts,
                         const ellSettingsStorage * const argSettingsStorage, const QString &argzTreeDataTargetPath,
                         const int argzTreePort, const QString &argzTreeVersionPath, QObject *argParent = nullptr );
    ~ellSession();

    /*! Returns the data item with the given index
     *
     * @param argIndex      The index of the desired item
     */
    QVariant GetDataItem( int argIndex );
signals:
    void SessionFinished( ellSession *argSession );

public slots:

private:
    const QString * const anonymousReceiptsPlaceholder = nullptr;   //! Placeholder which shall be inserted for participant names if anonymous printing is desired (QString != "")
    QVector< ellClient* > *associatedClients = nullptr;             //! This vector stores all clients belonging to this session
    const QString * const latexHeaderName;                          //! The name of the chosen LaTeX header
    const bool printAnonymousReceipts = false;
    ellReceiptsCreator *receiptsCreator = nullptr;
    const ellSettingsStorage * const settingsStorage = nullptr;
    QString * const zTreeDataTargetPath = nullptr;                  //! The path were the data of this zTree instance's session will be saved
    ellzTree *zTreeInstance= nullptr;                            //! The session's zTree instance
    const int zTreePort = 7000;
    const QString zTreeVersionPath;                             //! The path to the version of zTree used by this session's instance

private slots:
    /*! Starts the session by creating instances of the relevant classes
     */
    void InitializeClasses();
    /*! Changes zTree's window title to contain its port number to make zTree windows distinguishable
     */
    void RenameWindow();
    void zTreeClosed();
};

#endif // ELLSESSION_H
