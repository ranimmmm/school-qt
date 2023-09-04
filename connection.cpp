#include "connection.h"

Connection::Connection()
{}

bool Connection::createconnection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Qt");
    db.setUserName("esprit");
    db.setPassword("esprit");

    if (db.open())
        return true;
    return false;
}
void Connection::closeconnection(){db.close();}
