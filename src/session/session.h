#ifndef ELLSESSION_H
#define ELLSESSION_H

#include <QObject>
#include <QVariant>

class ellSession : public QObject
{
    Q_OBJECT
public:
    explicit ellSession( QObject *argParent = nullptr );

    const int zTreePort = 7000;                         //! The port this session's zTree instance is running on
    const QString zTreeVersionPath;                     //! The path to the version of zTree used by this session's instance

    /*! Returns the data item with the given index
     *
     * @param argIndex      The index of the desired item
     */
    QVariant GetDataItem( int argIndex );
signals:

public slots:

};

#endif // ELLSESSION_H
