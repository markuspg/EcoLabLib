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

#ifndef ZTREE_H
#define ZTREE_H

#include <stdexcept>

#include "../settingsstorage.h"

#include <QObject>

//! Starts and stores a z-Tree instance
/*!
  This class starts and stores a z-Tree instance. It passes all important options to z-Tree
  and later keeps track of its finish.
 */
class ellzTree : public QObject
{
    Q_OBJECT
public:
    //! 'ellzTree's constructor which will be called by 'ellSession'
    /*!
     * This constructor starts a new z-Tree instance with all given options.
     * \param argSettingsStorage The 'ellSettingsStorage' containing all settings
     * \param argZTreeDataTargetPath The path were z-Tree shall store all its data
     * \param argZTreePort The port z-Tree shall listen on
     * \param argZTreeVersionPath The path of the z-Tree version which shall be used
     * \param argParent 'ellzTree's parent object
     */
    explicit ellzTree( const ellSettingsStorage * const argSettingsStorage, const QString &argZTreeDataTargetPath,
                       const int &argZTreePort, const QString &argZTreeVersionPath, QObject *argParent = nullptr );

signals:
    /*!
     * \brief This signal is emitted if z-Tree closes
     * \param argExitCode The exit code of the z-Tree process
     * \param argExitStatus The exit status of the z-Tree process
     */
    void zTreeClosed( const int &argExitCode, const QProcess::ExitStatus &argExitStatus );

public slots:

private:
    QProcess ztreeInstance; //! Stores the actual z-Tree process instance
};

#endif // ZTREE_H
