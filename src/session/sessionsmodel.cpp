/*
 * Copyright 2015-2016 Markus Prasser
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

#include "sessionsmodel.h"

ell::SessionsModel::SessionsModel( QObject *argParent ) :
    QAbstractTableModel{ argParent },
    sessionsVector{ new QVector< Session* > }
{
}

ell::SessionsModel::~SessionsModel() {
    for (auto s: *sessionsVector)
        delete s;
    delete sessionsVector;
}

ell::Session *ell::SessionsModel::back() const {
    return sessionsVector->back();
}

void ell::SessionsModel::CleanupFinishedSession( Session *argSession ) {
    int finishedSessionIndex = sessionsVector->indexOf( argSession );
    delete argSession;
    sessionsVector->remove( finishedSessionIndex );
}

int ell::SessionsModel::columnCount( const QModelIndex &argParent ) const {
    Q_UNUSED( argParent );
    return 2;
}

QVariant ell::SessionsModel::data( const QModelIndex &argIndex, int argRole ) const {
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

QVariant ell::SessionsModel::headerData( int argSection, Qt::Orientation argOrientation, int argRole ) const {
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

void ell::SessionsModel::push_back( Session *argSession ) {
    connect( argSession, SIGNAL( SessionFinished( Session* ) ),
             this, SLOT( CleanupFinishedSession( Session* ) ) );
    sessionsVector->push_back( argSession );
}

int ell::SessionsModel::rowCount( const QModelIndex &argParent ) const {
    Q_UNUSED( argParent );
    return sessionsVector->length();
}

