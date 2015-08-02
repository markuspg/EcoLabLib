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

#ifndef BUILDER_H
#define BUILDER_H

#include <QDir>
#include <QFile>
#include <QObject>
#include <QProcessEnvironment>
#include <QSettings>

//! This class is only used for the creation of the 'ellSettingsStorage' of 'EcoLabLib'
/*!
  'ellBuilder' loads all settings for 'EcoLabLib' which will be stored in 'EcoLabLib's
  'settingsStorage' variable. Values which failed to load will be set to 'nullptr'
  to signal this failure to higher level code using those values.
 */
class ellBuilder : public QObject {
    Q_OBJECT
public:
    explicit ellBuilder( QObject *argParent = nullptr );

    QStringList *adminUsers = nullptr;
    QString *browserCommand = nullptr;
    quint16 *defaultReceiptIndex = nullptr;
    QString *dvipsCommand = nullptr;
    QString *ecolablibInstallationDirectory = nullptr;
    QString *fileManager = nullptr;
    quint16 *initialzTreePort = nullptr;
    QStringList *installedLaTeXHeaders = nullptr;
    QStringList *installedzTreeVersions = nullptr;
    QString *killallCommand = nullptr;
    QString *latexCommand = nullptr;
    QString *localzLeafName = nullptr;
    QString *lprCommand = nullptr;
    QString *networkBroadcastAddress = nullptr;
    QString *orseeURL = nullptr;
    QString *postscriptViewer = nullptr;
    const QProcessEnvironment * const processEnvironment = nullptr;
    QString *ps2pdfCommand = nullptr;
    QString *publicKeyPathRoot = nullptr;
    QString *publicKeyPathUser = nullptr;
    QString *rcpCommand = nullptr;
    QString *rmCommand = nullptr;
    QString *serverIP = nullptr;
    quint16 *serverPort = nullptr;
    QString *sshCommand = nullptr;
    QString *terminalEmulatorCommand = nullptr;
    QString *userNameOnClients = nullptr;
    QString *vncViewer = nullptr;
    QString *wakeonlanCommand = nullptr;
    QString *webcamDisplayCommand = nullptr;
    QStringList *webcamNames = nullptr;
    QStringList *webcamURLs = nullptr;
    QString *wineCommand = nullptr;
    QString *wmctrlCommand = nullptr;
    QString *xsetCommand = nullptr;
    QString *zTreeInstallationDirectory = nullptr;

signals:

public slots:

private:
    QSettings settings;     //! Is used to load all settings' values

    //! Checks if the specified path exists and returns false if not
    /**
     * @param argPath   The path which shall be tested for its existance
     * @return          True, if the path could be found, false otherwise
     */
    bool CheckPath( const QString * const argPath );
    //! Tries to convert the given string to a quint16
    /**
     * @param argValueString    The string which shall be converted to a numeric value
     * @return                  A pointer to the quint16 on success, otherwise nullptr
     */
    quint16 *ConvertToNumber( QString *& argValueString );
    //! Detects installed zTree versions and LaTeX headers and initializes the respective QStringLists (settings must be read before!)
    void DetectInstalledZTreeVersionsAndLaTeXHeaders();
    //! Reads all settings from QSettings
    void ReadSettings();
    //! Reads the specified settings item and returns the value represented by it
    /**
     * @param argVariableName   The name of the variable which shall be read
     * @param argIsFile         Should be true, if the variable is a file and should be checked for its existance
     * @return                  The represented value, if reading was successful, otherwise 'nullptr'
     */
    QString *ReadSettingsItem( const QString &argVariableName, const bool &argIsFile );
    //! Tries to convert the given QString to a QStringList by splitting at argSep
    /**
     * @param argSep            The seperator character, at whom shall be split
     * @param argListString     The QString which shall be split
     * @return                  The QStringList on success, nullptr otherwise
     */
    QStringList *SplitStringListsToStrings( const QChar &argSep, QString *& argListString );
};

#endif // BUILDER_H
