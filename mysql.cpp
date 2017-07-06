#include "mysql.h"

mySql::mySql()
{

}

void mySql::open()
{

//    QStringList drivers = QSqlDatabase::drivers();
//    qDebug() << drivers;
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("jtgl");
    db.setUserName("root");
    db.setPassword("840912");
    if(db.open())
    {
        qDebug() << "succeed！";
    }
    else
        qDebug() << db.lastError();
}
