#include "session.h"

ellSession::ellSession( QObject *argParent ) :
    QObject{ argParent }
{
}

QVariant ellSession::GetDataItem( int argIndex ) {
    switch ( argIndex ) {
    case 0:
        return QVariant{ zTreeVersionPath.split( '_', QString::KeepEmptyParts, Qt::CaseInsensitive )[ 1 ] };
    case 1:
        return QVariant{ zTreePort };
    default:
        return QVariant{};
    }
}
