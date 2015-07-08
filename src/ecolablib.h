/*
 * Copyright 2015 Markus Prasser
 *
 * This file is part of Labcontrol.
 *
 *  Labcontrol is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Labcontrol is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Labcontrol.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ECOLABLIB_H
#define ECOLABLIB_H

#include "ecolablib_global.h"
#include "settingsstorage.h"

#include <QDir>
#include <QObject>

class ellBuilder;

class ECOLABLIBSHARED_EXPORT EcoLabLib : public QObject {
    Q_OBJECT

public:
    explicit EcoLabLib( const ellBuilder &argBuilder, QObject *argParent = nullptr );
    ~EcoLabLib();

    bool ShowORSEE();
    bool ShowPreprints();

private:
    QStringList *installedLaTeXHeaders = nullptr;
    QStringList *installedzTreeVersions = nullptr;
    ellSettingsStorage settingsStorage;

    //! Detects installed zTree versions and LaTeX headers and initializes the respective QStringLists
    void DetectInstalledZTreeVersionsAndLaTeXHeaders();
};

#endif // ECOLABLIB_H
