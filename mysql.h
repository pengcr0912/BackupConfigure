#ifndef MYSQL_H
#define MYSQL_H

#include <QtSql/QSqlDatabase>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class mySql
{
public:
    mySql();
    void open();
    QString insertline;
    QSqlDatabase db;

};

#endif // MYSQL_H
