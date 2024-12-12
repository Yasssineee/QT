#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("CPP_Project");  // Your ODBC data source name
    db.setUserName("hamza");            // Your username
    db.setPassword("hamza");            // Your passwo

if (db.open())
test=true;





    return  test;
}
void Connection::closeconnection(){db.close();}
