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
};

#endif // SESSIONSMODEL_H
