#ifndef ZTREE_H
#define ZTREE_H

#include "../settingsstorage.h"

#include <QObject>

class ellzTree : public QObject
{
    Q_OBJECT
public:
    explicit ellzTree( const ellSettingsStorage * const argSettingsStorage, const QString &argZTreeDataTargetPath,
                       const int &argZTreePort, const QString &argZTreeVersionPath, QObject *argParent = nullptr );

signals:

public slots:

};

#endif // ZTREE_H
