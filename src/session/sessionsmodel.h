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

#ifndef SESSIONSMODEL_H
#define SESSIONSMODEL_H

#include "session.h"

#include <QAbstractTableModel>

class ellSessionsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ellSessionsModel( QObject *argParent = nullptr );
    ~ellSessionsModel();
    ellSessionsModel( const ellSessionsModel& ) = delete;
    ellSession *back() const;
    int columnCount( const QModelIndex &argParent ) const;
    QVariant data( const QModelIndex &argIndex, int argRole ) const;
    QVariant headerData( int argSection, Qt::Orientation argOrientation, int argRole ) const;
    void push_back( ellSession *argSession );
    int rowCount( const QModelIndex &argParent ) const;


signals:

public slots:

private:
    QVector< ellSession* > *sessionsVector;

private slots:
    void CleanupFinishedSession( ellSession *argSession );
};

#endif // SESSIONSMODEL_H
