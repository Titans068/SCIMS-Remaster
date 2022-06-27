#ifndef LOGINDAO_H
#define LOGINDAO_H

#include <QDebug>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>


class LoginDAO
{
public:
    LoginDAO();
    void init_connection();
    bool login(QString);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

};

#endif // LOGINDAO_H
