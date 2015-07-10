#include "sessionsmodel.h"

ellSessionsModel::ellSessionsModel( QObject *argParent ) :
    QAbstractTableModel{ argParent },
    sessionsVector{ new QVector< ellSession* > }
{
}

ellSessionsModel::~ellSessionsModel() {
    for (auto s: *sessionsVector)
        delete s;
    delete sessionsVector;
}

ellSession *ellSessionsModel::back() const {
    return sessionsVector->back();
}

int ellSessionsModel::columnCount( const QModelIndex &argParent ) const {
    Q_UNUSED( argParent );
    return 2;
}

QVariant ellSessionsModel::data( const QModelIndex &argIndex, int argRole ) const {
    if ( !argIndex.isValid() ) {
        return QVariant{};
    }

    if ( argIndex.row() >= sessionsVector->size() || argIndex.row() < 0 ) {
        return QVariant{};
    }

    if ( argRole == Qt::DisplayRole ) {
        return sessionsVector->at( argIndex.row() )->GetDataItem( argIndex.column() );
    }

    return QVariant{};
}

QVariant ellSessionsModel::headerData( int argSection, Qt::Orientation argOrientation, int argRole ) const {
    if ( argOrientation == Qt::Horizontal && argRole == Qt::DisplayRole ) {
        switch(argSection) {
        case 0:
            return tr("zTree Version");
        case 1:
            return tr("Port");
        case 2:
            return tr("LaTeX Header");
        case 3:
            return tr("Anonymous Receipts Placeholder");
        default:
            return QVariant{};
        }
    }
    return QVariant{};
}

void ellSessionsModel::push_back( ellSession *argSession ) {
    sessionsVector->push_back( argSession );
}

int ellSessionsModel::rowCount( const QModelIndex &argParent ) const {
    Q_UNUSED( argParent );
    return sessionsVector->length();
}

