#include "loginDAO.h"

LoginDAO::LoginDAO()
{

}

void LoginDAO::init_connection() {
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("scims");
}

bool LoginDAO::login(QString password) {
    init_connection();
    if (db.open()) {
        QSqlQuery q(db);
        if (q.exec("SELECT password FROM scims_users WHERE username='admin'")) {
            if (q.next()) {
                //qDebug() << q.value("password") << "\n" << password;
                return q.value("password").toString() == password;
            }
        }
        qDebug() << q.lastError().text();
    }
    return false;
}
