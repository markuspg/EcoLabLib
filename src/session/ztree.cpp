#include "ztree.h"

ellzTree::ellzTree( const ellSettingsStorage * const argSettingsStorage, const QString &argZTreeDataTargetPath,
                    const int &argZTreePort, const QString &argZTreeVersionPath, QObject *argParent ) :
    QObject{ argParent }
{
    QString program;
    QStringList arguments;
#ifdef Q_OS_UNIX
    program = *argSettingsStorage->wineCommand;
    arguments.append( QString{ *argSettingsStorage->zTreeInstallationDirectory + "/zTree_" + argZTreeVersionPath + "/ztree.exe" } );
#else
    program = QString{ *argSettingsStorage->zTreeInstallationDirectory + "/zTree_" + argZTreeVersionPath + "/ztree.exe" };
#endif
    arguments << "/datadir" << QString{ "Z:/" + argZTreeDataTargetPath } << "/privdir" << QString{ "Z:/" + argZTreeDataTargetPath }
              << "/gsfdir" << QString{ "Z:/" + argZTreeDataTargetPath } << "/tempdir" << QString{ "Z:/" + argZTreeDataTargetPath }
              << "/leafdir" << QString{ "Z:/" + argZTreeDataTargetPath } << "/channel" << QString::number( argZTreePort - 7000 );

    ztreeInstance.setProcessEnvironment( *argSettingsStorage->processEnvironment );
    ztreeInstance.startDetached( program, arguments, QDir::currentPath() );
}
