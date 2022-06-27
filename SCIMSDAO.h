#ifndef SCIMSDAO_H
#define SCIMSDAO_H
#include <QMessageBox>
#include <QLineEdit>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QTableView>

#include <algorithm>
#include <sstream>
#include <regex>

#include "sqltablemodel.h"

class SCIMSDAO {
public:
	enum Query {
		INVENTORY, ORDERS, DESTOCKED, LOW_STOCK, EXPIRING, SEARCH, MASS, PRICE, DELIVERY, PASSWORD, DELETE
	};

	Query query;

	void set_query(Query q);

    void init_connection();

    void read(QTableView*, QLineEdit* = NULL);

	bool insert(QVector<QString>);

    bool update(QVector<QString>);

	bool delete_(QVector<QString>);

	bool save(QTableView*);

    int expiring();

    int low();

	~SCIMSDAO();
private:
	SqlTableModel* model;

	Query get_query();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
};
#endif // SCIMSDAO_H
