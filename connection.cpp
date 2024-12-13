#include "connection.h"

connection::connection()
{

}

bool connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("CPP_Project");//inserer le nom de la source de données
    db.setUserName("wiem");//inserer nom de l'utilisateur
    db.setPassword("wiem");//inserer mot de passe de cet utilisateur

    if (db.open())
        test=true;



    return  test;
}
