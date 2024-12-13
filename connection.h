#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
class connection
{
public:
    connection();
    bool createconnect();
};

#endif // CONNECTION_H
