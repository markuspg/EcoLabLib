#ifndef ECOLABLIB_H
#define ECOLABLIB_H

#include "ecolablib_global.h"

#include <QObject>

class ECOLABLIBSHARED_EXPORT EcoLabLib : public QObject {
    Q_OBJECT

public:
    EcoLabLib( QObject *argParent = nullptr );
};

#endif // ECOLABLIB_H
