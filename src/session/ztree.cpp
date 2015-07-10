#include "ztree.h"

ellzTree::ellzTree( const ellSettingsStorage * const argSettingsStorage, const QString &argZTreeDataTargetPath,
                    const int &argZTreePort, const QString &argZTreeVersionPath, QObject *argParent ) :
    QObject{ argParent }
{
    QString program{ *argSettingsStorage->ecolablibInstallationDirectory + "/scripts/start_zTree_labcontrol2.sh" };
    QStringList arguments;
    arguments << *argSettingsStorage->zTreeInstallationDirectory << argZTreeVersionPath << argZTreeDataTargetPath << QString::number( static_cast<int>( argZTreePort ) - 7000 );

    ztreeInstance.setProcessEnvironment( *argSettingsStorage->processEnvironment );
    ztreeInstance.startDetached( program, arguments, QDir::currentPath() );
}
