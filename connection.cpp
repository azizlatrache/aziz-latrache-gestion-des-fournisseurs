#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("CPP_Project");//inserer le nom de la source de données
    db.setUserName("system");//inserer nom de l'utilisateur
    db.setPassword("oracle2a32");//inserer mot de passe de cet utilisateur


    qDebug() << "Available drivers: " << QSqlDatabase::drivers();


    if (db.open())
        test=true;





    return  test;


}
